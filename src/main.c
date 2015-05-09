#include "pebble.h"
Window *main_window;
Layer *root_layer;
GBitmap *background_bmp;
uint8_t *background;

void load_images() {
  background_bmp = gbitmap_create_with_resource(RESOURCE_ID_BACKGROUND);
  #ifdef PBL_COLOR
    background   = gbitmap_get_data(background_bmp);
  #else
    background   = background_bmp->addr;
  #endif
}

void destroy_images() {
  gbitmap_destroy(background_bmp);
}

//#define screen ((uint32_t*)framebuffer->addr)
void root_layer_update(Layer *me, GContext *ctx) {
  GBitmap* framebuffer = graphics_capture_frame_buffer(ctx);
  if(framebuffer) {
    #ifdef PBL_COLOR
      uint8_t* screen = gbitmap_get_data(framebuffer);
      for(uint16_t i=0; i<144*168; i++)
        screen[i] = background[i];
    #else
      uint8_t* screen = (uint8_t*)(framebuffer->addr);
      for(uint16_t i=0; i<20*168; i++)
        screen[i] = background[i];
    #endif
    graphics_release_frame_buffer(ctx, framebuffer);
  }
}

// ------------------------------------------------------------------------ //
//  Main Functions
// ------------------------------------------------------------------------ //
static void main_window_load(Window *window) {
  root_layer = window_get_root_layer(window);
  layer_set_update_proc(root_layer, root_layer_update);
}

static void init(void) {
  main_window = window_create();
  window_set_window_handlers(main_window, (WindowHandlers) {
    .load = main_window_load,
  });
  
  #ifdef PBL_BW
    window_set_fullscreen(main_window, true);
  #endif
  
  load_images();
  window_stack_push(main_window, true); // Display window
}
  
static void deinit(void) {
  destroy_images();
  window_destroy(main_window);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}