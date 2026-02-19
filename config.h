/* See LICENSE file for copyright and license details. */

/* appearance */
static unsigned int borderpx = 3; /* border pixel of windows */
static const int startwithgaps[] = {
    1
}; /* 1 means gaps are used by default, this can be customized
      for each tag */
static const unsigned int gappx[] = {
    4
}; /* default gap between windows in pixels, this can be
      customized for each tag */
static unsigned int snap = 32; /* snap pixel */
static int showbar = 1;        /* 0 means no bar */
static int topbar = 1;         /* 0 means bottom bar */

static char normbgcolor[] = "#01080b";
static char normbordercolor[] = "#333333";
static char normfgcolor[] = "#086aab"; // "#124578";
static char selfgcolor[] = "#04202c";
static char selbordercolor[] = "#0760a2";
static char selbgcolor[] = "#02aacd";

// TODO improve I guess
static char *colors[][3] = {
    /*               fg           bg           border   */
    [SchemeNorm] = {normfgcolor, normbgcolor, normbordercolor},
    [SchemeSel] = {selfgcolor,  selbgcolor,  selbgcolor     },
    // [SchemeHid]  = { selfgcolor,  selbgcolor,  selbordercolor
    // },
    [SchemeHid] = {normbgcolor, normfgcolor, selbordercolor },
};

static char font[256] = "MesloLGS:size=12:antialias=true:autohint=true";
static char dmenufont[256] = "MesloLGS:size=12:antialias=true:autohint=true";
static const char *fonts[] = {
    font,
    // All possible combinations just in case
    "MesloLGS:size=12",
    "MesloLGS NF:size=12:antialias=true:autohint=true",
    "MesloLGS NF:size=12",
    "MesloLGS Nerd Font:size=12:antialias=true:autohint=true",
    "MesloLGS Nerd Font:size=12",
    "MesloLG S NF:size=12:antialias=true:autohint=true",
    "MesloLG S NF:size=12",
    "Liberation Mono:size=12:antialias=true:autohint=true",
    "Liberation Mono:size=12",
    "monospace:size=12",
};

/* tagging */
static const char *tags[] = {
    "1", "2", "3", "4", "5", "6", "7", "8", "9"
};
static const int taglayouts[] = {0, 2, 2, 2, 2, 2, 2, 0, 0};

static const Rule rules[] = {
    /* xprop(1):
     *  WM_CLASS(STRING) = instance, class
     *  WM_NAME(STRING) = title
     */
    /* class instance title tags mask isfloating monitor */
    {"zenity",         NULL, NULL,                 0, 1, -1},
    {"Xdialog",        NULL, NULL,                 0, 1, -1},
    {"yad",            NULL, NULL,                 0, 1, -1},
    {"steamwebhelper", NULL, "Friends List",       0, 1, -1},
    {NULL,             NULL, "Picture-in-Picture", 0, 1, -1},
};

/* layout(s) */
static float mfact =
    0.55; /* factor of master area size [0.05..0.95] */
static int nmaster = 1; /* number of clients in master area */
static int resizehints =
    1; /* 1 means respect size hints in tiled resizals */
static int attachdirection = 2; /* 0 default, 1 above, 2 aside,
                                   3 below, 4 bottom, 5 top */
static const int lockfullscreen =
    1; /* 1 will force focus on the fullscreen window */

static const Layout layouts[] = {
    /* symbol     arrange function */
    {"[]=", tile      }, /* first entry is default */
    {"><>", NULL      }, /* no layout function means floating behavior */
    {"[M]", monocle   },
    {"DbD", doubledeck},
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY, TAG)                                      \
    {MODKEY, KEY, view, {.ui = 1 << TAG}},                     \
        {MODKEY | ControlMask,                                 \
         KEY,                                                  \
         toggleview,                                           \
         {.ui = 1 << TAG}},                                    \
        {MODKEY | ShiftMask, KEY, tag, {.ui = 1 << TAG}}, {    \
        MODKEY | ControlMask | ShiftMask, KEY, toggletag, {    \
            .ui = 1 << TAG                                     \
        }                                                      \
    }

/* helper for spawning shell commands in the pre dwm-5.0 fashion
 */
#define SHCMD(cmd)                                             \
    {                                                          \
        .v = (const char *[]) {                                \
            "/usr/bin/env", "sh", "-c", cmd, NULL              \
        }                                                      \
    }

/* component of dmenucmd, manipulated in spawn() */
static char dmenumon[2] = "0";

/* commands */
#define LOCAL_DIR(name) "$HOME/.local/dwm/helpers/" name
// runs script `name` if it can be run otherwise runs command
// `cmd`
#define LOCAL_DSCR(name, cmd)                                  \
    "if [ -x \"" LOCAL_DIR(name) "\" ]; then \"" LOCAL_DIR(    \
        name                                                   \
    ) "\" ; else " cmd " ; fi"
// same as above but adds argument `arg` to script
#define LOCAL_DSCR_ARG(name, cmd, arg)                         \
    "if [ -x \"" LOCAL_DIR(name) "\" ]; then \"" LOCAL_DIR(    \
        name                                                   \
    ) "\" " arg " ; else " cmd " ; fi"
// same as above but adds argument `arg` to both
#define LOCAL_DSCR_ARG2(name, cmd, arg)                        \
    "if [ -x \"" LOCAL_DIR(name) "\" ]; then \"" LOCAL_DIR(    \
        name                                                   \
    ) "\" " arg " ; else " cmd " " arg " ; fi"
#define EMPTY_CMD "true"

static char *dmenucmd[] = {
    "sh",
    "-c",
    LOCAL_DSCR_ARG2("dmenucmd", "exec dmenu_run -m", "$0"),
    dmenumon,
    NULL
};
static char *dmenucmd_shift[] = {
    "sh",
    "-c",
    LOCAL_DSCR_ARG2(
        "dmenucmd_shift", "exec dmenu_run -F -m", "$0"
    ),
    dmenumon,
    NULL
};
static char *dmenucmd_ctrl[] = {
    "sh",
    "-c",
    LOCAL_DSCR_ARG2("dmenucmd_ctrl", "exec dmenu_run -m", "$0"),
    dmenumon,
    NULL
};
static char *alt_runner[] = {
    "sh",
    "-c",
    LOCAL_DSCR_ARG(
        "alt_runner",
        "exec rofi -show run -matching fuzzy",
        "$0"
    ),
    dmenumon,
    NULL
};
static char *alt_runner_shift[] = {
    "sh",
    "-c",
    LOCAL_DSCR_ARG(
        "alt_runner_shift",
        "exec rofi -show run -matching fuzzy",
        "$0"
    ),
    dmenumon,
    NULL
};
static char *alt_runner_ctrl[] = {
    "sh",
    "-c",
    LOCAL_DSCR_ARG(
        "alt_runner_ctrl",
        "exec rofi -show run -matching fuzzy",
        "$0"
    ),
    dmenumon,
    NULL
};
static char *termcmd[] = {
    "sh",
    "-c",
    LOCAL_DSCR_ARG("termcmd", "exec tabbed st -w", "$0"),
    dmenumon,
    NULL
};
static char *termcmd_shift[] = {
    "sh",
    "-c",
    LOCAL_DSCR_ARG("termcmd_shift", "exec st", "$0"),
    dmenumon,
    NULL
};
static char *termcmd_ctrl[] = {
    "sh",
    "-c",
    LOCAL_DSCR_ARG("termcmd_ctrl", "exec st", "$0"),
    dmenumon,
    NULL
};
static char *suspend[] = {
    "sh",
    "-c",
    LOCAL_DSCR_ARG("suspend", "slock", "$0"),
    dmenumon,
    NULL
};
static char *suspend_shift[] = {
    "sh",
    "-c",
    LOCAL_DSCR_ARG("suspend_shift", "systemctl suspend", "$0"),
    dmenumon,
    NULL
};
static char *suspend_ctrl[] = {
    "sh",
    "-c",
    LOCAL_DSCR_ARG("suspend_ctrl", "loginctl suspend", "$0"),
    dmenumon,
    NULL
};
static char *pass_man[] = {
    "sh",
    "-c",
    LOCAL_DSCR_ARG2("pass_man", "passmenu -m", "$0"),
    dmenumon,
    NULL
};
static char *pass_man_shift[] = {
    "sh",
    "-c",
    LOCAL_DSCR_ARG2("pass_man_shift", "passmenu-otp -m", "$0"),
    dmenumon,
    NULL
};
static char *pass_man_ctrl[] = {
    "sh",
    "-c",
    LOCAL_DSCR_ARG2("pass_man_ctrl", "passmenu-otp -m", "$0"),
    dmenumon,
    NULL
};
static char *mon_man[] = {
    "sh",
    "-c",
    LOCAL_DSCR_ARG("mon_man", EMPTY_CMD, "$0"),
    dmenumon,
    NULL
};
static char *mon_man_shift[] = {
    "sh",
    "-c",
    LOCAL_DSCR_ARG("mon_man_shift", EMPTY_CMD, "$0"),
    dmenumon,
    NULL
};
static char *mon_man_ctrl[] = {
    "sh",
    "-c",
    LOCAL_DSCR_ARG("mon_man_ctrl", EMPTY_CMD, "$0"),
    dmenumon,
    NULL
};
static char *screenshot[] = {
    "sh",
    "-c",
    LOCAL_DSCR_ARG("screenshot", EMPTY_CMD, "$0"),
    dmenumon,
    NULL
};
static char *screenshot_shift[] = {
    "sh",
    "-c",
    LOCAL_DSCR_ARG("screenshot_shift", EMPTY_CMD, "$0"),
    dmenumon,
    NULL
};
static char *screenshot_ctrl[] = {
    "sh",
    "-c",
    LOCAL_DSCR_ARG("screenshot_ctrl", EMPTY_CMD, "$0"),
    dmenumon,
    NULL
};
static char *media[] = {
    "sh",
    "-c",
    LOCAL_DSCR_ARG("media", "pavucontrol", "$0"),
    dmenumon,
    NULL
};
static char *media_shift[] = {
    "sh",
    "-c",
    LOCAL_DSCR_ARG("media_shift", EMPTY_CMD, "$0"),
    dmenumon,
    NULL
};
static char *media_ctrl[] = {
    "sh",
    "-c",
    LOCAL_DSCR_ARG("media_ctrl", EMPTY_CMD, "$0"),
    dmenumon,
    NULL
};
static char *user_cmd[] = {
    "sh",
    "-c",
    LOCAL_DSCR_ARG("user_cmd", EMPTY_CMD, "$0"),
    dmenumon,
    NULL
};
static char *user_cmd_shift[] = {
    "sh",
    "-c",
    LOCAL_DSCR_ARG("user_cmd_shift", EMPTY_CMD, "$0"),
    dmenumon,
    NULL
};
static char *user_cmd_ctrl[] = {
    "sh",
    "-c",
    LOCAL_DSCR_ARG("user_cmd_ctrl", EMPTY_CMD, "$0"),
    dmenumon,
    NULL
};
static char *user_cmd_alt[] = {
    "sh",
    "-c",
    LOCAL_DSCR_ARG("user_cmd_alt", EMPTY_CMD, "$0"),
    dmenumon,
    NULL
};
static char *bar_cmd1[] = {
    "sh",
    "-c",
    LOCAL_DSCR_ARG("bar_cmd1", EMPTY_CMD, "$0"),
    dmenumon,
    NULL
};
static char *bar_cmd2[] = {
    "sh",
    "-c",
    LOCAL_DSCR_ARG("bar_cmd2", EMPTY_CMD, "$0"),
    dmenumon,
    NULL
};
static char *bar_cmd3[] = {
    "sh",
    "-c",
    LOCAL_DSCR_ARG("bar_cmd3", EMPTY_CMD, "$0"),
    dmenumon,
    NULL
};

/*
 * Xresources preferences to load at startup
 */
ResourcePref resources[] = {
    {"font",             STRING,  font               },
    {"dmenufont",        STRING,  dmenufont          },
    {"normbgcolor",      STRING,  normbgcolor        },
    {"normbordercolor",  STRING,  normbordercolor    },
    {"normfgcolor",      STRING,  normfgcolor        },
    {"selbgcolor",       STRING,  selbgcolor         },
    {"selbordercolor",   STRING,  selbordercolor     },
    {"selfgcolor",       STRING,  selfgcolor         },
    {"borderpx",         INTEGER, &borderpx           },
    {"snap",             INTEGER, &snap               },
    {"showbar",          INTEGER, &showbar            },
    {"topbar",           INTEGER, &topbar             },
    {"nmaster",          INTEGER, &nmaster            },
    {"resizehints",      INTEGER, &resizehints        },
    {"attachdirection",  INTEGER, &attachdirection    },
    {"mfact",            FLOAT,   &mfact              },
};

static const Key keys[] = {
    /* modifier                     key             function
       argument */
    {MODKEY,               XK_d,         spawn,          {.v = dmenucmd}        },
    {MODKEY | ShiftMask,   XK_d,         spawn,          {.v = dmenucmd_shift}  },
    {MODKEY | ControlMask, XK_d,         spawn,          {.v = dmenucmd_ctrl}   },
    {MODKEY,               XK_g,         spawn,          {.v = alt_runner}      },
    {MODKEY | ShiftMask,   XK_g,         spawn,          {.v = alt_runner_shift}},
    {MODKEY | ControlMask, XK_g,         spawn,          {.v = alt_runner_ctrl} },
    {MODKEY | Mod1Mask,    XK_Return,    reload_xres,    {0}                    },
    {MODKEY,               XK_Return,    spawn,          {.v = termcmd}         },
    {MODKEY | ShiftMask,   XK_Return,    spawn,          {.v = termcmd_shift}   },
    {MODKEY | ControlMask, XK_Return,    spawn,          {.v = termcmd_ctrl}    },
    {MODKEY,               XK_s,         spawn,          {.v = suspend}         },
    {MODKEY | ShiftMask,   XK_s,         spawn,          {.v = suspend_shift}   },
    {MODKEY | ControlMask, XK_s,         spawn,          {.v = suspend_ctrl}    },
    {MODKEY,               XK_w,         spawn,          {.v = pass_man}        },
    {MODKEY | ShiftMask,   XK_w,         spawn,          {.v = pass_man_shift}  },
    {MODKEY | ControlMask, XK_w,         spawn,          {.v = pass_man_ctrl}   },
    {MODKEY,               XK_m,         spawn,          {.v = mon_man}         },
    {MODKEY | ShiftMask,   XK_m,         spawn,          {.v = mon_man_shift}   },
    {MODKEY | ControlMask, XK_m,         spawn,          {.v = mon_man_ctrl}    },
    {MODKEY,               XK_y,         spawn,          {.v = screenshot}      },
    {MODKEY | ShiftMask,   XK_y,         spawn,          {.v = screenshot_shift}},
    {MODKEY | ControlMask, XK_y,         spawn,          {.v = screenshot_ctrl} },
    {MODKEY,               XK_Print,     spawn,          {.v = screenshot}      },
    {MODKEY | ShiftMask,   XK_Print,     spawn,          {.v = screenshot_shift}},
    {MODKEY | ControlMask, XK_Print,     spawn,          {.v = screenshot_ctrl} },
    {MODKEY,               XK_u,         spawn,          {.v = media}           },
    {MODKEY | ShiftMask,   XK_u,         spawn,          {.v = media_shift}     },
    {MODKEY | ControlMask, XK_u,         spawn,          {.v = media_ctrl}      },
    {MODKEY,               XK_r,         spawn,          {.v = user_cmd}        },
    {MODKEY | ShiftMask,   XK_r,         spawn,          {.v = user_cmd_shift}  },
    {MODKEY | ControlMask, XK_r,         spawn,          {.v = user_cmd_ctrl}   },
    {MODKEY | Mod1Mask,    XK_r,         spawn,          {.v = user_cmd_alt}    },

    {MODKEY,               XK_c,         killclient,     {0}                    },
    {MODKEY | ShiftMask,   XK_e,         quit,           {0}                    },

    {MODKEY,               XK_h,         focusstackvis,  {.i = -1}              },
    {MODKEY,               XK_l,         focusstackvis,  {.i = +1}              },
    {MODKEY | ShiftMask,   XK_h,         focusstackhid,  {.i = -1}              },
    {MODKEY | ShiftMask,   XK_l,         focusstackhid,  {.i = +1}              },
    {MODKEY | Mod1Mask,    XK_h,         pushup,         {0}                    },
    {MODKEY | Mod1Mask,    XK_l,         pushdown,       {0}                    },

    {MODKEY,               XK_j,         setmfact,       {.f = -0.05}           },
    {MODKEY,               XK_k,         setmfact,       {.f = +0.05}           },
    {MODKEY | ShiftMask,   XK_j,         setmfact,       {.f = -0.01}           },
    {MODKEY | ShiftMask,   XK_k,         setmfact,       {.f = +0.01}           },
    {MODKEY | Mod1Mask,    XK_k,         incnmaster,     {.i = +1}              },
    {MODKEY | Mod1Mask,    XK_j,         incnmaster,     {.i = -1}              },

    {MODKEY,               XK_comma,     focusmon,       {.i = -1}              },
    {MODKEY,               XK_period,    focusmon,       {.i = +1}              },
    {MODKEY | ShiftMask,   XK_comma,     tagmon,         {.i = -1}              },
    {MODKEY | ShiftMask,   XK_period,    tagmon,         {.i = +1}              },
    // TODO implement direction for swapmon
    {MODKEY | Mod1Mask,    XK_comma,     swapmon,        {.i = -1}              },
    {MODKEY | Mod1Mask,    XK_period,    swapmon,        {.i = +1}              },

    {MODKEY | ShiftMask,   XK_Tab,       togglebar,      {0}                    },
    {MODKEY | ControlMask, XK_Tab,       togglefloating, {0}                    },
    {MODKEY | Mod1Mask,    XK_Tab,       unfloatvisible, {0}                    },

    {MODKEY,               XK_z,         focusmaster,    {0}                    },
    {MODKEY | ShiftMask,   XK_z,         zoom,           {0}                    },
    {MODKEY,               XK_space,     swapfocus,      {0}                    },
    {MODKEY | Mod1Mask,    XK_space,     setlayout,      {0}                    },

    {MODKEY,               XK_v,         hidtoggle,      {0}                    },
    {MODKEY | ControlMask, XK_v,         showall,        {0}                    },

    {MODKEY,               XK_0,         view,           {.ui = ~0}             },
    {MODKEY | ShiftMask,   XK_0,         tag,            {.ui = ~0}             },
    TAGKEYS(XK_1, 0),
    TAGKEYS(XK_2, 1),
    TAGKEYS(XK_3, 2),
    TAGKEYS(XK_4, 3),
    TAGKEYS(XK_5, 4),
    TAGKEYS(XK_6, 5),
    TAGKEYS(XK_7, 6),
    TAGKEYS(XK_8, 7),
    TAGKEYS(XK_9, 8),
    {MODKEY,               XK_minus,     setgaps,        {.i = -5}              },
    {MODKEY,               XK_equal,     setgaps,        {.i = +5}              },
    {MODKEY | ShiftMask,   XK_minus,     setgaps,        {.i = -1}              },
    {MODKEY | ShiftMask,   XK_equal,     setgaps,        {.i = +1}              },
    {MODKEY,               XK_BackSpace, setgaps,        {.i = GAP_RESET}       },
    {MODKEY | ShiftMask,   XK_BackSpace, setgaps,        {.i = GAP_TOGGLE}      },

    {MODKEY | Mod1Mask,    XK_6,         setlayout,      {.v = &layouts[0]}     },
    {MODKEY | Mod1Mask,    XK_7,         setlayout,      {.v = &layouts[1]}     },
    {MODKEY | Mod1Mask,    XK_8,         setlayout,      {.v = &layouts[2]}     },
    {MODKEY | Mod1Mask,    XK_9,         setlayout,      {.v = &layouts[3]}     },
};

// TODO it could be more powerful I guess
/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText,
 * ClkWinTitle, ClkClientWin, or ClkRootWin */
static const Button buttons[] = {
    /* click                event mask      button function
       argument */
    {ClkLtSymbol,   0,         Button1, setlayout,      {0}               },
    {ClkLtSymbol,   0,         Button2, setlayout,      {.v = &layouts[3]}},
    {ClkLtSymbol,   0,         Button3, setlayout,      {.v = &layouts[2]}},
    {ClkWinTitle,   0,         Button1, togglewin,      {0}               },
    {ClkWinTitle,   0,         Button2, zoom,           {0}               },
    {ClkWinTitle,   0,         Button3, hidtoggle,      {0}               },
    // TODO
    // {ClkWinTitle,   Mod1Mask,  Button3, killclient,     {0}               },

    // scroll
    {ClkWinTitle,   0,         Button4, focusstackvis,  {.i = -1}         },
    {ClkWinTitle,   0,         Button5, focusstackvis,  {.i = +1}         },
    {ClkWinTitle,   ShiftMask, Button4, focusstackhid,  {.i = -1}         },
    {ClkWinTitle,   ShiftMask, Button5, focusstackhid,  {.i = +1}         },

    {ClkStatusText, 0,         Button1, spawn,          {.v = bar_cmd1}   },
    {ClkStatusText, 0,         Button2, spawn,          {.v = bar_cmd2}   },
    {ClkStatusText, 0,         Button3, spawn,          {.v = bar_cmd3}   },
    {ClkClientWin,  MODKEY,    Button1, movemouse,      {0}               },
    {ClkClientWin,  MODKEY,    Button2, togglefloating, {0}               },
    {ClkClientWin,  MODKEY,    Button3, resizemouse,    {0}               },
    {ClkTagBar,     0,         Button1, view,           {0}               },
    {ClkTagBar,     0,         Button3, toggleview,     {0}               },
    {ClkTagBar,     MODKEY,    Button1, tag,            {0}               },
    {ClkTagBar,     MODKEY,    Button3, toggletag,      {0}               },
};
