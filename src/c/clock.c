//
// Copyright (C) 2016, Vinodh Kumar M. <GreenHex@gmail.com>
//

#include <pebble.h>
#include "global.h"
#include "clock.h"
#include "utils.h"
#include "date.h"
#include "battery.h"
#include "animation.h"

tm tm_time;

static Layer *window_layer = 0;
static Layer *dial_layer = 0;
static BitmapLayer *snooze_layer = 0;
static Layer *hours_layer = 0;
static Layer *minutes_layer = 0;
static Layer *seconds_layer = 0;
#ifdef SHOW_SECONDS
static bool show_seconds = true;
#else
static bool show_seconds = false;
#endif
static AppTimer *secs_display_apptimer = 0;

static uint32_t const two_segments[] = { 200, 200, 200 };
VibePattern double_vibe_pattern = {
  .durations = two_segments,
  .num_segments = ARRAY_LENGTH( two_segments ),
};

static void handle_clock_tick( struct tm *tick_time, TimeUnits units_changed ) {
  tm_time = *tick_time; // copy to global
    
  #ifdef DEBUG
  APP_LOG( APP_LOG_LEVEL_INFO, "clock.c: handle_clock_tick(): %d:%d:%d", tm_time.tm_hour, tm_time.tm_min, tm_time.tm_sec );
  #endif
  
  layer_mark_dirty( dial_layer );
  if ( ( units_changed & HOUR_UNIT ) && ( !quiet_time_is_active() ) ) vibes_enqueue_custom_pattern( double_vibe_pattern );
}

static void dial_layer_update_proc( Layer *layer, GContext *ctx ) {
  GRect bounds = layer_get_bounds( layer );
  graphics_context_set_antialiased( ctx, true );
  graphics_context_set_fill_color( ctx, BACKGROUND_COLOUR );
  graphics_fill_rect( ctx, bounds, 0, GCornerNone );
  draw_seconds_ticks( & (DRAW_TICKS_PARAMS) { 
    .layer = layer, 
    .ctx = ctx, 
    .p_gpath_info = &PATH_TICK, 
    .increment = 1, 
    .tick_thk = 1, 
    .tick_length = 6,
    #ifdef REVERSE
    .tick_colour = GColorLightGray, 
    #else
    .tick_colour = GColorDarkGray, 
    #endif
    .bg_colour = BACKGROUND_COLOUR
  } );
  draw_seconds_ticks( & (DRAW_TICKS_PARAMS) { 
    .layer = layer, 
    .ctx = ctx, 
    .p_gpath_info = &PATH_TICK, 
    .increment = 5, 
    .tick_thk = 1, 
    .tick_length = 10, 
    .tick_colour = TICKS_COLOUR, 
    .bg_colour = BACKGROUND_COLOUR
  } );
  draw_seconds_ticks( & (DRAW_TICKS_PARAMS) {
    .layer = layer,
    .ctx = ctx,
    .p_gpath_info = &PATH_TICK,
    .increment = 15,
    .tick_thk = 3,
    .tick_length = 15,
    .tick_colour = TICKS_COLOUR, 
    .bg_colour = BACKGROUND_COLOUR
  } );
  graphics_context_set_stroke_color( ctx, BACKGROUND_COLOUR );
  graphics_context_set_stroke_width( ctx, CLOCK_TICK_EDGE_OFFSET );
  graphics_draw_round_rect( ctx, grect_inset( bounds, GEdgeInsets( CLOCK_TICK_EDGE_OFFSET / 2 ) ), 0 );
 
}

static void snooze_layer_update_proc( Layer *layer, GContext *ctx ) {
  if ( quiet_time_is_active() ) {
    GRect bounds = layer_get_bounds( layer );
    graphics_context_set_compositing_mode( ctx, GCompOpSet );
    #ifdef REVERSE
    GBitmap *snooze_bitmap = gbitmap_create_with_resource( RESOURCE_ID_IMAGE_MOUSE_W );
    #else
    GBitmap *snooze_bitmap = gbitmap_create_with_resource( RESOURCE_ID_IMAGE_MOUSE_B );
    #endif
    graphics_draw_bitmap_in_rect( ctx, snooze_bitmap, bounds );
    gbitmap_destroy( snooze_bitmap );
  }
}

static void hours_layer_update_proc( Layer *layer, GContext *ctx ) {
  GRect bounds = layer_get_bounds( layer );
  GPoint center_pt = grect_center_point( &bounds );
  
  uint32_t hour_angle = ( TRIG_MAX_ANGLE * ( ( ( tm_time.tm_hour % 12 ) * 6 ) + ( tm_time.tm_min / 10 ) ) ) / ( 12 * 6 );  
  draw_gpath_hands( & (GPATH_HANDS_PARAMS) { 
    .ctx = ctx, 
    .center_pt = center_pt, 
    .angle = hour_angle, 
    .gpath_hand = &HOUR_HAND_SBGE001_POINTS, 
    .gpath_hand_highlight = &HOUR_HAND_SBGE001_POINTS_HIGHLIGHT_2,
    .hand_colour = HAND_COLOUR,
    .hand_highlight_colour = HAND_HIGHLIGHT_COLOUR,
    .hand_outline_colour = HAND_OUTLINE_COLOUR,
    .center_dot_radius = 2,
    .center_dot_colour = GColorLightGray /* BACKGROUND_COLOUR */
  } );
}

static void minutes_layer_update_proc( Layer *layer, GContext *ctx ) {
  GRect bounds = layer_get_bounds( layer );
  GPoint center_pt = grect_center_point( &bounds );
  
  uint32_t minute_angle = TRIG_MAX_ANGLE * tm_time.tm_min / 60;
  draw_gpath_hands( & (GPATH_HANDS_PARAMS) { 
    .ctx = ctx, 
    .center_pt = center_pt, 
    .angle = minute_angle, 
    .gpath_hand = &MINUTE_HAND_SBGE001_POINTS, 
    .gpath_hand_highlight = &MINUTE_HAND_SBGE001_POINTS_HIGHLIGHT_2,
    .hand_colour = HAND_COLOUR,
    .hand_highlight_colour = HAND_HIGHLIGHT_COLOUR,
    .hand_outline_colour = HAND_OUTLINE_COLOUR,
    .center_dot_radius = 2,
    .center_dot_colour = CENTER_DOT_COLOUR
  } );
}

#ifdef USE_GPATH_SECONDS_HAND
static void seconds_layer_update_proc( Layer *layer, GContext *ctx ) {
  if ( !show_seconds ) return;
  
  GRect bounds = layer_get_bounds( layer );
  GPoint center_pt = grect_center_point( &bounds );
  
  uint32_t sec_angle = TRIG_MAX_ANGLE * tm_time.tm_sec / 60;
    
  draw_gpath_hands( & (GPATH_HANDS_PARAMS) { 
    .ctx = ctx, 
    .center_pt = center_pt, 
    .angle = sec_angle, 
    .gpath_hand = &SECOND_HAND_POINTS, 
    .gpath_hand_highlight = &SECOND_HAND_POINTS,
    .hand_colour = GColorRed,
    .hand_highlight_colour = GColorRed,
    .hand_outline_colour = GColorRed,
    .center_dot_radius = 3,
    .center_dot_colour = CENTER_DOT_COLOUR
  } );
}
#else
static void seconds_layer_update_proc( Layer *layer, GContext *ctx ) {
  if ( !show_seconds ) return;
  
  GRect bounds = layer_get_bounds( layer );
  GPoint center_pt = grect_center_point( &bounds );
  
  uint32_t sec_angle = TRIG_MAX_ANGLE * tm_time.tm_sec / 60;
  uint32_t sec_tail_angle = ( TRIG_MAX_ANGLE * tm_time.tm_sec / 60 ) + ( TRIG_MAX_ANGLE / 2 );
  GPoint sec_hand = (GPoint) {
    .x = ( sin_lookup( sec_angle ) * SEC_HAND_LENGTH / TRIG_MAX_RATIO ) + center_pt.x,
    .y = ( -cos_lookup( sec_angle ) * SEC_HAND_LENGTH / TRIG_MAX_RATIO ) + center_pt.y
  };    
  GPoint sec_hand_tail = (GPoint) {
    .x = ( sin_lookup( sec_tail_angle ) * SEC_HAND_TAIL_LENGTH / TRIG_MAX_RATIO ) + center_pt.x,
    .y = ( -cos_lookup( sec_tail_angle ) * SEC_HAND_TAIL_LENGTH / TRIG_MAX_RATIO ) + center_pt.y
  }; 
  
  draw_clock_hand( & (HAND_DRAW_PARAMS) {
    .ctx = ctx,
    .center_pt = center_pt,
    .from_pt = sec_hand,
    .to_pt = sec_hand_tail,
    .hand_width = SEC_HAND_WIDTH,
    .hand_colour = COLOUR_SEC_HAND,
    .hand_outline_colour = COLOUR_BG_BITMAP_BG,
    .dot_radius = SEC_CENTER_DOT_RADIUS,
    .dot_colour = COLOUR_SEC_HAND,
    .dot_outline_colour = COLOUR_DOT_OUTLINE
  } );
  
  #if defined( PBL_COLOR )
  GPoint sec_hand_tip = (GPoint) {
    .x = ( sin_lookup( sec_angle ) * ( SEC_HAND_LENGTH - SEC_HAND_TIP_LENGTH ) / TRIG_MAX_RATIO ) + center_pt.x,
    .y = ( -cos_lookup( sec_angle ) * ( SEC_HAND_LENGTH - SEC_HAND_TIP_LENGTH ) / TRIG_MAX_RATIO ) + center_pt.y
  };
  graphics_context_set_stroke_width( ctx, SEC_HAND_WIDTH );
  graphics_context_set_stroke_color( ctx, COLOUR_SEC_HAND_TIP );
  graphics_draw_line( ctx, sec_hand, sec_hand_tip );
  #endif
}
#endif

#ifndef SHOW_SECONDS
static void stop_seconds_display( void* data ) { // after timer elapses
  secs_display_apptimer = 0;
  show_seconds = false;
  tick_timer_service_subscribe( MINUTE_UNIT, handle_clock_tick );
}

static void start_seconds_display( AccelAxisType axis, int32_t direction ) {
  tick_timer_service_subscribe( SECOND_UNIT, handle_clock_tick );
  show_seconds = true;
  if ( secs_display_apptimer ) {
    app_timer_reschedule( secs_display_apptimer, SHOW_SECONDS_TIMER_TIMEOUT_MS );
  } else {
    secs_display_apptimer = app_timer_register( SHOW_SECONDS_TIMER_TIMEOUT_MS, stop_seconds_display, 0 );
  }
}
#endif

static void unobstructed_change_proc( AnimationProgress progress, void *context ) {
  GRect uo_bounds = layer_get_unobstructed_bounds( (Layer *) context );
  GRect bounds = layer_get_bounds( (Layer *) context );
  layer_set_bounds( dial_layer, uo_bounds );
  layer_set_bounds( hours_layer, uo_bounds );
  layer_set_bounds( minutes_layer, uo_bounds );
  layer_set_bounds( seconds_layer, uo_bounds );
  GRect outer_window_frame = OUTER_WINDOW_FRAME;
  outer_window_frame.origin.y = uo_bounds.origin.y + uo_bounds.size.h/2 - outer_window_frame.size.h/2;
  layer_set_frame( outer_window_layer, outer_window_frame );
  GRect battery_gauge_frame = BATTERY_GAUGE_FRAME;
  battery_gauge_frame.origin.y = uo_bounds.origin.y + uo_bounds.size.h/2 - battery_gauge_frame.size.h/2;
  layer_set_frame( battery_layer, battery_gauge_frame );
  layer_mark_dirty( dial_layer );
  GRect snooze_layer_frame = SNOOZE_LAYER_FRAME;
  snooze_layer_frame.origin.y = snooze_layer_frame.origin.y * uo_bounds.size.h / bounds.size.h;
  layer_set_frame( bitmap_layer_get_layer( snooze_layer ), snooze_layer_frame );
  layer_mark_dirty( bitmap_layer_get_layer( snooze_layer ) );
}

void clock_init( Window* window ){
  
  time_t now = time( NULL ) - ( 60 * 45 );
  tm_time = *localtime( &now );
  
  window_layer = window_get_root_layer( window );
  
  dial_layer = layer_create( CLOCK_DIAL_RECT );
  layer_set_update_proc( dial_layer, dial_layer_update_proc );
  layer_add_child( window_layer, dial_layer );
  
  snooze_layer = bitmap_layer_create( SNOOZE_LAYER_FRAME );
  layer_set_update_proc( bitmap_layer_get_layer( snooze_layer ), snooze_layer_update_proc );
  layer_add_child( dial_layer, bitmap_layer_get_layer( snooze_layer ) );
  
  date_init( dial_layer );
  battery_init( dial_layer );
  
  hours_layer = layer_create( CLOCK_DIAL_RECT );
  layer_set_update_proc( hours_layer, hours_layer_update_proc );
  layer_add_child( dial_layer, hours_layer );
  
  minutes_layer = layer_create( CLOCK_DIAL_RECT );
  layer_set_update_proc( minutes_layer, minutes_layer_update_proc );
  layer_add_child( dial_layer, minutes_layer );
  
  seconds_layer = layer_create( CLOCK_DIAL_RECT );
  layer_set_update_proc( seconds_layer, seconds_layer_update_proc );
  layer_add_child( dial_layer, seconds_layer );
  
  start_animation( 0, 1200, AnimationCurveEaseInOut, (void *) dial_layer );
}

void implementation_teardown( Animation *animation ) {
  unobstructed_area_service_subscribe( (UnobstructedAreaHandlers) { .change = unobstructed_change_proc }, window_layer );

  #ifdef SHOW_SECONDS
  tick_timer_service_subscribe( SECOND_UNIT, handle_clock_tick );
  #else
  tick_timer_service_subscribe( MINUTE_UNIT, handle_clock_tick );
  #endif
  accel_tap_service_subscribe( start_seconds_display );
  
  time_t now = time( NULL );
  handle_clock_tick( localtime( &now ), 0 );

  animation_destroy( animation );
}

void clock_deinit( void ){
  if ( seconds_layer ) layer_destroy( seconds_layer );
  if ( minutes_layer ) layer_destroy( minutes_layer );
  if ( hours_layer ) layer_destroy( hours_layer );
  battery_deinit();
  date_deinit();
  if ( snooze_layer ) bitmap_layer_destroy( snooze_layer );
  if ( dial_layer ) layer_destroy( dial_layer );
}
