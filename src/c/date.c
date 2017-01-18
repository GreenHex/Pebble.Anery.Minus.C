//
// Copyright (C) 2016, Vinodh Kumar M. <GreenHex@gmail.com>
//
// Fonts:
// https://fonts.google.com/specimen/Inconsolata
// https://fonts.google.com/specimen/BioRhyme
// https://fonts.google.com/specimen/BioRhyme+Expanded
//

#include <pebble.h>
#include "date.h"

Layer *outer_window_layer;
static TextLayer *day_text_layer = 0;
static TextLayer *date_text_layer = 0;

static void outer_window_layer_update_proc( Layer *layer, GContext *ctx ) {
  GRect bounds = layer_get_bounds( layer );
  graphics_context_set_fill_color( ctx, PBL_IF_COLOR_ELSE( GColorDarkGray, GColorDarkGray ) );
  graphics_fill_rect( ctx, bounds, OUTER_WINDOW_OUTLINE_THK, GCornersAll );

  graphics_context_set_fill_color( ctx, GColorLightGray );
  graphics_fill_rect( ctx, grect_inset( bounds, GEdgeInsets( OUTER_WINDOW_OUTLINE_THK ) ), 0, GCornersAll );  
}

#define ALTERNATE_FONT
#define DAY_FONT_NORMAL RESOURCE_ID_FONT_INCONSOLATA_REGULAR_16
#define DATE_FONT_EXPANDED RESOURCE_ID_FONT_BIORHYME_EXPANDED_REGULAR_18
#define DATE_FONT_NORMAL RESOURCE_ID_FONT_BIORHYME_REGULAR_18

static void date_text_layer_update_proc( Layer *layer, GContext *ctx ) {
  GRect bounds = layer_get_bounds( layer );
  static char date_text[] = "31";
  graphics_context_set_fill_color( ctx, GColorWhite );
  graphics_fill_rect( ctx, bounds, 0, GCornersAll );
  
  #if defined(PBL_COLOR_NO_COLOUR)
  GColor text_colour = ( tm_time.tm_wday == 0 ) ? GColorOrange : ( tm_time.tm_wday == 6 ) ? GColorBlueMoon : GColorBlack;
  #else
  GColor text_colour = GColorBlack;
  #endif

  // tm_time.tm_mday = 4;
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

static void day_text_layer_update_proc( Layer *layer, GContext *ctx ) {
  GRect bounds = layer_get_bounds( layer );
  static char day_text[] = "SUN";
  
  graphics_context_set_fill_color( ctx, GColorWhite );
  graphics_fill_rect( ctx, bounds, 0, GCornersAll );
  
  #if defined(PBL_COLOR)
  GColor text_colour = ( tm_time.tm_wday == 0 ) ? GColorOrange : ( tm_time.tm_wday == 6 ) ? GColorBlueMoon : GColorBlack;
  #else
  GColor text_colour = GColorBlack;
  #endif
  // tm_time.tm_wday = 0;
  graphics_context_set_text_color( ctx, text_colour );
  strftime( day_text, sizeof( day_text ), "%a", &tm_time );
  day_text[1] -= 32;
  day_text[2] -= 32;
  bounds.origin.y -= DAY_TXT_VERT_ADJ;
  #ifdef ALTERNATE_FONT
  GFont font = fonts_load_custom_font( resource_get_handle( DAY_FONT_NORMAL ) );
  graphics_draw_text( ctx, day_text, font, bounds,
                     GTextOverflowModeTrailingEllipsis, GTextAlignmentCenter, NULL );
  fonts_unload_custom_font( font );
  #else
  graphics_draw_text( ctx, day_text, fonts_get_system_font( FONT_KEY_GOTHIC_18_BOLD ), bounds,
                     GTextOverflowModeTrailingEllipsis, GTextAlignmentCenter, NULL );
  #endif
}

void date_init( Layer *parent_layer ) {
  outer_window_layer = layer_create( OUTER_WINDOW_FRAME );
  layer_set_update_proc( outer_window_layer, outer_window_layer_update_proc );
  layer_add_child( parent_layer, outer_window_layer );
  
  day_text_layer = text_layer_create( DAY_WINDOW_FRAME );
  layer_set_update_proc( text_layer_get_layer( day_text_layer ), day_text_layer_update_proc );
  layer_add_child( outer_window_layer, text_layer_get_layer( day_text_layer ) );
  
  date_text_layer = text_layer_create( DATE_WINDOW_FRAME );
  layer_set_update_proc( text_layer_get_layer( date_text_layer ), date_text_layer_update_proc );
  layer_add_child( outer_window_layer, text_layer_get_layer( date_text_layer ) );
}

void date_deinit( void ) {
  if (day_text_layer) text_layer_destroy( day_text_layer );
  if (date_text_layer) text_layer_destroy( date_text_layer );
  if (outer_window_layer) layer_destroy( outer_window_layer );
}
