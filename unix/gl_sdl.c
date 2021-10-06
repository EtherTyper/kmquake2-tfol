#include <stdio.h>
#include "../renderer/r_local.h"

#include <SDL.h>
#include "glw_unix.h"

glwstate_t glw_state;

int mx, my;

// FIXME -flibit

int GLimp_Init(void *hinstance, void *wndproc)
{
	return 1;
}

void GLimp_Shutdown(void)
{
}

void GLimp_BeginFrame(float camera_seperation)
{
	SDL_ShowWindow(glw_state.glWindow);
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
	*pwidth = width;
	*pheight = height;
	VID_NewWindow(width, height);
	return rserr_ok;
}

void UpdateGammaRamp(void)
{
}

char *Sys_GetClipboardData(void)
{
	return SDL_GetClipboardText();
}

void IN_Activate(qboolean active)
{
}

void HandleEvents(void)
{
	SDL_Event evt;
	while (SDL_PollEvent(&evt) == 1)
	{
		if (evt.type == SDL_QUIT)
		{
			Cbuf_ExecuteText(EXEC_NOW, "quit");
		}
	}
}
