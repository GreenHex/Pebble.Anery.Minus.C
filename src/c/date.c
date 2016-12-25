//
// Copyright (C) 2016, Vinodh Kumar M. <GreenHex@gmail.com>
//
// Fonts:
// https://fonts.google.com/specimen/BioRhyme
// https://fonts.google.com/specimen/BioRhyme+Expanded
//

#include <pebble.h>
#include "date.h"

Layer *date_layer;

static TextLayer *date_text_layer = 0;

static void date_layer_update_proc( Layer *layer, GContext *ctx ) {
  GRect bounds = layer_get_bounds( layer );
  graphics_context_set_fill_color( ctx, PBL_IF_COLOR_ELSE( GColorLightGray, GColorDarkGray ) );
  graphics_fill_rect( ctx, bounds, DATE_WINDOW_OUTLINE_THK, GCornersAll );

  graphics_context_set_fill_color( ctx, GColorWhite );
  graphics_fill_rect( ctx, grect_inset( bounds, GEdgeInsets( DATE_WINDOW_OUTLINE_THK ) ), 0, GCornersAll );  
}

#define ALTERNATE_FONT
#define DATE_FONT_EXPANDED RESOURCE_ID_FONT_BIORHYME_EXPANDED_REGULAR_20
#define DATE_FONT_NORMAL RESOURCE_ID_FONT_BIORHYME_REGULAR_20

static void date_text_layer_update_proc( Layer *layer, GContext *ctx ) {
  GRect bounds = layer_get_bounds( layer );
  static char date_text[3] = "";
  
  #if defined(PBL_COLOR)
  GColor text_colour = ( tm_time.tm_wday == 0 ) ? GColorOrange : ( tm_time.tm_wday == 6 ) ? GColorBlueMoon : GColorBlack;
  #else
  GColor text_colour = GColorBlack;
  #endif
  graphics_context_set_text_color( ctx, text_colour );
  snprintf( date_text, sizeof( date_text ), "%d", tm_time.tm_mday );
  
  #ifdef ALTERNATE_FONT
  GFont font = ( tm_time.tm_mday < 10 ) ?
                      fonts_load_custom_font( resource_get_handle( DATE_FONT_EXPANDED ) ) :
                      fonts_load_custom_font( resource_get_handle( DATE_FONT_NORMAL ) );

  bounds.origin.y -= ( DATE_TXT_VERT_ADJ - 1 );
  graphics_draw_text( ctx, date_text, font, bounds,
                     GTextOverflowModeTrailingEllipsis, GTextAlignmentCenter, NULL );
  fonts_unload_custom_font( font );
  #else
  bounds.origin.y -= DATE_TXT_VERT_ADJ;
  graphics_draw_text( ctx, date_text, fonts_get_system_font( FONT_KEY_ROBOTO_CONDENSED_21 ), bounds,
                     GTextOverflowModeTrailingEllipsis, GTextAlignmentCenter, NULL );
  #endif
}

void date_init( Layer *parent_layer ) {
  date_layer = layer_create( DATE_WINDOW_FRAME );
  layer_set_update_proc( date_layer, date_layer_update_proc );
  layer_add_child( parent_layer, date_layer );
  
  date_text_layer = text_layer_create( grect_inset( DATE_WINDOW_BOUNDS, GEdgeInsets( DATE_WINDOW_OUTLINE_THK ) ) );
  layer_set_update_proc( text_layer_get_layer( date_text_layer ), date_text_layer_update_proc );
  layer_add_child( date_layer, text_layer_get_layer( date_text_layer ) );
}

void date_deinit( void ) {
  layer_destroy( date_layer );
  text_layer_destroy( date_text_layer );
}
