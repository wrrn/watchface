#include <pebble.h>

static Window *s_main_window;
static TextLayer *s_time_layer;
static GFont s_time_font;
static GColor background_color;
static GColor foreground_color;


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
                                   GRect(0, PBL_IF_ROUND_ELSE(60, 52), bounds.size.w, 58)
                                   );

  // Improve the layout to be more like a watchface
  text_layer_set_background_color(s_time_layer, background_color);
  text_layer_set_text_color(s_time_layer, foreground_color);
  text_layer_set_text(s_time_layer, "00:00");


  // Font
  // Create GFont
  s_time_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_BUNGEE_48));
  // Apply to TextLayer
  text_layer_set_font(s_time_layer, s_time_font);

  // Alignment
  text_layer_set_text_alignment(s_time_layer, GTextAlignmentCenter);
  
  // Add it as a child layer to the Window's root layer
  layer_add_child(window_layer, text_layer_get_layer(s_time_layer));
  
}

static void main_window_unload(Window *window) {
  // Destroy the TextLayer
  text_layer_destroy(s_time_layer);

  // Unload GFont
  fonts_unload_custom_font(s_time_font);
}

static Window* init() {
  // Create main Window element and assign to pointer
  Window *w = window_create();

  // Set Background color of window
  window_set_background_color(w, background_color);

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

  background_color = GColorBlack;
  foreground_color = GColorWhite; 

  s_main_window = init();
  app_event_loop();
  deinit(s_main_window);
}
