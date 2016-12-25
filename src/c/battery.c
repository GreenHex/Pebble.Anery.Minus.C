//
// Copyright (C) 2016, Vinodh Kumar M. <GreenHex@gmail.com>
//

#include <pebble.h>
#include "battery.h"
#include "utils.h"

Layer *battery_layer;
static BatteryChargeState charge_state;

static void batt_gauge_update_proc( BatteryChargeState state ) {
  charge_state = state;
  layer_mark_dirty( battery_layer );
}

static void battery_layer_update_proc( Layer *layer, GContext *ctx ) {
  graphics_context_set_antialiased( ctx, true );
  graphics_context_set_fill_color( ctx, BACKGROUND_COLOUR );
  graphics_fill_rect( ctx, layer_get_bounds( layer ), 0, GCornerNone );
  
  GPath *gpath = gpath_create( &BATT_GAUGE_TICK ); 
  gpath_move_to( gpath, BATTERY_GAUGE_TICK_POS );
  graphics_context_set_stroke_width( ctx, 1 );
  gpath_rotate_to( gpath, DEG_TO_TRIGANGLE( 0 ) );
  graphics_context_set_stroke_color( ctx, PBL_IF_COLOR_ELSE( GColorLightGray, GColorWhite ) );
  gpath_draw_outline( ctx, gpath );
  gpath_rotate_to( gpath, DEG_TO_TRIGANGLE( BATTERY_GAUGE_MAX_ANGLE_DEG ) );
  graphics_context_set_stroke_color( ctx, PBL_IF_COLOR_ELSE( GColorIslamicGreen, GColorWhite ) );
  gpath_draw_outline( ctx, gpath );
  gpath_rotate_to( gpath, DEG_TO_TRIGANGLE( -BATTERY_GAUGE_MAX_ANGLE_DEG ) );
  graphics_context_set_stroke_color( ctx, PBL_IF_COLOR_ELSE( GColorOrange, GColorWhite ) );
  gpath_draw_outline( ctx, gpath );
  gpath_destroy( gpath );
  
  uint32_t batt_angle = DEG_TO_TRIGANGLE( (uint32_t) ( ( charge_state.charge_percent * 40 ) / 100 ) + 250 );
  GPoint battery_hand = (GPoint) {
    .x = ( sin_lookup( batt_angle ) * BATT_HAND_LENGTH / TRIG_MAX_RATIO ) + BATTERY_GAUGE_PIVOT.x,
    .y = ( -cos_lookup( batt_angle ) * BATT_HAND_LENGTH / TRIG_MAX_RATIO ) + BATTERY_GAUGE_PIVOT.y
  };
  
  draw_clock_hand( & (HAND_DRAW_PARAMS) {
    .ctx = ctx,
    .center_pt = BATTERY_GAUGE_PIVOT,
    .from_pt = BATTERY_GAUGE_PIVOT,
    .to_pt = battery_hand,
    .hand_width = 1,
    .hand_colour = BATTERY_GAUGE_POINTER_FILL_COLOUR,
    .hand_outline_colour = BATTERY_GAUGE_POINTER_OUTLINE_COLOUR,
    .dot_radius = BATT_GAUGE_DOT_RADIUS,
    .dot_colour = BATTERY_GAUGE_POINTER_FILL_COLOUR,
    .dot_outline_colour = BATTERY_GAUGE_POINTER_OUTLINE_COLOUR
  } );
  
  graphics_context_set_fill_color( ctx, charge_state.is_charging ? GColorJaegerGreen : 
                                  charge_state.charge_percent < 16 ? GColorFolly : GColorDarkGray );
  graphics_fill_circle( ctx, BATTERY_GAUGE_PIVOT, 1 );
}

void battery_init( Layer* parent_layer ) {
  battery_layer = layer_create( BATTERY_GAUGE_FRAME );
  layer_set_update_proc( battery_layer, battery_layer_update_proc );
  layer_add_child( parent_layer, battery_layer );
  
  batt_gauge_update_proc( battery_state_service_peek() );
  battery_state_service_subscribe( batt_gauge_update_proc );
}

void battery_deinit( void ) {
  if ( battery_layer ) layer_destroy( battery_layer );
}