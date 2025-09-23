/* See LICENSE file for copyright and license details. */

/* appearance */
static unsigned int borderpx  = 2;        /* border pixel of windows */
static const int startwithgaps[]    = { 1 };	/* 1 means gaps are used by default, this can be customized for each tag */
static const unsigned int gappx[]   = { 4 };   /* default gap between windows in pixels, this can be customized for each tag */
static unsigned int snap      = 32;       /* snap pixel */
static int showbar            = 1;        /* 0 means no bar */
static int topbar             = 1;        /* 0 means bottom bar */

static char normbgcolor[]           = "#01080b";
static char normbordercolor[]       = "#222222";
static char normfgcolor[]           = "#086aab";
// TODO
// static char selfgcolor[]            = "#124578";
// static char selbordercolor[]        = "#02aacd";
// static char selbgcolor[]            = "#04202c";
static char selfgcolor[]            = "#01080b";
static char selbordercolor[]        = "#086aab";
static char selbgcolor[]            = "#02aacd";
// TODO
static char *colors[][3] = {
       /*               fg           bg           border   */
       [SchemeNorm] = { normfgcolor, normbgcolor, normbordercolor },
       [SchemeSel]  = { selfgcolor,  selbgcolor,  selbordercolor  },
       // [SchemeHid]  = { selfgcolor,  selbgcolor,  selbordercolor  },
       [SchemeHid]  = { normbgcolor,  normfgcolor,  selbgcolor  },
};

#ifdef __FreeBSD__
static char fonts[] = "MesloLGS:pixelsize=13:antialias=true:autohint=true";
static char dmenufont[] = "MesloLGS:pixelsize=13:antialias=true:autohint=true";
#else
static char font[] = "MesloLGS NF:pixelsize=13:antialias=true:autohint=true";
static char dmenufont[] = "MesloLGS NF:pixelsize=13:antialias=true:autohint=true";
#endif
static const char *fonts[] = { font };

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };
static const int taglayouts[] = { 0, 2, 2, 2, 2, 2, 2, 0, 0 };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class            instance    title             tags mask     isfloating   monitor */
  {"zenity",          NULL,       NULL,             0,            1,           -1},
  {"Xdialog",         NULL,       NULL,             0,            1,           -1},
  {"yad",             NULL,       NULL,             0,            1,           -1},
  {"steamwebhelper",  NULL,       "Friends List",   0,            1,           -1},
  // xev, it seems to not have class
  // {NULL,              NULL,       "Event Tester",   0,            1,           -1},

};

/* layout(s) */
static float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static int nmaster     = 1;    /* number of clients in master area */
static int resizehints = 1;    /* 1 means respect size hints in tiled resizals */
static int attachdirection = 0;    /* 0 default, 1 above, 2 aside, 3 below, 4 bottom, 5 top */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
	{ "DbD",      doubledeck },
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/usr/bin/env", "sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
// static char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", normbgcolor, "-nf", normfgcolor, "-sb", selbordercolor, "-sf", selfgcolor, NULL };
// static char *termcmd[]  = { "st", NULL };
static char *dmenucmd[] = { "sh", "-c", "exec dmenu_run -F", NULL };
static char *alt_dmenucmd[] = { "sh", "-c", "exec dmenu_run", NULL };
static char *alt_runner[] = { "sh", "-c", "exec rofi -show run -matching fuzzy", NULL };
static char *termcmd[]  = { "sh", "-c", "exec st", NULL };
static char *tab_termcmd[]  = { "sh", "-c", "exec tabbed st -w", NULL };
// TODO
// more scripted way
// password manager script
// monitors management script
// suspend script
// screenshots

/*
 * Xresources preferences to load at startup
 */
ResourcePref resources[] = {
		{ "font",               STRING,  &font },
		{ "dmenufont",          STRING,  &dmenufont },
		{ "normbgcolor",        STRING,  &normbgcolor },
		{ "normbordercolor",    STRING,  &normbordercolor },
		{ "normfgcolor",        STRING,  &normfgcolor },
		{ "selbgcolor",         STRING,  &selbgcolor },
		{ "selbordercolor",     STRING,  &selbordercolor },
		{ "selfgcolor",         STRING,  &selfgcolor },
		{ "borderpx",          	INTEGER, &borderpx },
		{ "snap",          		  INTEGER, &snap },
		{ "showbar",          	INTEGER, &showbar },
		{ "topbar",          	  INTEGER, &topbar },
		{ "nmaster",          	INTEGER, &nmaster },
		{ "resizehints",       	INTEGER, &resizehints },
		{ "attachdirection",    INTEGER, &attachdirection },
		{ "mfact",      	 	    FLOAT,   &mfact },
		{ "dmenucmd",      	 	  STRING,  &dmenucmd[2] },
		{ "alt_dmenucmd", 	 	  STRING,  &alt_dmenucmd[2] },
		{ "alt_runner",    	 	  STRING,  &alt_runner[2] },
		{ "termcmd",      	 	  STRING,  &termcmd[2] },
		{ "tab_termcmd",      	STRING,  &tab_termcmd[2] },
};

static const Key keys[] = {
	/* modifier                     key             function          argument */
	{ MODKEY,                       XK_d,           spawn,            {.v = dmenucmd } },
	{ MODKEY|ShiftMask,             XK_d,           spawn,            {.v = alt_dmenucmd } },
	{ MODKEY,                       XK_g,           spawn,            {.v = alt_runner } },
	{ MODKEY,                       XK_Return,      spawn,            {.v = tab_termcmd } },
	{ MODKEY|ShiftMask,             XK_Return,      spawn,            {.v = termcmd } },

	{ MODKEY|ShiftMask,             XK_space,       togglefloating,   {0} },
	{ MODKEY|ShiftMask,             XK_Tab,         togglebar,        {0} },

	{ MODKEY,                       XK_r,           reload_xres,      {0} },
	{ MODKEY,                       XK_c,           killclient,       {0} },
	{ MODKEY|ShiftMask,             XK_e,           quit,             {0} },

	{ MODKEY,                       XK_h,           focusstackvis,    {.i = -1 } },
	{ MODKEY,                       XK_l,           focusstackvis,    {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_h,           focusstackhid,    {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_l,           focusstackhid,    {.i = +1 } },

	{ MODKEY,                       XK_j,           setmfact,         {.f = -0.05} },
	{ MODKEY,                       XK_k,           setmfact,         {.f = +0.05} },
	{ MODKEY|ShiftMask,             XK_j,           setmfact,         {.f = -0.01} },
	{ MODKEY|ShiftMask,             XK_k,           setmfact,         {.f = +0.01} },

	{ MODKEY,                       XK_comma,       focusmon,         {.i = -1 } },
	{ MODKEY,                       XK_period,      focusmon,         {.i = +1 } },
	{ MODKEY|ControlMask,           XK_comma,       tagmon,           {.i = -1 } },
	{ MODKEY|ControlMask,           XK_period,      tagmon,           {.i = +1 } },

	{ MODKEY|Mod1Mask,              XK_k,           incnmaster,       {.i = +1 } },
	{ MODKEY|Mod1Mask,              XK_j,           incnmaster,       {.i = -1 } },

	{ MODKEY,                       XK_z,           zoom,             {0} },
  { MODKEY,                       XK_grave,       swapfocus,        {0} },
  { MODKEY|Mod1Mask,              XK_grave,       swapmon,          {0} },
	{ MODKEY|ControlMask,           XK_space,       focusmaster,      {0} },

	{ MODKEY,                       XK_v,           hidtoggle,        {0} },
	{ MODKEY|ControlMask,           XK_v,           showall,          {0} },

	{ MODKEY,                       XK_0,           view,             {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,           tag,              {.ui = ~0 } },
	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_6,                      5)
	TAGKEYS(                        XK_7,                      6)
	TAGKEYS(                        XK_8,                      7)
	TAGKEYS(                        XK_9,                      8)

	{ MODKEY,                       XK_minus,       setgaps,        {.i = -5 } },
	{ MODKEY,                       XK_equal,       setgaps,        {.i = +5 } },
	{ MODKEY|ShiftMask,             XK_minus,       setgaps,        {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_equal,       setgaps,        {.i = +1 } },
	{ MODKEY,                       XK_BackSpace,   setgaps,        {.i = GAP_RESET } },
	{ MODKEY|ShiftMask,             XK_BackSpace,   setgaps,        {.i = GAP_TOGGLE} },

	{ MODKEY|Mod1Mask,              XK_7,           setlayout,      {.v = &layouts[0]} },
	{ MODKEY|Mod1Mask,              XK_8,           setlayout,      {.v = &layouts[1]} },
	{ MODKEY|Mod1Mask,              XK_9,           setlayout,      {.v = &layouts[2]} },
	{ MODKEY|Mod1Mask,              XK_0,           setlayout,      {.v = &layouts[3]} },
	{ MODKEY,                       XK_space,       setlayout,      {0} },
};

// TODO
/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static const Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button2,        setlayout,      {.v = &layouts[3]} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button1,        togglewin,      {0} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};

