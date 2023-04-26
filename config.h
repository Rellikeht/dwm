/* See LICENSE file for copyright and license details. */

#include "tcl.c"

/* appearance */
static const unsigned int gappx     = 4;        /* gaps between windows */
static const unsigned int borderpx  = 0;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const int smartgaps          = 0;        /* 1 means no outer gap when there is only one window */

#ifdef __FreeBSD__
    static const char *fonts[] = {
	"MesloLGS:pixelsize=13:antialias=true:autohint=true" };
    static const char dmenufont[] =
	"MesloLGS:pixelsize=13:antialias=true:autohint=true";

#else
    static const char *fonts[] = {
	"MesloLGS NF:pixelsize=13:antialias=true:autohint=true" };
    static const char dmenufont[] =
	"MesloLGS NF:pixelsize=13:antialias=true:autohint=true";

#endif

static const char col_gray1[]       = "#01080b"; // "#222222";
static const char col_gray2[]       = "#02aacd"; // "#444444";
static const char col_gray3[]       = "#086aab"; // "#bbbbbb";
static const char col_gray4[]       = "#01080b"; //"#445566";
static const char col_cyan[]        = "#02aacd"; //"#00cd48";
static const char *colors[][3]      = {
	/*               fg         bg         border   */
	[SchemeNorm] = { col_gray3, col_gray1, col_gray2 },
	[SchemeSel]  = { col_gray4, col_cyan,  col_gray3 },
	[SchemeHid]  = { col_gray1, col_gray3,  col_gray2 },
};

/* tagging */
//static const char *tags[] = { ">$", "#i", "@w", "||", "::", ">>", "+∞", "∨∧", "„”"};
static const char *tags[] = {"1", "2", "3", "4", "5", "6", "7", "8", "9"};
//static const char *tags[] = {"•1•", "•2•", "•3•", "•4•", "•5•", "•6•", "•7•", "•8•", "•9•"};

/* default layout per tags */
/* The first element is for all-tag view, following i-th element corresponds to */
/* tags[i]. Layout is referred using the layouts array index.*/
static int def_layouts[1 + LENGTH(tags)]  = { 2, 2, 2, 2, 2, 2, 2, 2, 2, 2};

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class      instance    title       tags mask     isfloating   monitor */
	{ "Firefox",  NULL,       NULL,       1 << 8,       0,           -1 },
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */
static const int attachdirection = 0;    /* 0 default, 1 above, 2 aside, 3 below, 4 bottom, 5 top */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
	{ "|||",      tcl },
	{ "DbD",      doubledeck }
};

/* key definitions */
#define MODKEY Mod4Mask
#define HOLDKEY XK_Scroll_Lock
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
//static const char *dmenucmd[] = { "dmenu_run", NULL };
static const char *dmenucmd[] = { "sh", "-c", "~/.dwm/drun.sh", NULL };
static const char *dmenuscr[] = { "sh", "-c", "ls -d --color=never ~/.dscripts/* | dmenu | sh -s", NULL };
static const char *rofi[] = { "rofi", "-show", "run", NULL };
static const char *rofic[] = { "rofi", "-show", "calc", NULL };
static const char *termcmd[]  = { "st", NULL };
static const char *termtab[]  = { "sh", "-c", "tabbed st -w", NULL };
static const char *alacrt[]  = { "sh", "-c", "tabbed alacritty --embed", NULL };
static const char *urxvtt[]  = { "sh", "-c", "tabbed urxvt -embed", NULL };
static const char *qute[]  = { "sh", "-c", "$qutebrowser", NULL };
static const char *pqute[]  = { "sh", "-c", "$qutebrowser -T", NULL };
static const char *shotg[]  = { "sh", "-c", "~/.dwm/scrshg.sh", NULL };
static const char *surf[]  = { "sh", "-c", "tabbed surf -e", NULL };

static const char *scrkbd[]  = { "svkbd-mobile-intl", NULL };
static const char *poweroff[]  = { "sh", "-c", "loginctl poweroff", NULL };
static const char *susp[]  = { "sh", "-c", "~/.local_scrs/suspend1.sh", NULL };
static const char *bsusp[]  = { "sh", "-c", "~/.local_scrs/suspend2.sh", NULL };
static const char *isusp[]  = { "sh", "-c", "~/.local_scrs/suspend3.sh", NULL };

//static const char *plumb[]  = { "sh", "-c", "xclip -o | sh -s | xmessage -file -", NULL };
//static const char *plumbc[]  = { "sh", "-c", "xclip -o | sh -s | xclip -o | xclip -selection clipboard -i", NULL };
static const char *dclip[]  = { "sh", "-c", "echo | dmenu && xclip -o | xclip -selection clipboard -i", NULL };
static const char *clip[]  = { "sh", "-c", "xclip -o | xclip -selection CLIPBOARD -i", NULL };
static const char *passm[]  = { "passmenu", NULL };
static const char *passt[]  = { "passmenu --type", NULL };

static const char *scr1[]  = { "sh", "-c", "~/.dwm/scr1.sh", NULL };
static const char *scr2[]  = { "sh", "-c", "~/.dwm/scr2.sh", NULL };
static const char *scr3[]  = { "sh", "-c", "~/.dwm/scr3.sh", NULL };
static const char *scr4[]  = { "sh", "-c", "~/.dwm/scr4.sh", NULL };
static const char *mocp[]  = { "sh", "-c", "~/.dwm/mode.sh mocp", NULL };
static const char *cmus[]  = { "sh", "-c", "~/.dwm/mode.sh cmus", NULL };
static const char *dispm[]  = { "sh", "-c", "~/.dwm/dispm.sh", NULL };
static const char *qwerty[]  = { "sh", "-c", "xmodmap ~/.xmodmap/qwerty", NULL };
static const char *workman[]  = { "sh", "-c", "xmodmap ~/.xmodmap/workman", NULL };

static Key keys[] = {
	/* modifier                     key		function        argument */
	{ MODKEY,                       XK_d,      	spawn,		{.v = dmenucmd } },
	{ MODKEY|ShiftMask,             XK_d,      	spawn,		{.v = dmenuscr } },
	{ MODKEY,                       XK_g,      	spawn,		{.v = rofi } },
	{ MODKEY|ControlMask,           XK_g,      	spawn,		{.v = rofic } },
	{ MODKEY,                       XK_Return, 	spawn,		{.v = termtab } },
	{ MODKEY|ControlMask,           XK_Return, 	spawn,		{.v = termcmd } },
	{ MODKEY,                       XK_slash,  	spawn,		{.v = urxvtt } },
	{ MODKEY|ControlMask,           XK_a,	   	spawn,		{.v = alacrt } },
	{ MODKEY,		 	XK_p,	   	spawn,		{.v = dispm } },
	{ MODKEY,                       XK_q, 	   	spawn,		{.v = qute } },
	{ MODKEY|ShiftMask,             XK_q, 	   	spawn,		{.v = pqute } },
	{ MODKEY,	                XK_i,      	spawn,		{.v = workman } },
	{ MODKEY,	             	XK_u,      	spawn,		{.v = qwerty } },
	{ MODKEY,                       XK_w,      	spawn,		{ .v = passm } },
	{ MODKEY|ControlMask,           XK_w,      	spawn,		{ .v = scrkbd } },
	{ MODKEY|ShiftMask,             XK_w,      	spawn,		{ .v = passt } },
	{ MODKEY,             		XK_BackSpace,	spawn,		{ .v = qwerty } },

	{ MODKEY|ShiftMask,             XK_s,		spawn,		{.v = isusp } },
	{ MODKEY|ControlMask,           XK_s, 	   	spawn,		{.v = susp } },
	{ MODKEY|Mod1Mask,              XK_s, 	   	spawn,		{.v = bsusp } },
	{ MODKEY|ControlMask,           XK_e, 	   	spawn,		{.v = poweroff } },

	{ MODKEY|Mod1Mask|ControlMask,  XK_l, 	   	spawn,		{.v = shotg} },
	{ MODKEY,           		XK_m, 	   	spawn,		{.v = mocp } },
	{ MODKEY|ShiftMask,           	XK_m, 	   	spawn,		{.v = cmus} },
	{ MODKEY,	                XK_grave,  	spawn,		{.v = scr1} },
	{ MODKEY|ShiftMask,	        XK_grave,  	spawn,		{.v = scr2} },
	{ MODKEY|ControlMask,           XK_grave,  	spawn,		{.v = scr3} },
	{ MODKEY|ShiftMask|ControlMask, XK_grave,  	spawn,		{.v = scr4} },
	{ MODKEY,	                XK_Tab,    	spawn,		{.v = dclip } },
	{ MODKEY|Mod1Mask,              XK_Tab,    	spawn,		{.v = clip } },
	{ MODKEY|ShiftMask,             XK_Tab,    	togglebar,	{-1} },
//	{ MODKEY|ShiftMask,             XK_semicolon,	tabmode,	{-1} },
//	{ MODKEY|ShiftMask,             XK_apostrophe,	tabmode,	{0} },
//	{ MODKEY|ShiftMask,             XK_backslash,	tabmode,        {1} },
//	{ MODKEY|ShiftMask,             XK_bracketright,tabmode,        {2} },
//	{ MODKEY|ShiftMask,             XK_bracketleft,	tabmode,        {3} },
	{ MODKEY,			XK_v,		show,		{0} },
	{ MODKEY|ShiftMask,             XK_v,		hide,        	{0} },

//	{ MODKEY,                       XK_l,      	focusstack,     {.i = +1 } },
//	{ MODKEY,                       XK_h,      	focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_l,      	focusstackvis,  {.i = +1 } },
	{ MODKEY,                       XK_h,      	focusstackvis,  {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_l,      	focusstackhid,  {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_h,      	focusstackhid,  {.i = -1 } },
	{ MODKEY|Mod1Mask,              XK_k,      	incnmaster,     {.i = +1 } },
	{ MODKEY|Mod1Mask,              XK_j,      	incnmaster,     {.i = -1 } },
	{ MODKEY|Mod1Mask,              XK_h,      	view,     	{ -1 } },
	{ MODKEY,                       XK_j,      	setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_k,      	setmfact,       {.f = +0.05} },
	{ MODKEY|ShiftMask,             XK_j,      	setmfact,       {.f = -0.01} },
	{ MODKEY|ShiftMask,             XK_k,      	setmfact,       {.f = +0.01} },
	{ MODKEY|ShiftMask,             XK_Return, 	zoom,           {0} },
	{ MODKEY|ControlMask,           XK_Tab,    	view,           {0} },
	{ MODKEY,	                XK_c,      	killclient,     {0} },
	{ MODKEY|ControlMask,           XK_semicolon,	setlayout,      {.v = &layouts[0]} },
	{ MODKEY|ControlMask|ShiftMask,	XK_space,	setlayout,      {.v = &layouts[1]} },
	{ MODKEY|ControlMask,		XK_p,		setlayout,      {.v = &layouts[2]} },
	{ MODKEY|ControlMask|ShiftMask,	XK_p,		setlayout,      {.v = &layouts[3]} },
	{ MODKEY|ControlMask|ShiftMask,	XK_semicolon,	setlayout,      {.v = &layouts[4]} },

	{ MODKEY|ControlMask,           XK_space,	focusmaster,	{0} },
	{ MODKEY|ShiftMask,             XK_space,	togglefloating, {0} },
	{ MODKEY,                       XK_0,		view,		{.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,		tag,		{.ui = ~0 } },
	{ MODKEY,                       XK_n,		focusmon,	{.i = -1 } },
	{ MODKEY|ShiftMask,             XK_n,		focusmon,	{.i = +1 } },
	{ MODKEY|ControlMask,           XK_comma,	tagmon,		{.i = -1 } },
	{ MODKEY|ControlMask,           XK_period,	tagmon,		{.i = +1 } },

        { MODKEY, 	  		XK_BackSpace,	setgaps,        {.i = 0  } },
        { MODKEY,			XK_equal,	setgaps,        {.i = +3  } },
        { MODKEY,			XK_minus,	setgaps,        {.i = -3  } },
	TAGKEYS(                        XK_1,		0)
	TAGKEYS(                        XK_2,		1)
	TAGKEYS(                        XK_3,		2)
	TAGKEYS(                        XK_4,		3)
	TAGKEYS(                        XK_5,		4)
	TAGKEYS(                        XK_6,		5)
	TAGKEYS(                        XK_7,		6)
	TAGKEYS(                        XK_8,		7)
	TAGKEYS(                        XK_9,		8)
	{ MODKEY|ShiftMask,             XK_e,		quit,		{0} },
        { 0,                            HOLDKEY,	holdbar,	{0} },

};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask	    button	    function		argument */
	{ ClkLtSymbol,          0,		    Button1,	    setlayout,		{0} },
	{ ClkLtSymbol,          0,		    Button3,	    setlayout,		{.v = &layouts[2]} },
	{ ClkWinTitle,          0,		    Button1,	    togglewin,		{0} },
	{ ClkWinTitle,          0,		    Button2,	    zoom,		{0} },
	{ ClkWinTitle,          0,		    Button4,	    focusstackvis,	{-1} },
	{ ClkWinTitle,          0,		    Button5,	    focusstackvis,	{1} },
	{ ClkStatusText,        0,		    Button2,	    spawn,		{ .v = termcmd } },
	{ ClkStatusText,        0,		    Button3,	    spawn,		{ .v = scrkbd } },
	{ ClkRootWin,           0,		    Button2,	    spawn,		{ .v = termcmd } },
	{ ClkRootWin,           0,		    Button3,	    spawn,		{ .v = termcmd } },
	{ ClkClientWin,         MODKEY,		    Button1,	    movemouse,		{0} },
	{ ClkClientWin,         MODKEY,		    Button2,	    togglefloating,	{0} },
	{ ClkClientWin,         MODKEY,		    Button3,	    resizemouse,	{0} },
	{ ClkClientWin,         MODKEY|ShiftMask,   Button3,	    spawn,		{ .v = clip } },
	{ ClkTagBar,            0,		    Button1,	    view,		{0} },
	{ ClkTagBar,            0,		    Button3,	    toggleview,		{0} },
	{ ClkTagBar,            MODKEY,		    Button1,	    tag,		{0} },
	{ ClkTagBar,            MODKEY,		    Button3,	    toggletag,		{0} },
};

