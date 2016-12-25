//
// Copyright (C) 2016, Vinodh Kumar M. <GreenHex@gmail.com>
//

#include <pebble.h>
#include "utils.h"
#include "global.h"

// function is "adjusted"" for whole hours or minutes; "after" 9:00 AM or "upto" 9:00 AM.
// "after" includes the hour, "upto" excludes the hour.
bool is_X_in_range( int a, int b, int x ) { return ( ( b > a ) ? ( ( x >= a ) && ( x < b ) ) : ( ( x >= a ) || ( x < b ) ) ); };

void log_pt( char *str, GPoint pt ) {
  #ifdef DEBUG
  APP_LOG( APP_LOG_LEVEL_INFO, "%s: ( %d, %d )", str, pt.x, pt.y );
  #endif
}

void log_rect( char *str, GRect rect ) {
  #ifdef DEBUG
  APP_LOG( APP_LOG_LEVEL_INFO, "%s: ( %d, %d, %d, %d )", str, rect.origin.x, rect.origin.y, rect.size.w, rect.size.h );
  #endif
}

void draw_seconds_ticks( DRAW_TICKS_PARAMS *pDTP ) {
  GRect bounds = layer_get_bounds( pDTP->layer );
  GPoint center_pt = grect_center_point( &bounds );
  GPath *gpath = gpath_create( pDTP->p_gpath_info );
  graphics_context_set_antialiased( pDTP->ctx, true );
  
  for ( int i = 0, angle = 0; i < 30; i += pDTP->increment ) { // drawing half the ticks is *probably* more efficient
    angle = TRIG_MAX_ANGLE * i / 60;
    gpath_rotate_to( gpath, angle );
    gpath_move_to( gpath, center_pt );
    graphics_context_set_stroke_color( pDTP->ctx, pDTP->tick_colour );
    graphics_context_set_stroke_width( pDTP->ctx, pDTP->tick_thk );
    gpath_draw_outline( pDTP->ctx, gpath );
  }
  graphics_context_set_fill_color( pDTP->ctx, pDTP->bg_colour );
  graphics_fill_rect( pDTP->ctx, grect_inset( bounds, GEdgeInsets( pDTP->tick_length ) ), 0, GCornerNone );
  gpath_destroy( gpath );
}

void draw_gpath_hands( GPATH_HANDS_PARAMS *pGP ) {
  GPath *gpath = gpath_create( pGP->gpath_hand );
  GPath *gpath_highlight = gpath_create( pGP->gpath_hand_highlight );
  
  graphics_context_set_antialiased( pGP->ctx, true );

  gpath_rotate_to( gpath, pGP->angle );
  gpath_move_to( gpath, pGP->center_pt );
  gpath_rotate_to( gpath_highlight, pGP->angle );
  gpath_move_to( gpath_highlight, pGP->center_pt );

  graphics_context_set_fill_color( pGP->ctx, pGP->hand_colour );
  gpath_draw_filled( pGP->ctx, gpath );
  
  graphics_context_set_fill_color( pGP->ctx, pGP->hand_highlight_colour );
  gpath_draw_filled( pGP->ctx, gpath_highlight );
  
  graphics_context_set_stroke_width( pGP->ctx, 1 );
  graphics_context_set_stroke_color( pGP->ctx, pGP->hand_outline_colour );
  gpath_draw_outline( pGP->ctx, gpath );
  
  graphics_context_set_fill_color( pGP->ctx, BACKGROUND_COLOUR );
  graphics_fill_circle( pGP->ctx, pGP->center_pt, 2 );
  
  gpath_destroy( gpath );
  gpath_destroy( gpath_highlight );
}

void draw_clock_hand( HAND_DRAW_PARAMS *pDP ) {
  graphics_context_set_antialiased( pDP->ctx, true );
  // dot outline
  graphics_context_set_stroke_color( pDP->ctx, pDP->dot_outline_colour );
  graphics_context_set_stroke_width( pDP->ctx, 1 );
  graphics_draw_circle( pDP->ctx, pDP->center_pt, pDP->dot_radius );
  // hand outline
  graphics_context_set_stroke_color( pDP->ctx, pDP->hand_outline_colour );
  graphics_context_set_stroke_width( pDP->ctx, pDP->hand_width + 2);
  graphics_draw_line( pDP->ctx, pDP->from_pt, pDP->to_pt );
  // hand
  graphics_context_set_stroke_color( pDP->ctx, pDP->hand_colour );
  graphics_context_set_stroke_width( pDP->ctx, pDP->hand_width );
  graphics_draw_line( pDP->ctx, pDP->from_pt, pDP->to_pt );
  // dot
  graphics_context_set_fill_color( pDP->ctx, pDP->dot_colour );
  graphics_fill_circle( pDP->ctx, pDP->center_pt, pDP->dot_radius - 1 );
  /*
  // dot outline
  graphics_context_set_stroke_color( pDP->ctx, GColorDarkGray );
  graphics_context_set_stroke_width( pDP->ctx, 1 );
  graphics_draw_circle( pDP->ctx, pDP->center_pt, pDP->dot_radius - 2 );
  */
}

