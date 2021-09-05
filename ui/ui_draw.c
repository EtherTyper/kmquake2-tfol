/*
===========================================================================
Copyright (C) 1997-2001 Id Software, Inc.

This file is part of Quake 2 source code.

Quake 2 source code is free software; you can redistribute it
and/or modify it under the terms of the GNU General Public License as
published by the Free Software Foundation; either version 2 of the License,
or (at your option) any later version.

Quake 2 source code is distributed in the hope that it will be
useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Quake 2 source code; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
===========================================================================
*/

// ui_draw.c -- object drawing functions for the menus

#include <string.h>
#include <ctype.h>

#include "../client/client.h"
#include "ui_local.h"

/*
=======================================================================

	MISC DRAWING FUNCS

=======================================================================
*/

/*
==========================
UI_DrawMenuString
==========================
*/
void UI_DrawMenuString (int x, int y, int size, const char *string, int alpha, qboolean R2L, qboolean altColor)
{
	char	newstring[1024];

	if (altColor)
		Com_sprintf (newstring, sizeof(newstring), S_COLOR_ALT"%s", string);
	else
		Com_sprintf (newstring, sizeof(newstring), "%s", string);

	if (R2L)
		x -= stringLen(string)*size;
	UI_DrawString (x, y, size, newstring, alpha);
}


/*
=============
UI_DrawString
=============
*/
void UI_DrawString (int x, int y, int size, const char *string, int alpha)
{
	SCR_DrawString (x, y, size, ALIGN_CENTER, string, FONT_UI, alpha);
}


/*
=============
UI_DrawStringDark
=============
*/
void UI_DrawStringDark (int x, int y, int size, const char *string, int alpha)
{
	char	newstring[1024];

	Com_sprintf (newstring, sizeof(newstring), S_COLOR_ALT"%s", string);
	SCR_DrawString (x, y, size, ALIGN_CENTER, newstring, FONT_UI, alpha);
}


/*
=============
UI_DrawStringR2L
=============
*/
void UI_DrawStringR2L (int x, int y, int size, const char *string, int alpha)
{
	x -= stringLen(string)*size;	// MENU_FONT_SIZE
	SCR_DrawString (x, y, size, ALIGN_CENTER, string, FONT_UI, alpha);
}


/*
=============
UI_DrawStringR2LDark
=============
*/
void UI_DrawStringR2LDark (int x, int y, int size, const char *string, int alpha)
{
	char	newstring[1024];

	Com_sprintf (newstring, sizeof(newstring), S_COLOR_ALT"%s", string);
	x -= stringLen(string)*size;	// MENU_FONT_SIZE
	SCR_DrawString (x, y, size, ALIGN_CENTER, newstring, FONT_UI, alpha);
}


/*
=============
UI_DrawMenuStatusBar
=============
*/
void UI_DrawMenuStatusBar (const char *string)
{
	if (string)
	{
		int l = (int)strlen( string );

		UI_DrawFill (0, SCREEN_HEIGHT-(MENU_FONT_SIZE+3), SCREEN_WIDTH, MENU_FONT_SIZE+4, ALIGN_BOTTOM_STRETCH, false, 60,60,60,255 );	// go 1 pixel past screen bottom to prevent gap from scaling
		UI_DrawFill (0, SCREEN_HEIGHT-(MENU_FONT_SIZE+3), SCREEN_WIDTH, 1, ALIGN_BOTTOM_STRETCH, false, 0,0,0,255 );
		SCR_DrawString (SCREEN_WIDTH/2-(l/2)*MENU_FONT_SIZE, SCREEN_HEIGHT-(MENU_FONT_SIZE+1), MENU_FONT_SIZE, ALIGN_BOTTOM, string, FONT_UI, 255 );
	}
	else
		UI_DrawFill (0, SCREEN_HEIGHT-(MENU_FONT_SIZE+3), SCREEN_WIDTH, MENU_FONT_SIZE+4, ALIGN_BOTTOM_STRETCH, false, 0,0,0,255 );	// go 1 pixel past screen bottom to prevent gap from scaling
}


/*
=============
UI_DrawTextBox
=============
*/
void UI_DrawTextBox (int x, int y, int width, int lines)
{
	int		cx, cy;
	int		n;
	vec4_t	stCoord_textBox[9];

	Vector4Set (stCoord_textBox[0], 0.0, 0.0, 0.25, 0.25);
	Vector4Set (stCoord_textBox[1], 0.0, 0.25, 0.25, 0.5);
	Vector4Set (stCoord_textBox[2], 0.0, 0.5, 0.25, 0.75);
	Vector4Set (stCoord_textBox[3], 0.25, 0.0, 0.5, 0.25);
	Vector4Set (stCoord_textBox[4], 0.25, 0.25, 0.5, 0.5);
	Vector4Set (stCoord_textBox[5], 0.25, 0.5, 0.5, 0.75);
	Vector4Set (stCoord_textBox[6], 0.5, 0.0, 0.75, 0.25);
	Vector4Set (stCoord_textBox[7], 0.5, 0.25, 0.75, 0.5);
	Vector4Set (stCoord_textBox[8], 0.5, 0.5, 0.75, 0.75);

	// draw left side
	cx = x;
	cy = y;
	if (ui_new_textbox->integer)
		UI_DrawPicST (cx, cy, MENU_FONT_SIZE, MENU_FONT_SIZE, stCoord_textBox[0], ALIGN_CENTER, true, color_identity, UI_TEXTBOX_PIC);
	else
		UI_DrawChar (cx, cy, MENU_FONT_SIZE, ALIGN_CENTER, 1, 255, 255, 255, 255, false, false);
	for (n = 0; n < lines; n++)
	{
		cy += MENU_FONT_SIZE;
		if (ui_new_textbox->integer)
			UI_DrawPicST (cx, cy, MENU_FONT_SIZE, MENU_FONT_SIZE, stCoord_textBox[1], ALIGN_CENTER, true, color_identity, UI_TEXTBOX_PIC);
		else
			UI_DrawChar (cx, cy, MENU_FONT_SIZE, ALIGN_CENTER, 4, 255, 255, 255, 255, false, false);
	}
	if (ui_new_textbox->integer)
		UI_DrawPicST (cx, cy+MENU_FONT_SIZE, MENU_FONT_SIZE, MENU_FONT_SIZE, stCoord_textBox[2], ALIGN_CENTER, true, color_identity, UI_TEXTBOX_PIC);
	else
		UI_DrawChar (cx, cy+MENU_FONT_SIZE, MENU_FONT_SIZE, ALIGN_CENTER, 7, 255, 255, 255, 255, false, false);

	// draw middle
	cx += MENU_FONT_SIZE;
	while (width > 0)
	{
		cy = y;
		if (ui_new_textbox->integer)
			UI_DrawPicST (cx, cy, MENU_FONT_SIZE, MENU_FONT_SIZE, stCoord_textBox[3], ALIGN_CENTER, true, color_identity, UI_TEXTBOX_PIC);
		else
			UI_DrawChar (cx, cy, MENU_FONT_SIZE, ALIGN_CENTER, 2, 255, 255, 255, 255, false, false);
		for (n = 0; n < lines; n++)
		{
			cy += MENU_FONT_SIZE;
			if (ui_new_textbox->integer)
				UI_DrawPicST (cx, cy, MENU_FONT_SIZE, MENU_FONT_SIZE, stCoord_textBox[4], ALIGN_CENTER, true, color_identity, UI_TEXTBOX_PIC);
			else
				UI_DrawChar (cx, cy, MENU_FONT_SIZE, ALIGN_CENTER, 5, 255, 255, 255, 255, false, false);
		}
		if (ui_new_textbox->integer)
			UI_DrawPicST (cx, cy+MENU_FONT_SIZE, MENU_FONT_SIZE, MENU_FONT_SIZE, stCoord_textBox[5], ALIGN_CENTER, true, color_identity, UI_TEXTBOX_PIC);
		else
			UI_DrawChar (cx, cy+MENU_FONT_SIZE, MENU_FONT_SIZE, ALIGN_CENTER, 8, 255, 255, 255, 255, false, false);
		width -= 1;
		cx += MENU_FONT_SIZE;
	}

	// draw right side
	cy = y;
	if (ui_new_textbox->integer)
		UI_DrawPicST (cx, cy, MENU_FONT_SIZE, MENU_FONT_SIZE, stCoord_textBox[6], ALIGN_CENTER, true, color_identity, UI_TEXTBOX_PIC);
	else
		UI_DrawChar (cx, cy, MENU_FONT_SIZE, ALIGN_CENTER, 3, 255, 255, 255, 255, false, false);
	for (n = 0; n < lines; n++)
	{
		cy += MENU_FONT_SIZE;
		if (ui_new_textbox->integer)
			UI_DrawPicST (cx, cy, MENU_FONT_SIZE, MENU_FONT_SIZE, stCoord_textBox[7], ALIGN_CENTER, true, color_identity, UI_TEXTBOX_PIC);
		else
			UI_DrawChar (cx, cy, MENU_FONT_SIZE, ALIGN_CENTER, 6, 255, 255, 255, 255, false, false);
	}
	if (ui_new_textbox->integer)
		UI_DrawPicST (cx, cy+MENU_FONT_SIZE, MENU_FONT_SIZE, MENU_FONT_SIZE, stCoord_textBox[8], ALIGN_CENTER, true, color_identity, UI_TEXTBOX_PIC);
	else
		UI_DrawChar (cx, cy+MENU_FONT_SIZE, MENU_FONT_SIZE, ALIGN_CENTER, 9, 255, 255, 255, 255, false, true);
}


/*
=================
UI_DrawBanner
=================
*/
void UI_DrawBanner (char *name)
{
	int		w, h;

	R_DrawGetPicSize (&w, &h, name );
	UI_DrawPic (SCREEN_WIDTH/2 - w/2, SCREEN_HEIGHT/2 - 150, w, h, ALIGN_CENTER, false, name, 1.0);
}


/*
=================
UI_Draw_Cursor
=================
*/
#if 1
void UI_Draw_Cursor (void)
{
//	int		w, h;
//	float	ofs_x, ofs_y;
	float	scale = SCR_ScaledScreen(ui_cursor_scale->value); // 0.4
	char	*cur_img = UI_MOUSECURSOR_PIC;

	// get sizing vars
/*	R_DrawGetPicSize (&w, &h, UI_MOUSECURSOR_PIC);
	ofs_x = SCR_ScaledScreen(w) * ui_cursor_scale->value * 0.5;
	ofs_y = SCR_ScaledScreen(h) * ui_cursor_scale->value * 0.5;
	
	R_DrawScaledPic (ui_mousecursor.x - ofs_x, ui_mousecursor.y - ofs_y, scale, 1.0f, cur_img);
*/
	SCR_DrawScaledPic (ui_mousecursor.x, ui_mousecursor.y, scale, true, false, cur_img, 1.0f);
}
#else
void UI_Draw_Cursor (void)
{
	float	alpha = 1, scale = SCR_ScaledScreen(0.66);
	int		w, h;
	char	*overlay = NULL;
	char	*cur_img = NULL;

	if (m_drawfunc == M_Main_Draw)
	{
		if (MainMenuMouseHover)
		{
			if ((cursor.buttonused[0] && cursor.buttonclicks[0])
				|| (cursor.buttonused[1] && cursor.buttonclicks[1]))
			{
				cur_img = "/gfx/ui/cursors/m_cur_click.pcx";
				alpha = 0.85 + 0.15*sin(anglemod(cl.time*0.005));
			}
			else
			{
				cur_img = "/gfx/ui/cursors/m_cur_hover.pcx";
				alpha = 0.85 + 0.15*sin(anglemod(cl.time*0.005));
			}
		}
		else
			cur_img = "/gfx/ui/cursors/m_cur_main.pcx";
		overlay = "/gfx/ui/cursors/m_cur_over.pcx";
	}
	else
	{
		if (cursor.menuitem)
		{
			if (cursor.menuitemtype == MENUITEM_TEXT)
			{
				cur_img = "/gfx/ui/cursors/m_cur_text.pcx";
			}
			else
			{
				if ((cursor.buttonused[0] && cursor.buttonclicks[0])
					|| (cursor.buttonused[1] && cursor.buttonclicks[1]))
				{
					cur_img = "/gfx/ui/cursors/m_cur_click.pcx";
					alpha = 0.85 + 0.15*sin(anglemod(cl.time*0.005));
				}
				else
				{
					cur_img = "/gfx/ui/cursors/m_cur_hover.pcx";
					alpha = 0.85 + 0.15*sin(anglemod(cl.time*0.005));
				}
				overlay = "/gfx/ui/cursors/m_cur_over.pcx";
			}
		}
		else
		{
			cur_img = "/gfx/ui/cursors/m_cur_main.pcx";
			overlay = "/gfx/ui/cursors/m_cur_over.pcx";
		}
	}
	
	if (cur_img)
	{
		R_DrawGetPicSize( &w, &h, cur_img );
		R_DrawScaledPic( cursor.x - scale*w/2, cursor.y - scale*h/2, scale, alpha, cur_img);

		if (overlay) {
			R_DrawGetPicSize( &w, &h, overlay );
			R_DrawScaledPic( cursor.x - scale*w/2, cursor.y - scale*h/2, scale, 1, overlay);
		}
	}
}
#endif

/*void UI_Draw_Cursor (void)
{
	int w,h;

	//get sizing vars
	R_DrawGetPicSize( &w, &h, "m_mouse_cursor" );
	w = SCR_ScaledScreen(w)*0.5;
	h = SCR_ScaledScreen(h)*0.5;
	R_DrawStretchPic (cursor.x-w/2, cursor.y-h/2, w, h, "m_mouse_cursor", 1.0);
}*/
