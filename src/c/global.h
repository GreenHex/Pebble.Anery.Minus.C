//
// Copyright (C) 2016, Vinodh Kumar M. <GreenHex@gmail.com>
//

#pragma once
#include <pebble.h>

// #define DEBUG

// #define REVERSE
#ifdef REVERSE

#define BACKGROUND_COLOUR GColorWhite
#define TICKS_COLOUR GColorBlack
#define HAND_COLOUR GColorBlack
#define HAND_HIGHLIGHT_COLOUR GColorDarkGray
#define HAND_OUTLINE_COLOUR GColorLightGray
/*
#define HAND_COLOUR GColorCobaltBlue
#define HAND_HIGHLIGHT_COLOUR GColorPictonBlue
#define HAND_OUTLINE_COLOUR GColorBlack
*/
#define BATTERY_GAUGE_POINTER_FILL_COLOUR PBL_IF_COLOR_ELSE( GColorLightGray, GColorBlack )
#define BATTERY_GAUGE_POINTER_OUTLINE_COLOUR BACKGROUND_COLOUR

#define COLOUR_BG_BITMAP_BG     BACKGROUND_COLOUR
#define COLOUR_DOT              TICKS_COLOUR
#define COLOUR_DOT_OUTLINE      BACKGROUND_COLOUR
#define COLOUR_SEC_HAND         TICKS_COLOUR /* GColorWhite */
#define COLOUR_SEC_HAND_TIP     PBL_IF_COLOR_ELSE( GColorRed /* GColorDarkCandyAppleRed */, GColorWhite )

#else

#define BACKGROUND_COLOUR PBL_IF_COLOR_ELSE( GColorBlack, GColorBlack )
#define TICKS_COLOUR PBL_IF_COLOR_ELSE( GColorWhite, GColorWhite )
#define HAND_COLOUR GColorLightGray
#define HAND_HIGHLIGHT_COLOUR GColorWhite
#define HAND_OUTLINE_COLOUR BACKGROUND_COLOUR
/*
#define HAND_COLOUR GColorCobaltBlue
#define HAND_HIGHLIGHT_COLOUR GColorPictonBlue
#define HAND_OUTLINE_COLOUR GColorBlack
*/
#define BATTERY_GAUGE_POINTER_FILL_COLOUR PBL_IF_COLOR_ELSE( GColorLightGray, GColorWhite )
#define BATTERY_GAUGE_POINTER_OUTLINE_COLOUR PBL_IF_COLOR_ELSE( GColorBlack, GColorBlack )

#define COLOUR_BG_BITMAP_BG     BACKGROUND_COLOUR
#define COLOUR_DOT              TICKS_COLOUR
#define COLOUR_DOT_OUTLINE      BACKGROUND_COLOUR
#define COLOUR_SEC_HAND         TICKS_COLOUR /* GColorWhite */
#define COLOUR_SEC_HAND_TIP     PBL_IF_COLOR_ELSE( GColorRed /* GColorDarkCandyAppleRed */, GColorWhite )

#endif

#define SHOW_SECONDS_TIMER_TIMEOUT_MS ( 5 * 1000 )

extern tm tm_time;
