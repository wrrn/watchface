#include <pebble.h>

static void main_window_load(Window *window) {
  
}

static void main_window_unload(Window *window) {
  
}

static Window* init() {
  // Create main Window element and assign to pointer
  Window *s_main_window = window_create();

  // Set handlers to manage the elements inside the Window
  window_set_window_handlers(s_main_window, (WindowHandlers) {
      .load = main_window_load,
        .unload = main_window_unload
        });
  // Show the window on the watch, with animated=true
  window_stack_push(s_main_window, true);
  return s_main_window;
}

static void deinit(Window *w) {
  // Destroy window
  window_destroy(w);
}

int main(void) {
  static Window *s_main_window;
  s_main_window = init();
  app_event_loop();
  deinit(s_main_window);
}
