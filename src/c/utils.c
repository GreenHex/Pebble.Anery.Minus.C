//
// Copyright (C) 2016, Vinodh Kumar M. <GreenHex@gmail.com>
//

#include <pebble.h>
#include "utils.h"
#include "global.h"

#if defined( PBL_COLOR )
#define NUM_PBL_64_COLOURS 64

const uint32_t PBL_64_COLOURS[ NUM_PBL_64_COLOURS ] = {
  0x000000, 0xFFFFFF, 0xAAAAAA, 0x005555, 0xFFFFAA, 0xFFFF55, 0xFFAA55, 0xFF5500,
  0xFF0000, 0xFF0055, 0xFF5555, 0xFFAAAA, 0xFFFF00, 0xFFAA00, 0xAA5500, 0xAA5555,
  0xAA0000, 0xFF00AA, 0xFF55AA, 0xFFAAFF, 0x550000, 0xAA0055, 0xFF00FF, 0xFF55FF,
  0x550055, 0xAA00AA, 0xAA55AA, 0x000055, 0x5500AA, 0xAA00FF, 0xAA55FF, 0x0000AA,
  0x5500FF, 0x5555AA, 0x0055AA, 0x55AAAA, 0x55AA55, 0x00AA00, 0x00FF00, 0x55FF00,
  0xAAFF55, 0xAAFF00, 0x55AA00, 0x005500, 0x005555, 0xAAAA55, 0x555500, 0xAAAA00,
  0xAAFFAA, 0x55FF55, 0x00FF55, 0x00AA55, 0x00AAAA, 0x00AAFF, 0x0000FF, 0x5555FF,
  0xAAAAFF, 0x55FFAA, 0x00FFAA, 0x00FFFF, 0x55AAFF, 0x0055FF, 0x55FFFF, 0xAAFFFF
};
#endif /* PBL_COLOR */


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
  if ( ! pDTP->p_gpath_info ) return;
  
  GRect bounds = layer_get_bounds( pDTP->layer );
  GPoint center_pt = grect_center_point( &bounds );
  GPath *gpath = gpath_create( pDTP->p_gpath_info );
  graphics_context_set_antialiased( pDTP->ctx, true );
  
  for ( int i = 0, angle = 0; i < 30; i += pDTP->increment ) { // drawing half the ticks is *probably* more efficient
    angle = TRIG_MAX_ANGLE * i / 60;
    gpath_rotate_to( gpath, angle );
    gpath_move_to( gpath, center_pt );
    graphics_context_set_stroke_color( pDTP->ctx, pDTP->tick_colour );
    // graphics_context_set_stroke_color( pDTP->ctx, PBL_IF_COLOR_ELSE( GColorFromHEX( PBL_64_COLOURS[ rand() % ( NUM_PBL_64_COLOURS - 3 ) + 3 ] ), pDTP->tick_colour ) );
    graphics_context_set_stroke_width( pDTP->ctx, pDTP->tick_thk );
    gpath_draw_outline( pDTP->ctx, gpath );
  }
  graphics_context_set_fill_color( pDTP->ctx, pDTP->bg_colour );
  graphics_fill_rect( pDTP->ctx, grect_inset( bounds, GEdgeInsets( pDTP->tick_length ) ), 0, GCornerNone );
  gpath_destroy( gpath );
}

void draw_gpath_hands( GPATH_HANDS_PARAMS *pGP ) {
  if ( ! pGP->gpath_hand ) return;
  
  graphics_context_set_antialiased( pGP->ctx, true );

  GPath *gpath = gpath_create( pGP->gpath_hand );
  gpath_rotate_to( gpath, pGP->angle );
  gpath_move_to( gpath, pGP->center_pt );
  graphics_context_set_fill_color( pGP->ctx, pGP->hand_colour );
  gpath_draw_filled( pGP->ctx, gpath );
  
  if ( pGP->gpath_hand_highlight ) {
    GPath *gpath_highlight = gpath_create( pGP->gpath_hand_highlight );
    gpath_rotate_to( gpath_highlight, pGP->angle );
    gpath_move_to( gpath_highlight, pGP->center_pt );
    graphics_context_set_fill_color( pGP->ctx, pGP->hand_highlight_colour );
    gpath_draw_filled( pGP->ctx, gpath_highlight );
    graphics_context_set_stroke_width( pGP->ctx, 1 );
    graphics_context_set_stroke_color( pGP->ctx, pGP->hand_highlight_colour );
    gpath_draw_outline( pGP->ctx, gpath_highlight );
    gpath_destroy( gpath_highlight );
  }
  
  graphics_context_set_stroke_width( pGP->ctx, 1 );
  graphics_context_set_stroke_color( pGP->ctx, pGP->hand_outline_colour );
  gpath_draw_outline( pGP->ctx, gpath );
  
  graphics_context_set_fill_color( pGP->ctx, pGP->center_dot_colour );
  graphics_fill_circle( pGP->ctx, pGP->center_pt, pGP->center_dot_radius );
  
  gpath_destroy( gpath );
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

