#include <X11/XF86keysym.h>
/* See LICENSE file for copyright and license details. */

/* appearance */
static const unsigned int borderpx  = 1;	/* border pixel of windows */
static const unsigned int snap	    = 32;	/* snap pixel */
static const int showbar	    = 1;	/* 0 means no bar */
static const int topbar		    = 1;	/* 0 means bottom bar */
static const char *fonts[]	    = { "monospace:size=10" };
static const char dmenufont[]	    = "monospace:size=10";
static char normbgcolor[]	    = "#222222";
static char normbordercolor[]	    = "#444444";
static char normfgcolor[]	    = "#bbbbbb";
static char selfgcolor[]	    = "#eeeeee";
static char selbordercolor[]	    = "#005577";
static char selbgcolor[]	    = "#005577";
static char *colors[][3] = {
       /*		fg	     bg		  border   */
       [SchemeNorm] = { normfgcolor, normbgcolor, normbordercolor },
       [SchemeSel]  = { selfgcolor,  selbgcolor,  selbordercolor  },
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class      instance	  title       tags mask     isfloating	 monitor */
	{ "Gimp",	 NULL,	     NULL,	 0,	       1,	    -1 },
	// { "Firefox",  NULL,	     NULL,	 1 << 8,       0,	    -1 },
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */
static const int decorhints  = 1;    /* 1 means respect decoration hints */

#include "fibonacci.c"
static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[\\]",     dwindle },
	{ "[]=",      tile },	 /* first entry is default */
	{ "><>",      NULL },	 /* no layout function means floating behavior */
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,			KEY,	  view,		  {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,		KEY,	  toggleview,	  {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,		KEY,	  tag,		  {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,	  toggletag,	  {.ui = 1 << TAG} },

/* commands */
#define CMD(x) (const char *[]){x, NULL}
#define SHCMD(x) (const char *[]){"/bin/sh", "-c", x, NULL}
#define RAISERUN(cls, cmd) {.v = &(const struct RaiseOrRun){.class = cls, .run = cmd}}
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", normbgcolor, "-nf", normfgcolor, "-sb", selbordercolor, "-sf", selfgcolor, NULL };

static const Key keys[] = {
	/* modifier			key	   function	   argument */
	{ MODKEY|ShiftMask,		XK_e,	   quit,	   {0} },
	{ MODKEY,			XK_F5,	   xrdb,	   {.v = NULL } },
	{ MODKEY,			XK_b,	   togglebar,	   {0} },

	{ MODKEY,			XK_i,	   setlayout,	   {.v = &layouts[0]} },
	{ MODKEY,			XK_o,	   setlayout,	   {.v = &layouts[1]} },
	{ MODKEY,			XK_p,	   setlayout,	   {.v = &layouts[2]} },
	{ MODKEY,			XK_space,  setlayout,	   {0} },

	{ MODKEY,			XK_w,      raiserun,	   RAISERUN("weechat", SHCMD("exec $TERMINAL -c weechat -e weechat")) },
	{ MODKEY|ShiftMask,		XK_w,      raiserun,	   RAISERUN("firefox", SHCMD("exec $BROWSER")) }, // environment variable
	{ MODKEY,			XK_d,	   spawn,	   {.v = dmenucmd } },
	{ MODKEY,			XK_Return, spawn,	   {.v = CMD("st")} },
	{ MODKEY,			XK_x,      spawn,	   {.v = (const char *[]){ "loginctl", "lock-session", NULL } } },

	{ MODKEY,			XK_slash,  spawn,	   {.v = (const char *[]){ "brightness", "up", NULL } } },
	{ MODKEY|ShiftMask,		XK_slash,  spawn,	   {.v = (const char *[]){ "brightness", "down", NULL } } },

	{ MODKEY,			XK_backslash,spawn,	   {.v = (const char *[]){ "volume", "mute", NULL } } },
	{ MODKEY,			XK_bracketleft,spawn,	   {.v = (const char *[]){ "volume", "down", NULL } } },
	{ MODKEY|ShiftMask,		XK_bracketleft,spawn,	   {.v = (const char *[]){ "volume", "down", "1", NULL } } },
	{ MODKEY,			XK_bracketright,spawn,	   {.v = (const char *[]){ "volume", "up", NULL } } },
	{ MODKEY|ShiftMask,		XK_bracketright,spawn,	   {.v = (const char *[]){ "volume", "up", "1", NULL } } },

	{ MODKEY,			XK_s,	   spawn,	   {.v = (const char *[]){ "scrsht", NULL } } },
	{ MODKEY|ShiftMask,		XK_s,	   spawn,	   {.v = (const char *[]){ "scrsht", "fullscreen", NULL } } },
	{ MODKEY|ControlMask,		XK_s,	   spawn,	   {.v = (const char *[]){ "scrsht", "select", NULL } } },

	{ 0,				XF86XK_PowerOff,spawn,	   {.v = CMD("powermenu") } },

	{ MODKEY|ShiftMask,		XK_q,	   killclient,	   {0} },
	{ MODKEY|ShiftMask,		XK_space,  togglefloating, {0} },
	{ MODKEY,			XK_f, 	   togglefullscr,   {0} },

	{ MODKEY,			XK_j,	   focusstack,	   {.i = INC(-1) } },
	{ MODKEY,			XK_k,	   focusstack, 	   {.i = INC(+1) } },
	{ MODKEY,			XK_h,	   focusstack, 	   {.i = 0 } },
	{ MODKEY,			XK_l,	   focusstack,	   {.i = -1 } },
	{ MODKEY|ShiftMask,		XK_j,	   pushstack, 	   {.i = INC(-1) } },
	{ MODKEY|ShiftMask,		XK_k,	   pushstack,	   {.i = INC(+1) } },
	{ MODKEY|ShiftMask,		XK_h,	   setmfact,	   {.f = -0.05} },
	{ MODKEY|ShiftMask,		XK_l,	   setmfact,	   {.f = +0.05} },

	{ MODKEY,			XK_Tab,    view,	   {0} },
	TAGKEYS(			XK_1,			   0)
	TAGKEYS(			XK_2,			   1)
	TAGKEYS(			XK_3,			   2)
	TAGKEYS(			XK_4,			   3)
	TAGKEYS(			XK_5,			   4)
	TAGKEYS(			XK_6,			   5)
	TAGKEYS(			XK_7,			   6)
	TAGKEYS(			XK_8,			   7)
	TAGKEYS(			XK_9,			   8)
	{ MODKEY,			XK_0,	   view,	   {.ui = ~0 } },
	{ MODKEY|ShiftMask,		XK_0,	   tag,		   {.ui = ~0 } },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static const Button buttons[] = {
	/* click		event mask	button		function	argument */
	{ ClkLtSymbol,		0,		Button1,	setlayout,	{0} },
	{ ClkLtSymbol,		0,		Button3,	setlayout,	{.v = &layouts[2]} },
	// { ClkStatusText,	0,		Button2,	spawn,		{.v = termcmd } },
	{ ClkClientWin,		MODKEY,		Button1,	movemouse,	{0} },
	{ ClkClientWin,		MODKEY,		Button2,	togglefloating, {0} },
	{ ClkClientWin,		MODKEY,		Button3,	resizemouse,	{0} },
	{ ClkTagBar,		0,		Button1,	view,		{0} },
	{ ClkTagBar,		0,		Button3,	toggleview,	{0} },
	{ ClkTagBar,		MODKEY,		Button1,	tag,		{0} },
	{ ClkTagBar,		MODKEY,		Button3,	toggletag,	{0} },
};

