//
// Copyright (C) 2016, Vinodh Kumar M. <GreenHex@gmail.com>
//

#pragma once
#include <pebble.h>
#include "global.h"

#define DATE_WINDOW_SIZE_W 35
#define DATE_WINDOW_SIZE_H 26
#define DATE_WINDOW_POS_X ( PBL_DISPLAY_WIDTH - DATE_WINDOW_SIZE_W - DATE_LEFT_GAP )
#define DATE_WINDOW_POS_Y ( PBL_DISPLAY_HEIGHT/2 - DATE_WINDOW_SIZE_H/2 )
#define DATE_WINDOW_OUTLINE_THK 3
#define DATE_LEFT_GAP 3 /* make it same as CLOCK_TICK_EDGE_OFFSET */

#define DATE_TXT_VERT_ADJ 4

#define DATE_WINDOW_BOUNDS ( GRect( 0, 0, DATE_WINDOW_SIZE_W, DATE_WINDOW_SIZE_H ) )
#define DATE_WINDOW_FRAME ( GRect ( DATE_WINDOW_POS_X, DATE_WINDOW_POS_Y, DATE_WINDOW_SIZE_W, DATE_WINDOW_SIZE_H ) )

extern Layer *date_layer;

void date_init( Layer *parent_layer );
void date_deinit( void );
