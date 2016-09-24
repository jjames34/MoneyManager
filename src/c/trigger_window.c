#include "trigger_window.h"

//Declare a Window pointer to hold the address of the trigger_window
static Window* trigger_window;

static void window_load(Window* window)
{
  Layer *windowLayer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(windowLayer);
}

static void window_unload(Window* window)
{
    window_destroy(window);
}


void trigger_window_push() 
{
    //First check if the trigger_window has been instantiated.
    if (!trigger_window)
      {
        trigger_window = window_create();
        window_set_window_handlers(trigger_window, (WindowHandlers) {
            .load = window_load,
            .unload = window_unload,
        });
      }
    window_stack_push(trigger_window, true);
}
