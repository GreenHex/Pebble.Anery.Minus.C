//
// Copyright (C) 2016, Vinodh Kumar M. <GreenHex@gmail.com>
//

#pragma once
#include <pebble.h>
#include "global.h"

#ifdef REVERSE
#define OUTER_WINDOW_OUTLINE_THK 3
#else
#define OUTER_WINDOW_OUTLINE_THK 3
#endif

#if PBL_DISPLAY_WIDTH == 200
#define DAY_WINDOW_SIZE_W 28
#else
#define DAY_WINDOW_SIZE_W 26
#endif
#define DAY_WINDOW_SIZE_H 16
#define DAY_WINDOW_FRAME ( GRect( OUTER_WINDOW_OUTLINE_THK, OUTER_WINDOW_OUTLINE_THK, DAY_WINDOW_SIZE_W, DAY_WINDOW_SIZE_H ) )
#define DAY_TXT_VERT_ADJ 2

#define GAP_SIZE_W 1

#define DATE_WINDOW_SIZE_W 26
#define DATE_WINDOW_SIZE_H DAY_WINDOW_SIZE_H
#define DATE_WINDOW_FRAME ( GRect( OUTER_WINDOW_OUTLINE_THK + DAY_WINDOW_SIZE_W + GAP_SIZE_W, OUTER_WINDOW_OUTLINE_THK, DATE_WINDOW_SIZE_W, DATE_WINDOW_SIZE_H ) )
#define DATE_TXT_VERT_ADJ 5

#define OUTER_WINDOW_SIZE_W ( DATE_WINDOW_SIZE_W + GAP_SIZE_W + DAY_WINDOW_SIZE_W + ( 2 * OUTER_WINDOW_OUTLINE_THK ) )
#define OUTER_WINDOW_SIZE_H ( DAY_WINDOW_SIZE_H + ( 2 * OUTER_WINDOW_OUTLINE_THK ) )

#if PBL_DISPLAY_WIDTH == 200
#define OUTER_WINDOW_LEFT_GAP 3 /* make it same as CLOCK_TICK_EDGE_OFFSET */
#else
#define OUTER_WINDOW_LEFT_GAP 1 /* make it same as CLOCK_TICK_EDGE_OFFSET */
#endif
#define OUTER_WINDOW_POS_X ( PBL_DISPLAY_WIDTH - OUTER_WINDOW_SIZE_W - OUTER_WINDOW_LEFT_GAP )
#define OUTER_WINDOW_POS_Y ( PBL_DISPLAY_HEIGHT/2 - OUTER_WINDOW_SIZE_H/2 )
#define OUTER_WINDOW_BOUNDS ( GRect( 0, 0, OUTER_WINDOW_SIZE_W, OUTER_WINDOW_SIZE_H ) )
#define OUTER_WINDOW_FRAME ( GRect ( OUTER_WINDOW_POS_X, OUTER_WINDOW_POS_Y, OUTER_WINDOW_SIZE_W, OUTER_WINDOW_SIZE_H ) )

extern Layer *outer_window_layer;

void date_init( Layer *parent_layer );
void date_deinit( void );
