#pragma once
#include <pebble.h>

static void window_load(Window* window);

static void window_unload(Window* window);

static void in_dropped_handler(AppMessageResult reason, void *context);

static void out_failed_handler(DictionaryIterator *failed, AppMessageResult reason, void *context);

static void in_recieved_handler(DictionaryIterator *iter, void *context);

void trigger_window_push();

void trigger_haptic_feedback();

void get_bad_balance();

void init();

void deinit();




