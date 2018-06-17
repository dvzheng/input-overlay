/**
 * Created by universal on 30.05.2018.
 * This file is part of reloded which is licensed
 * under the MOZILLA PUBLIC LICENSE 2.0 - mozilla.org/en-US/MPL/2.0/
 * github.com/univrsal/input-overlay
 */

#pragma once

#include <SDL.h>
#include "SDL_helper.hpp"

#define SIZE_LEFT 0
#define SIZE_RIGHT 1
#define SIZE_TOP 2
#define SIZE_BOTTOM 3
#define SIZE_MOVE 4
#define SIZE_NONE 5

class SDL_helper;

class CoordinateSystem
{
public:
	CoordinateSystem()
	{
		m_scale_f = 1;
		m_origin = { 0, 0 };
		m_origin_anchor = m_origin;
		m_dimensions = { 0, 0, 1280, 720 };
		m_system_area = { get_origin_left(), get_origin_top(),
			m_dimensions.w - m_origin_anchor.x, m_dimensions.h - m_origin_anchor.y };
	}

	CoordinateSystem(SDL_Point origin, SDL_Rect size, SDL_helper * h)
	{
		m_helper = h;
		m_origin.x = origin.x + size.x;
		m_origin.y = origin.y + size.y;
		m_origin_anchor = origin;
		m_dimensions = size;
		m_scale_f = 1.0;
		m_system_area = { get_origin_left(), get_origin_top(),
			m_dimensions.w - m_origin_anchor.x, m_dimensions.h - m_origin_anchor.y };
	}

	void enable_border(void) { m_border = true; }

	bool handle_events(SDL_Event * e);

	void draw_foreground(void);

	void draw_background(void);

	void set_dimensions(SDL_Rect r);

	void set_pos(int x, int y);

	/* Adjusts to pixel raster */
	int adjust(int i);

	/* X position of Origin anchor */
	int get_origin_left(void) { return m_dimensions.x + m_origin_anchor.x; }

	/* Y position of Origin anchor */
	int get_origin_top(void) { return m_dimensions.y + m_origin_anchor.y; }

	SDL_Point * get_origin(void) { return &m_origin; }

	int get_bottom(void) { return m_dimensions.y + m_dimensions.h; }
	int get_right(void) { return m_dimensions.x + m_dimensions.w; }

	int get_left(void) { return m_dimensions.x; }
	int get_top(void) { return m_dimensions.y; }

	int get_width(void) { return m_dimensions.w; }
	int get_height(void) { return m_dimensions.h; }

	/* Returns origin adjusted to viewport */
	int get_origin_x() { return m_origin.x - get_left() - m_origin_anchor.x; }
	/* Returns origin adjusted to viewport */
	int get_origin_y() { return m_origin.y - get_top() - m_origin_anchor.y; }

	double get_scale(void) { return m_scale_f; }
	SDL_Rect get_dimensions(void) { return m_dimensions; }
	SDL_helper * get_helper(void) { return m_helper; }
	const SDL_Rect * get_system_area(void) { return & m_system_area; }

	void set_selection(SDL_Rect * r) { m_selection = r; }

	/* Prepares the viewport to fit inside the coordinate system area */
	void begin_draw(void) { SDL_RenderSetViewport(m_helper->renderer(), &m_system_area); }
	/* Resets the viewport */
	void end_draw(void) { SDL_RenderSetViewport(m_helper->renderer(), NULL); }

private:
	void mouse_state(SDL_Event * event);

	inline bool in_range(int a, int b, int range)
	{
		return a <= b + range && a >= b - range;
	}

	inline bool in_between(int a, int min, int max, int o)
	{
		return a >= min * m_scale_f + o && a <= max * m_scale_f + o;
	}

	double m_scale_f; /* Multiplier for zooming */
	SDL_Rect m_dimensions; /* Complete size of the system */
	SDL_Rect m_system_area; /* Area minus axes and scale text */

	SDL_Rect * m_selection = nullptr; /* For atlas selector */
	SDL_Point m_selection_a;
	uint8_t m_size_mode = SIZE_NONE;

	bool m_selecting = false;
	bool m_sizing = false;

	SDL_Point m_origin_anchor; /* Constant position of the origin*/
	SDL_Point m_origin; /* Origin after zooming and moving */
	SDL_Point m_drag_offset;

	SDL_helper * m_helper = nullptr;

	bool m_dragging = false;
	bool m_border = false; /* Used inside dialogs */
};
