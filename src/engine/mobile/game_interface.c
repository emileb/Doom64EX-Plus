#include "doomdef.h"
#include "doomtype.h"
#include "doomstat.h"
#include "d_main.h"
#include "d_ticcmd.h"
#include "m_fixed.h"
#include "g_controls.h"
#include <pthread.h>

#include "SDL.h"
#include "SDL_keycode.h"

#include "game_interface.h"

// FIFO STUFF ////////////////////

#define EVENTQUEUELENGTH 128

struct eventlist_s
{
	int scancode, unicode,state;
} eventlist[EVENTQUEUELENGTH];

volatile int events_avail = 0; /*volatile to make sure the cc doesn't try leaving these cached in a register*/
volatile int events_used = 0;

static struct eventlist_s *in_newevent(void)
{
	if (events_avail >= events_used + EVENTQUEUELENGTH)
		return NULL;
	return &eventlist[events_avail & (EVENTQUEUELENGTH-1)];
}

static void in_finishevent(void)
{
	events_avail++;
}
///////////////////////

struct eventlistcmd_s
{
	char cmd[256];
} eventlistcmd[EVENTQUEUELENGTH];

volatile int events_cmd_avail = 0; /*volatile to make sure the cc doesn't try leaving these cached in a register*/
volatile int events_cmd_used = 0;

static struct eventlistcmd_s *in_newevent_cmd(void)
{
	if (events_cmd_avail >= events_cmd_used + EVENTQUEUELENGTH)
		return NULL;
	return &eventlistcmd[events_cmd_avail & (EVENTQUEUELENGTH-1)];
}

static void in_finishevent_cmd(void)
{
	events_cmd_avail++;
}

///////////////////////

extern int SDL_SendKeyboardKey(Uint8 state, SDL_Scancode scancode);

int PortableKeyEvent(int state, int code,int unicode)
{

	//LOGI("PortableKeyEvent %d %d",state,code);
	struct eventlist_s *ev = in_newevent();
	if (!ev)
		return 0;

	ev->scancode = code;
	ev->unicode = code;
	ev->state = state;
	in_finishevent();
	return 0;

}

void PortableBackButton()
{
    PortableKeyEvent(1, KEY_ESCAPE,0);
    PortableKeyEvent(0, KEY_ESCAPE,0);
}


void D_PostEvent(event_t* ev);

void Android_SendKeys( void )
{
	struct eventlist_s *ev;

	while (events_used != events_avail)
	{
        event_t event;

		ev = &eventlist[events_used & (EVENTQUEUELENGTH-1)];
		if (ev->state)
			event.type = ev_keydown;
		else
			event.type = ev_keyup;

		event.data1 = ev->scancode;

		D_PostEvent(&event);

		events_used++;
	}
}


void PortableAction(int state, int action)
{
	LOGI("PortableAction %d %d",state, action);

	if ((action >= PORT_ACT_CUSTOM_0) && (action <= PORT_ACT_CUSTOM_17))
    {
        if( action <= PORT_ACT_CUSTOM_9 )
            PortableKeyEvent(state, KEY_KEYPAD0 + action - PORT_ACT_CUSTOM_0, 0);
    }
	else if (( PortableGetScreenMode() == TS_MENU ) || ( PortableGetScreenMode() == TS_BLANK )  || ( PortableGetScreenMode() == TS_Y_N ))
	{
		if (action >= PORT_ACT_MENU_UP && action <= PORT_ACT_MENU_ABORT)
		{

			int sdl_code [] = { KEY_UPARROW, KEY_DOWNARROW, KEY_LEFTARROW,
					KEY_RIGHTARROW, KEY_ENTER, KEY_ESCAPE };
			PortableKeyEvent(state, sdl_code[action-PORT_ACT_MENU_UP],0);
			return;
		}
	}
	else
	{
		switch (action)
		{
		case PORT_ACT_LEFT:
			state ?	PortableCommand("+left") : PortableCommand("-left");
			break;
		case PORT_ACT_RIGHT:
			state ?	PortableCommand("+right") : PortableCommand("-right");
			break;
		case PORT_ACT_FWD:
			state ?	PortableCommand("+forward") : PortableCommand("-forward");
			break;
		case PORT_ACT_BACK:
			state ?	PortableCommand("+back") : PortableCommand("-back");
			break;
		case PORT_ACT_MOVE_LEFT:
			state ?	PortableCommand("+strafeleft") : PortableCommand("-strafeleft");
			break;
		case PORT_ACT_MOVE_RIGHT:
			state ?	PortableCommand("+straferight") : PortableCommand("-straferight");
			break;
		case PORT_ACT_USE:
			state ?	PortableCommand("+use") : PortableCommand("-use");
			break;
		case PORT_ACT_ATTACK:
			state ?	PortableCommand("+fire") : PortableCommand("-fire");
			break;
		case PORT_ACT_NEXT_WEP:
            if(state)
			    PortableCommand("nextweap");
			break;
		case PORT_ACT_PREV_WEP:
            if(state)
    			PortableCommand("prevweap");
			break;
		case PORT_ACT_JUMP:
			state ?	PortableCommand("+jump") : PortableCommand("-jump");
			break;
		case PORT_ACT_MAP:
			if(state) PortableCommand("automap");
			break;
		case PORT_ACT_MAP_UP:

			break;
		case PORT_ACT_MAP_DOWN:

			break;
		case PORT_ACT_MAP_LEFT:

			break;
		case PORT_ACT_MAP_RIGHT:

			break;
		case PORT_ACT_MAP_ZOOM_IN:

			break;
		case PORT_ACT_MAP_ZOOM_OUT:

			break;
		case PORT_ACT_WEAP1:
			if(state)
				PortableCommand("weapon 1");
			break;
		case PORT_ACT_WEAP2:
			if(state)
				PortableCommand("weapon 2");
			break;
		case PORT_ACT_WEAP3:
			if(state)
				PortableCommand("weapon 3");
			break;
		case PORT_ACT_WEAP4:
			if(state)
				PortableCommand("weapon 4");
			break;
		case PORT_ACT_WEAP5:
			if(state)
				PortableCommand("weapon 5");
			break;
		case PORT_ACT_WEAP6:
			if(state)
				PortableCommand("weapon 6");
			break;
		case PORT_ACT_WEAP7:
			if(state)
				PortableCommand("weapon 7");
			break;
		case PORT_ACT_WEAP8:
			if(state)
				PortableCommand("weapon 8");
			break;
		case PORT_ACT_WEAP9:
			if(state)
				PortableCommand("weapon 9");
			break;
		case PORT_ACT_WEAP0:
			if(state)
				PortableCommand("weapon 10");
			break;
        case PORT_ACT_QUICKSAVE:
			if(state)
				PortableCommand("quicksave");
            break;
        case PORT_ACT_QUICKLOAD:
			if(state)
				PortableCommand("quickload");
            break;
        case PORT_ACT_CONSOLE:
			if (state)
				SDL_SendKeyboardKey(SDL_PRESSED, SDL_SCANCODE_GRAVE);
			else
				SDL_SendKeyboardKey(SDL_RELEASED, SDL_SCANCODE_GRAVE);
            break;
		}

	}
}

// =================== FORWARD and SIDE MOVMENT ==============

static float forwardmove_android, sidemove_android; //Joystick mode

void PortableMoveFwd(float fwd)
{
	if (fwd > 1)
		fwd = 1;
	else if (fwd < -1)
		fwd = -1;

	forwardmove_android = fwd;
}

void PortableMoveSide(float strafe)
{
	if (strafe > 1)
		strafe = 1;
	else if (strafe < -1)
		strafe = -1;

	sidemove_android = strafe;
}

void PortableMove(float fwd, float strafe)
{
	PortableMoveFwd(fwd);
	PortableMoveSide(strafe);
}

//======================================================================

//Look up and down
static float look_pitch_mouse,look_pitch_abs,look_pitch_joy;
void PortableLookPitch(int mode, float pitch)
{
	switch(mode)
	{
	case LOOK_MODE_MOUSE:
		look_pitch_mouse -= pitch;
		break;
	case LOOK_MODE_JOYSTICK:
		look_pitch_joy = pitch;
		break;
	}
}

//left right
static float look_yaw_mouse,look_yaw_joy;
void PortableLookYaw(int mode, float yaw)
{
	switch(mode)
	{
	case LOOK_MODE_MOUSE:
		look_yaw_mouse += yaw;
		break;
	case LOOK_MODE_JOYSTICK:
		look_yaw_joy = yaw;
		break;
	}
}


static float am_zoom = 0;
static float am_pan_x = 0;
static float am_pan_y = 0;

void PortableAutomapControl(float zoom, float x, float y)
{
	am_zoom += zoom;
	am_pan_x += x;
	am_pan_y += y;
}

static const char * quickCommand = 0;
void PortableCommand(const char * cmd)
{
	struct eventlistcmd_s *c = in_newevent_cmd();

	if(!c)
		return;

    snprintf(c->cmd, 256, "%s", cmd);

	in_finishevent_cmd();
}

void PortableInit(int argc,const char ** argv){

	extern int main_android(int argc, char **argv);
	main_android(argc,argv);
}

extern boolean menuactive;
extern boolean usergame;
extern boolean demoplayback;
extern boolean automapactive;
touchscreemode_t PortableGetScreenMode()
{
	if(menuactive) {
		return TS_MENU;
	}
	else
	{
		if (automapactive)
			return TS_MAP;
		else if( usergame )
			return TS_GAME;
		else if(demoplayback)
			return TS_DEMO;
		else
			return TS_BLANK;
	}
}

void Mobile_AM_controls(double *zoom, fixed_t *pan_x, fixed_t *pan_y )
{
	if (am_zoom)
	{
        *zoom = am_zoom * 10;
		am_zoom = 0;
	}

	*pan_x += (fixed_t)(am_pan_x * 20000000);
	*pan_y += -(fixed_t)(am_pan_y * 10000000);
	am_pan_x = am_pan_y = 0;
	//LOGI("zoom = %f",*zoom);
}

//in g_game.c for max speeds
extern fixed_t forwardmove[2];
extern fixed_t sidemove[2];
extern int mlooky;

//Called by doom on each tick
void Mobile_IN_Move(ticcmd_t* cmd )
{
    int blockGamepad( void );
    int blockMove = blockGamepad() & ANALOGUE_AXIS_FWD;
    int blockLook = blockGamepad() & ANALOGUE_AXIS_PITCH;


    if( !blockMove )
    {
        float fwdSpeed =  forwardmove_android;
        float sideSpeed = sidemove_android;

        if(!isPlayerRunning())
        {
            fwdSpeed = fwdSpeed / 2;
            sideSpeed = sideSpeed /2;
        }

	    cmd->forwardmove  += fwdSpeed * forwardmove[1];
	    cmd->sidemove  += sideSpeed   * sidemove[1];
    }

    if( !blockLook )
    {
		cmd->pitch -= look_pitch_mouse * 30000;
        look_pitch_mouse = 0;
		cmd->pitch -= look_pitch_joy * 1000;

        cmd->angleturn += look_yaw_mouse * 80000;
        look_yaw_mouse = 0;
        cmd->angleturn += look_yaw_joy * 1000;
    }

    while (events_cmd_used != events_cmd_avail)
    {
        struct eventlistcmd_s *ev = &eventlistcmd[events_cmd_used & (EVENTQUEUELENGTH-1)];

        G_ExecuteCommand(ev->cmd);

        events_cmd_used++;
    }
}

uint64_t SDLCALL SDL_GetTicks64(void)
{
	return SDL_GetTicks();
}