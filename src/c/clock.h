//
// Copyright (C) 2016, Vinodh Kumar M. <GreenHex@gmail.com>
//

#pragma once
#include <pebble.h>
#include "global.h"

#define CLOCK_DIAL_SIZE_W PBL_DISPLAY_WIDTH 
#define CLOCK_DIAL_SIZE_H PBL_DISPLAY_HEIGHT
#define CLOCK_DIAL_POS_X 0
#define CLOCK_DIAL_POS_Y 0

#define CLOCK_DIAL_RECT ( GRect( CLOCK_DIAL_POS_X, CLOCK_DIAL_POS_Y, CLOCK_DIAL_SIZE_W, CLOCK_DIAL_SIZE_H ) )
#define CLOCK_TICK_EDGE_OFFSET 3 /* make it an odd number */

#define SNOOZE_LAYER_FRAME ( GRect( PBL_DISPLAY_WIDTH / 2 - 20 / 2, 36, 20, 20 ) )

// #define USE_GPATH_SECONDS_HAND

static GPathInfo PATH_TICK = {
  2, (GPoint []) {
    { 0, - ( CLOCK_DIAL_SIZE_W > CLOCK_DIAL_SIZE_H ? CLOCK_DIAL_SIZE_W : CLOCK_DIAL_SIZE_H ) },
    { 0, ( CLOCK_DIAL_SIZE_W >  CLOCK_DIAL_SIZE_H ? CLOCK_DIAL_SIZE_W : CLOCK_DIAL_SIZE_H ) }
  }
};

#if PBL_DISPLAY_WIDTH == 200

static GPathInfo HOUR_HAND_SBGE001_POINTS = {
  5, (GPoint []) {
    { -3, 20 },
    { -11, 0 },
    { 0, -48 },
    { 11, 0 },
    { 3, 20 }
  }
};

static GPathInfo HOUR_HAND_SBGE001_POINTS_HIGHLIGHT = {
  4, (GPoint []) {
    { 0, 20 },
    { 3, 20 },
    { 11, 0 },
    { 0, -48 },
  }
};

static GPathInfo MINUTE_HAND_SBGE001_POINTS = {
  5, (GPoint []) {
    { -3, 20 },
    { -9, 0 },
    { 0, -82 },
    { 9, 0 },
    { 3, 20 }
  }
};

static GPathInfo MINUTE_HAND_SBGE001_POINTS_HIGHLIGHT = {
  4, (GPoint []) {
    { 0, 20 },
    { -3, 20 },
    { -9, 0 },
    { 0, -82 },
  }
};

static GPathInfo SECOND_HAND_POINTS = {
  2, (GPoint []) {
    { 0, 28 },
    { 0, -74 }
  }
};

#else

static GPathInfo HOUR_HAND_SBGE001_POINTS = {
  5, (GPoint []) {
    { -3, 14 },
    { -9, 0 },
    { 0, -42 },
    { 9, 0 },
    { 3, 14 }
  }
};

static GPathInfo HOUR_HAND_SBGE001_POINTS_HIGHLIGHT = {
  2, (GPoint []) {
    { 0, 8 },
    { 0, -22 },
  }
};

static GPathInfo HOUR_HAND_SBGE001_POINTS_HIGHLIGHT_2 = {
  4, (GPoint []) {
    { 0, 14 },
    { 3, 14 },
    { 9, 0 },
    { 0, -42 },
  }
};

static GPathInfo MINUTE_HAND_SBGE001_POINTS = {
  5, (GPoint []) {
    { -3, 14 },
    { -7, 0 },
    { 0, -64 },
    { 7, 0 },
    { 3, 14 }
  }
};

static GPathInfo MINUTE_HAND_SBGE001_POINTS_HIGHLIGHT = {
  2, (GPoint []) {
    { 0, 8 },
    { 0, -28 },
  }
};

static GPathInfo MINUTE_HAND_SBGE001_POINTS_HIGHLIGHT_2 = {
  4, (GPoint []) {
    { 0, 14 },
    { -3, 14 },
    { -7, 0 },
    { 0, -64 },
  }
};

static GPathInfo SECOND_HAND_POINTS = {
  2, (GPoint []) {
    { 0, 20 },
    { 0, -61 }
  }
};

#endif

#if PBL_DISPLAY_WIDTH == 200

#define SEC_HAND_LENGTH 74
#define SEC_HAND_TAIL_LENGTH 28
#define SEC_HAND_TIP_LENGTH 16
#define SEC_HAND_WIDTH 1
#define SEC_CENTER_DOT_RADIUS 5

#else

#define SEC_HAND_LENGTH 61 /* 55  53 */
#define SEC_HAND_TAIL_LENGTH 23
#define SEC_HAND_TIP_LENGTH 12
#define SEC_HAND_WIDTH 1
#define SEC_CENTER_DOT_RADIUS 4

#endif

extern tm tm_time;

void clock_init( Window* window );
void clock_deinit( void );
