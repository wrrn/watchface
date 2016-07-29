#include <pebble.h>

static  Window *s_main_window;
static TextLayer *s_time_layer;

static void update_time(struct tm *tick_time) {
  static char s_buffer[8];
  strftime(s_buffer, sizeof(s_buffer),
           clock_is_24h_style() ? "%H:%M" :
                                  "%I:%M",
           tick_time
           );
  text_layer_set_text(s_time_layer, s_buffer);

}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  update_time(tick_time);
}

static void display_current_time() {
  time_t temp = time(NULL);
  update_time(localtime(&temp));
}





static void main_window_load(Window *window) {
  // Get information about the Window
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);

  //Create the TextLayer with specific bounds
  s_time_layer = text_layer_create(
                                   GRect(0, PBL_IF_ROUND_ELSE(58, 52), bounds.size.w, 50)
                                   );

  // Improve the layout to be more like a watchface
  text_layer_set_background_color(s_time_layer, GColorClear);
  text_layer_set_text_color(s_time_layer, GColorBlack);
  text_layer_set_text(s_time_layer, "00:00");
  text_layer_set_font(s_time_layer, fonts_get_system_font(FONT_KEY_BITHAM_42_BOLD));
  text_layer_set_text_alignment(s_time_layer, GTextAlignmentCenter);

  // Add it as a child layer to the Window's root layer
  layer_add_child(window_layer, text_layer_get_layer(s_time_layer));
  
}

static void main_window_unload(Window *window) {
  // Destroy the TextLayer
  text_layer_destroy(s_time_layer);
  
}

static Window* init() {
  // Create main Window element and assign to pointer
  Window *w = window_create();

  // Set handlers to manage the elements inside the Window
  window_set_window_handlers(w, (WindowHandlers) {
      .load = main_window_load,
        .unload = main_window_unload
        });

  // Register the TickTimerService
  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
  
  // Show the window on the watch, with animated=true
  window_stack_push(w, true);

  //Make sure the time is displayed from the start
  display_current_time();
  return w;
}

static void deinit(Window *w) {
  // Destroy window
  window_destroy(w);
}

int main(void) {
  s_main_window = init();
  app_event_loop();
  deinit(s_main_window);
}
