#include "trigger_window.h"

//The TextLayer object that shows the current "balance" (how much left can afford before the balance runs out).
static TextLayer *warning_prompt;
static TextLayer *balance_layer;
//The TextLayer object that is the warning message 
static char balance[9];


//Items that are transfered between the objects.
enum MessageKey {
  MESSAGE_KEY_balance = 0x1,
};

//Declare a Window pointer to hold the address of the trigger_window
static Window* trigger_window;

//Initilize a windowLayer on startup.
static void window_load(Window* window)
{
  Layer *windowLayer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(windowLayer);
  // Create the TextLayer with specific bounds
  warning_prompt = text_layer_create(
      GRect(0, PBL_IF_RECT_ELSE(58, 52), bounds.size.w, 50));

  // Make some minor tweaks to the layout.
  text_layer_set_text_color(warning_prompt, GColorBlack);
  text_layer_set_text(warning_prompt, "Wait! You are in a bad place!");
  text_layer_set_font(warning_prompt, fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD));
  text_layer_set_text_alignment(warning_prompt, GTextAlignmentCenter);
  
  layer_add_child(windowLayer, text_layer_get_layer(warning_prompt));


}

//Destroy the window when we are done, 
static void window_unload(Window* window)
{
    text_layer_destroy(warning_prompt);
    text_layer_destroy(balance_layer);
}

//Following two methods Log any dropped packets between the Pebble and Mobile devices.
static void in_dropped_handler(AppMessageResult reason, void *context) 
{
  APP_LOG(APP_LOG_LEVEL_DEBUG, "App Instruction Dropped!");
}

static void out_failed_handler(DictionaryIterator *failed, AppMessageResult reason, void *context) 
{
  APP_LOG(APP_LOG_LEVEL_DEBUG, "App Instruction Failed to Send!");
}



static void in_recieved_handler(DictionaryIterator *iter, void *context)
{
  //A new message has been successfully recieved.
  Tuple *balance_tuple = dict_find(iter, MESSAGE_KEY_balance);
  if (balance_tuple)
  {
     strncpy(balance, balance_tuple->value->cstring, 9);

     text_layer_set_text(balance_layer, balance);
     text_layer_set_background_color(balance_layer, GColorDarkCandyAppleRed);
  }
 
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


void trigger_haptic_feedback()
{
   //Simply call the double vibrate from Vibes to trigger.
   vibes_double_pulse();
}

void get_bad_balance()
{
  //Get data from the phone and parse it for the bad balance. 
  
}

void init()
{
   // Register the file handlers
  app_message_register_inbox_received(in_recieved_handler);
  app_message_register_inbox_dropped(in_dropped_handler);
  app_message_register_outbox_failed(out_failed_handler);
  //Initialize the Buffer for incoming and outgoing data.
  app_message_open(64, 64);
  
  //trigger the window push
  trigger_window_push();
  
}

void deinit() 
{
    window_destroy(trigger_window);

}