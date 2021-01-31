/* See LICENSE file for copyright and license details. */

#include "tcl.c"

/* appearance */
//static const unsigned int gappih    = 10;       /* horiz inner gap between windows */
//static const unsigned int gappiv    = 10;       /* vert inner gap between windows */
//static const unsigned int gappoh    = 10;       /* horiz outer gap between windows and screen edge */
//static const unsigned int gappov    = 10;       /* vert outer gap between windows and screen edge */
static const unsigned int gappx     = 6;        /* gaps between windows */
static const unsigned int borderpx  = 0;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const int smartgaps          = 0;        /* 1 means no outer gap when there is only one window */

/*  Display modes of the tab bar: never shown, always shown, shown only in  */
/*  monocle mode in the presence of several windows.                        */
/*  Modes after showtab_nmodes are disabled.                                */
//enum showtab_modes { showtab_never, showtab_auto, showtab_nmodes, showtab_always};
//static const int showtab			= showtab_always;        /* Default tab bar show mode */
//static const Bool toptab				= True;               /* False means bottom tab bar */
// static const char resources[]	    = { "~/.Xresources" };

static const char *fonts[]          = { "monospace:size=9" };
static const char dmenufont[]       = "monospace:size=9";
static const char col_gray1[]       = "#01080b"; // "#222222";
static const char col_gray2[]       = "#02aacd"; // "#444444";
static const char col_gray3[]       = "#086aab"; // "#bbbbbb";

//	[SchemeNorm] = { "#086aab", "#01080b" },
//	[SchemeSel] = { "#01080b", "#02aacd" },

static const char col_gray4[]       = "#01080b"; //"#445566";
static const char col_cyan[]        = "#02aacd"; //"#00cd48";
static const char *colors[][3]      = {
	/*               fg         bg         border   */
	[SchemeNorm] = { col_gray3, col_gray1, col_gray2 },
	[SchemeSel]  = { col_gray4, col_cyan,  col_cyan  },
};

/* tagging */
static const char *tags[] = { ">$", "#i", "@w", "||", "::", ">>", "+∞", "∨∧", "„”"};

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class      instance    title       tags mask     isfloating   monitor */
//	{ "Gimp",     NULL,       NULL,       0,            1,           -1 },
//	{ "Firefox",  NULL,       NULL,       1 << 8,       0,           -1 },
//	{ "mpv",  NULL,       NULL,       0,       1,           -1 }
//	{ "net-mc-main-Main", "sun-awt-X11-XFramePeer",     NULL,       0,       1,        -1 }
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
static const char *dmenucmd[] = { "dmenu_run", NULL };
static const char *rofi[] = { "rofi", "-show", "run", NULL };
static const char *rofic[] = { "rofi", "-show", "calc", NULL };
static const char *termcmd[]  = { "st", NULL };
static const char *termtab[]  = { "sh", "-c", "tabbed st -w", NULL };
static const char *alacr[]  = { "alacritty", NULL };
static const char *urxvt[]  = { "urxvt", NULL };
static const char *urxvtt[]  = { "sh", "-c", "tabbed urxvt -embed", NULL };
static const char *pcman[]  = { "pcmanfm", NULL };
static const char *qute[]  = { "qutebrowser", NULL };
static const char *pqute[]  = { "qutebrowser", "-T", NULL };
static const char *qterm[]  = { "qterminal", NULL };
static const char *scrsh[]  = { "shotgun", NULL };
static const char *shotg[]  = { "sh", "-c", "~/.scrs/scrshg.sh", NULL };
static const char *shotc[]  = { "sh", "-c", "~/.scrs/scrshc.sh", NULL };
static const char *surf[]  = { "sh", "-c", "tabbed surf -e", NULL };

static const char *pvolm[]  = { "sh", "-c", "pactl set-sink-volume @DEFAULT_SINK@ -5%", NULL };
static const char *pvolp[]  = { "sh", "-c", "pactl set-sink-volume @DEFAULT_SINK@ +5%", NULL };
static const char *volm[]  = { "sh", "-c", "amixer -c 0 -- sset Master 2dB-", NULL };
static const char *volp[]  = { "sh", "-c", "amixer -c 0 -- sset Master 2dB+", NULL };
//static const char *susp[]  = { "loginctl",  "suspend", NULL };
//static const char *ssusp[]  = { "sh", "-c", "loginctl suspend && slock", NULL };
static const char *susp[]  = { "sh", "-c", "export C=`doas fgconsole` N=`doas fgconsole -n` && doas chvt $N && loginctl suspend && doas chvt $C && export C= N=", NULL };
static const char *bsusp[]  = { "sh", "-c", "export C=`doas fgconsole` N=`doas fgconsole -n` && doas chvt $N && i3lock-fancy -p && loginctl suspend && doas chvt $C && export C= N=", NULL };
static const char *isusp[]  = { "sh", "-c", "export C=`doas fgconsole` N=`doas fgconsole -n` && doas chvt $N && loginctl suspend && sleep 1 && doas chvt $C && xlock && export C= N=", NULL };
//static const char *bsusp[]  = { "sh", "-c", "i3lock-fancy -p && loginctl suspend", NULL };
//static const char *isusp[]  = { "xlock",  "-startCmd",  "loginctl suspend", NULL };
static const char *xra[]  = { "sh", "-c", "xrandr --output HDMI1 --mode 1920x1080 --primary && xrandr --output LVDS1 --right-of HDMI1 && nitrogen --restore", NULL };

static const char *plumb[]  = { "sh", "-c", "xclip -o | sh -s | xmessage -file -", NULL };
static const char *plumbc[]  = { "sh", "-c", "xclip -o | sh -s | xclip -o | xclip -selection clipboard -i", NULL };
static const char *dclip[]  = { "sh", "-c", "echo | dmenu && xclip -o | xclip -selection clipboard -i", NULL };
static const char *clip[]  = { "sh", "-c", "xclip -o | xclip -selection clipboard -i", NULL };

static const char *scr1[]  = { "sh", "-c", "~/.scrs/scr1.sh", NULL };
static const char *scr2[]  = { "sh", "-c", "~/.scrs/scr2.sh", NULL };
static const char *scr3[]  = { "sh", "-c", "~/.scrs/scr3.sh", NULL };
static const char *imgv[]  = { "sh", "-c", "~/.scrs/imgv.sh", NULL };
static const char *mocp[]  = { "sh", "-c", "~/.scrs/mode.sh mocp", NULL };
static const char *acmem[]  = { "sh", "-c", "~/.scrs/mode.sh acme", NULL };
static const char *dispm[]  = { "sh", "-c", "~/.scrs/display-maker.sh", NULL };
static const char *qwerty[]  = { "sh", "-c", "xmodmap ~/.xmodmap.qwerty", NULL };
static const char *workman[]  = { "sh", "-c", "xmodmap ~/.xmodmap.workman", NULL };

static Key keys[] = {
	/* modifier                     key        function        argument */
	{ MODKEY,                       XK_d,      spawn,          {.v = dmenucmd } },
	{ MODKEY,                       XK_g,      spawn,          {.v = rofi } },
	{ MODKEY|ControlMask,           XK_g,      spawn,          {.v = rofic } },
	{ MODKEY,                       XK_Return, spawn,          {.v = termcmd } },
	{ MODKEY|ControlMask,           XK_Return, spawn,          {.v = termtab } },
	{ MODKEY,                       XK_slash,  spawn,          {.v = urxvt } },
	{ MODKEY|ShiftMask,             XK_slash,  spawn,          {.v = urxvtt } },
	{ MODKEY|ShiftMask,             XK_a,	   spawn,          {.v = alacr } },
	{ MODKEY,		 	XK_p,	   spawn,          {.v = dispm } },
	{ MODKEY|ShiftMask,	 	XK_p,	   spawn,          {.v = xra } },
	{ MODKEY,                       XK_x, 	   spawn,          {.v = pcman} },
	{ MODKEY,                       XK_q, 	   spawn,          {.v = qute } },
	{ MODKEY|ControlMask,           XK_q, 	   spawn,          {.v = qterm } },
	{ MODKEY|ShiftMask,             XK_q, 	   spawn,          {.v = pqute } },
	{ MODKEY|Mod1Mask,              XK_s, 	   spawn,          {.v = bsusp } },
	{ MODKEY|ControlMask,           XK_Home,   spawn,          {.v = qwerty } },
	{ MODKEY|ControlMask,           XK_End,    spawn,          {.v = workman } },
	{ MODKEY,                       XK_b, 	   spawn,          {.v = surf } },

	{ MODKEY|ShiftMask,             XK_s, 	   spawn,          {.v = isusp } },
//	{ MODKEY|ControlMask,           XK_s, 	   spawn,          {.v = ssusp } },
//	{ MODKEY|ControlMask,           XK_l, 	   spawn,          {.v = susp } },
	{ MODKEY|Mod1Mask,	        XK_l, 	   spawn,          {.v = scrsh} },
	{ MODKEY|Mod1Mask|ControlMask,  XK_l, 	   spawn,          {.v = shotg} },
	{ MODKEY|Mod1Mask|ShiftMask,    XK_l, 	   spawn,          {.v = shotc} },
	{ MODKEY,           		XK_s, 	   spawn,          {.v = plumb } },
	{ MODKEY,           		XK_m, 	   spawn,          {.v = mocp } },
	{ MODKEY|ShiftMask,             XK_d,      spawn,      	{.v = plumbc } },
	{ MODKEY|Mod1Mask,              XK_d,      spawn,      	{.v = acmem } },
	{ MODKEY,	                XK_grave,  spawn,      	{.v = scr1} },
	{ MODKEY|ShiftMask,	        XK_grave,  spawn,      	{.v = scr2} },
	{ MODKEY|ControlMask,           XK_grave,  spawn,      	{.v = scr3} },
	{ MODKEY,	                XK_i,      spawn,      	{.v = imgv } },
	{ MODKEY,	                XK_Tab,    spawn,      	{.v = dclip } },
	{ MODKEY|Mod1Mask,              XK_Tab,    spawn,      	{.v = clip } },
	{ MODKEY|ShiftMask,             XK_Tab,    togglebar,      {-1} },
//	{ MODKEY,                       XK_w,      tabmode,        {-1} },

	{ MODKEY,                       XK_l,      focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_h,      focusstack,     {.i = -1 } },
	{ MODKEY|Mod1Mask,              XK_k,      incnmaster,     {.i = +1 } },
	{ MODKEY|Mod1Mask,              XK_j,      incnmaster,     {.i = -1 } },
	{ MODKEY|Mod1Mask,              XK_h,      view,     { -1 } },
	{ MODKEY,                       XK_j,      setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_k,      setmfact,       {.f = +0.05} },
	{ MODKEY|ShiftMask,             XK_j,      setmfact,       {.f = -0.01} },
	{ MODKEY|ShiftMask,             XK_k,      setmfact,       {.f = +0.01} },
	{ MODKEY|ShiftMask,             XK_Return, zoom,           {0} },
	{ MODKEY|ControlMask,           XK_Tab,    view,           {0} },
	{ MODKEY,	                XK_c,      killclient,     {0} },
	{ MODKEY|ControlMask,           XK_semicolon,      setlayout,      {.v = &layouts[0]} },
	{ MODKEY|ControlMask,           XK_apostrophe,      setlayout,      {.v = &layouts[1]} },
	{ MODKEY|ControlMask,           XK_backslash,      setlayout,      {.v = &layouts[2]} },
	{ MODKEY|ControlMask,           XK_bracketright,      setlayout,      {.v = &layouts[3]} },
	{ MODKEY|ControlMask,           XK_bracketleft,      setlayout,      {.v = &layouts[4]} },
	{ MODKEY,                       XK_space,  setlayout,      {0} },

	{ MODKEY|ControlMask,           XK_space,  focusmaster,      {0} },
	{ MODKEY|ShiftMask,             XK_space,  togglefloating, {0} },
//	{ MODKEY,                       XK_f,      fullscreen,  {0} },
	{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_bracketleft, focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_bracketright, focusmon,       {.i = +1 } },
	{ MODKEY|ControlMask,           XK_comma,  tagmon,         {.i = -1 } },
	{ MODKEY|ControlMask,           XK_period, tagmon,         {.i = +1 } },
	{ MODKEY,	                XK_comma,  spawn,          {.v = pvolm } },
	{ MODKEY,        	        XK_period, spawn,          {.v = pvolp } },
	{ MODKEY|ShiftMask,             XK_comma,  spawn,          {.v = volm } },
	{ MODKEY|ShiftMask,             XK_period, spawn,          {.v = volp } },

        { MODKEY, 	  		XK_BackSpace,  setgaps,        {.i = 0  } },
        { MODKEY, 	       	     XK_equal,  setgaps,        {.i = +3  } },
        { MODKEY,      		     XK_minus,  setgaps,        {.i = -3  } },
	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_6,                      5)
	TAGKEYS(                        XK_7,                      6)
	TAGKEYS(                        XK_8,                      7)
	TAGKEYS(                        XK_9,                      8)
	{ MODKEY|ShiftMask,             XK_e,      quit,           {0} },
        { 0,                            HOLDKEY,   holdbar,        {0} },
//        { MODKEY|Mod1Mask|ShiftMask,    XK_Tab,   holdbar,        {0} },

};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button1,        focusstack,     {-1} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkWinTitle,          0,              Button3,        focusstack,     {1} },
	{ ClkWinTitle,          0,              Button4,        focusstack,     {1} },
	{ ClkWinTitle,          0,              Button5,        focusstack,     {-1} },
	{ ClkStatusText,        0,              Button2,        spawn,          { .v = termcmd } },
	{ ClkRootWin,           0,              Button2,        spawn,          { .v = termcmd } },
	{ ClkRootWin,           0,              Button3,        spawn,          { .v = termcmd } },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkClientWin,         MODKEY|ShiftMask,         Button1,        spawn, { .v = plumbc } },
	{ ClkClientWin,         MODKEY|ShiftMask,         Button2,        spawn, { .v = plumb } },
	{ ClkClientWin,         MODKEY|ShiftMask,         Button3,        spawn, { .v = clip } },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
//	{ ClkTabBar,            0,              Button1,        focuswin,       {0} },
};

