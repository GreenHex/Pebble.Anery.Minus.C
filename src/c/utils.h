//
// Copyright (C) 2016, Vinodh Kumar M. <GreenHex@gmail.com>
//

#pragma once

typedef struct {
  Layer *layer;
  GContext *ctx;
  GPathInfo *p_gpath_info;
  int increment;
  int tick_thk;
  int tick_length;
  GColor tick_colour;
  GColor bg_colour;
} DRAW_TICKS_PARAMS;

typedef struct {
  GContext *ctx;
  GPoint center_pt;
  uint32_t angle;
  GPathInfo *gpath_hand;
  GPathInfo *gpath_hand_highlight;
  GColor hand_colour;
  GColor hand_highlight_colour;
  GColor hand_outline_colour;
} GPATH_HANDS_PARAMS;

typedef struct {
  GContext *ctx;
  GPoint center_pt;
  GPoint from_pt;
  GPoint to_pt;
  uint8_t hand_width;
  GColor hand_colour;
  GColor hand_outline_colour;
  uint16_t dot_radius;
  GColor dot_colour;
  GColor dot_outline_colour;
} HAND_DRAW_PARAMS;

bool is_X_in_range( int a, int b, int x );
void log_pt( char *str, GPoint pt );
void log_rect( char *str, GRect rect );
void draw_seconds_ticks( DRAW_TICKS_PARAMS *pDTP );
void draw_gpath_hands( GPATH_HANDS_PARAMS *pGP );
void draw_clock_hand( HAND_DRAW_PARAMS *pDP );
