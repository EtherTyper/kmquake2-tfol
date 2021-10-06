#include <stdio.h>
#include "../renderer/r_local.h"
#include "../client/keys.h"
#include "../ui/ui_local.h"

#include <SDL.h>
#include "glw_unix.h"

glwstate_t glw_state;

int mx, my;
qboolean mouse_active = false;

int GLimp_Init(void *hinstance, void *wndproc)
{
	/* No-op */
	return 1;
}

void GLimp_Shutdown(void)
{
	SDL_SetRelativeMouseMode(SDL_FALSE);
	mouse_active = false;
}

void GLimp_BeginFrame(float camera_seperation)
{
	/* No-op */
}

void GLimp_EndFrame(void)
{
	SDL_GL_SwapWindow(glw_state.glWindow);
}

int GLimp_SetMode(int *pwidth, int *pheight, int mode, dispType_t fullscreen)
{
	int width, height;
	if (!VID_GetModeInfo(&width, &height, mode))
	{
		Com_Printf(" invalid mode\n");
		return rserr_invalid_mode;
	}

	SDL_SetWindowSize(glw_state.glWindow, width, height);
	/* TODO: mode, faux-backbuffer */

	*pwidth = width;
	*pheight = height;
	SDL_ShowWindow(glw_state.glWindow);
	VID_NewWindow(width, height);
	return rserr_ok;
}

void UpdateGammaRamp(void)
{
	/* Unsupported in 2021 */
}

char *Sys_GetClipboardData(void)
{
	return SDL_GetClipboardText();
}

void IN_Activate(qboolean active)
{
	if (active)
	{
		if (!mouse_active)
		{
			mx = my = 0;
			SDL_SetRelativeMouseMode(SDL_TRUE);
			mouse_active = true;
		}
	}
	else
	{
		if (mouse_active)
		{
			SDL_SetRelativeMouseMode(SDL_FALSE);
			mouse_active = false;
		}
	}
}

static int SDLToQuake(SDL_Keycode key)
{
	#define KEY(sdl, quake) \
		case SDLK_##sdl: return K_##quake;
	switch(key)
	{
	KEY(SPACE, SPACE)
	KEY(PAGEUP, PGUP)
	KEY(PAGEDOWN, PGDN)
	KEY(HOME, HOME)
	KEY(END, END)
	KEY(LEFT, LEFTARROW)
	KEY(RIGHT, RIGHTARROW)
	KEY(DOWN, DOWNARROW)
	KEY(UP, UPARROW)
	KEY(ESCAPE, ESCAPE)
	KEY(RETURN, ENTER)
	KEY(TAB, TAB)
	KEY(F1, F1)
	KEY(F2, F2)
	KEY(F3, F3)
	KEY(F4, F4)
	KEY(F5, F5)
	KEY(F6, F6)
	KEY(F7, F7)
	KEY(F8, F8)
	KEY(F9, F9)
	KEY(F10, F10)
	KEY(F11, F11)
	KEY(F12, F12)
	KEY(BACKSPACE, BACKSPACE)
	KEY(DELETE, DEL)
	KEY(PAUSE, PAUSE)
	KEY(LSHIFT, SHIFT)
	KEY(RSHIFT, SHIFT)
	KEY(LCTRL, CTRL)
	KEY(RCTRL, CTRL)
	KEY(LALT, ALT)
	KEY(RALT, ALT)
	KEY(INSERT, INS)
	KEY(KP_MULTIPLY, KP_MULT)
	KEY(KP_PLUS, KP_PLUS)
	KEY(KP_MINUS, KP_MINUS)
	KEY(KP_DIVIDE, KP_SLASH)
	default:
		if (key >= SDLK_a && key <= SDLK_z)
		{
			if (SDL_GetModState() & KMOD_SHIFT)
			{
				return 'A' + (key - SDLK_a);
			}
		}
		return key;
	};
	#undef KEY
}

void HandleEvents(void)
{
	int multiclicktime = 750;
	SDL_Event evt;
	while (SDL_PollEvent(&evt) == 1)
	{
		if (evt.type == SDL_QUIT)
		{
			Cbuf_ExecuteText(EXEC_NOW, "quit");
		}
		else if (evt.type == SDL_KEYDOWN || evt.type == SDL_KEYUP)
		{
			Key_Event(
				SDLToQuake(evt.key.keysym.sym),
				evt.type == SDL_KEYDOWN,
				Sys_Milliseconds()
			);
		}
		else if (evt.type == SDL_MOUSEMOTION)
		{
			if (mouse_active)
			{
				/* Relative should be on here */
				mx += evt.motion.xrel;
				my += evt.motion.yrel;
			}
		}
		else if (evt.type == SDL_MOUSEBUTTONDOWN)
		{
			int mouse_button = evt.button.button - 1;
			if (mouse_button == 1)
			{
				mouse_button = 2;
			}
			else if (mouse_button == 2)
			{
				mouse_button = 1;
			}
			if (Sys_Milliseconds() - ui_mousecursor.buttontime[mouse_button] < multiclicktime)
			{
				ui_mousecursor.buttonclicks[mouse_button] += 1;
			}
			else
			{
				ui_mousecursor.buttonclicks[mouse_button] = 1;
			}

			if (ui_mousecursor.buttonclicks[mouse_button] > 3)
			{
				ui_mousecursor.buttonclicks[mouse_button] = 3;
			}

			ui_mousecursor.buttontime[mouse_button] = Sys_Milliseconds();

			ui_mousecursor.buttondown[mouse_button] = true;
			ui_mousecursor.buttonused[mouse_button] = false;
			ui_mousecursor.mouseaction = true;

			Key_Event(K_MOUSE1 + mouse_button, 1, Sys_Milliseconds());
		}
		else if (evt.type == SDL_MOUSEBUTTONUP)
		{
			int mouse_button = evt.button.button;
			if (mouse_button == 1)
			{
				mouse_button = 2;
			}
			else if (mouse_button == 2)
			{
				mouse_button = 1;
			}

			ui_mousecursor.buttondown[mouse_button] = false;
			ui_mousecursor.buttonused[mouse_button] = false;
			ui_mousecursor.mouseaction = true;

			Key_Event(K_MOUSE1 + mouse_button, 0, Sys_Milliseconds());
		}
		else if (evt.type == SDL_MOUSEWHEEL)
		{
			/* TODO -flibit */
		}
	}
}
