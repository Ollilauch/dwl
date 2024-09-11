/* Taken from https://github.com/djpohly/dwl/issues/466 */
#define COLOR(hex)    { ((hex >> 24) & 0xFF) / 255.0f, \
                        ((hex >> 16) & 0xFF) / 255.0f, \
                        ((hex >> 8) & 0xFF) / 255.0f, \
                        (hex & 0xFF) / 255.0f }
/* appearance */
static const int sloppyfocus               = 1;  /* focus follows mouse */
static const int bypass_surface_visibility = 0;  /* 1 means idle inhibitors will disable idle tracking even if it's surface isn't visible  */
static const int smartgaps                 = 0;  /* 1 means no outer gap when there is only one window */
static int gaps                            = 1;  /* 1 means gaps between windows are added */
static const unsigned int gappx            = 10; /* gap pixel between windows */
static const unsigned int borderpx         = 2;  /* border pixel of windows */
static const float rootcolor[]             = COLOR(0x222222ff);
static const float bordercolor[]           = COLOR(0x444444ff);
static const float focuscolor[]            = COLOR(0x6E56AFff);
static const float urgentcolor[]           = COLOR(0xff0000ff);
/* This conforms to the xdg-protocol. Set the alpha to zero to restore the old behavior */
static const float fullscreen_bg[]         = {0.1f, 0.1f, 0.1f, 1.0f}; /* You can also use glsl colors */
static const int respect_monitor_reserved_area = 0;  /* 1 to monitor center while respecting the monitor's reserved area, 0 to monitor center */
static const char *cursor_theme            = "macOS";
static const char cursor_size[]            = "24"; /* Make sure it's a valid integer, otherwise things will break */

/* tagging - TAGCOUNT must be no greater than 31 */
#define TAGCOUNT (9)

/* logging */
static int log_level = WLR_ERROR;

/* NOTE: ALWAYS keep a rule declared even if you don't use rules (e.g leave at least one example) */
static const Rule rules[] = {
    /* app_id             title       tags mask     isfloating   monitor   x   y   width   height */
    /* examples: */
    { "Gimp_EXAMPLE",     NULL,       0,            1,           -1,       0,  0,  1000,   0.75 }, /* Start on currently visible tags floating, not tiled */
    { "firefox_EXAMPLE",  NULL,       1 << 8,       0,           -1,       0,  0,  0,      0 },/* Start on ONLY tag "9" */
};

/* layout(s) */
static const Layout layouts[] = {
    /* symbol     arrange function */
    { "[]=",      tile },
    { "><>",      NULL },    /* no layout function means floating behavior */
    { "[M]",      monocle },
    { "@|@",      snail },
};

/* monitors */
/* (x=-1, y=-1) is reserved as an "autoconfigure" monitor position indicator
 * WARNING: negative values other than (-1, -1) cause problems with Xwayland clients
 * https://gitlab.freedesktop.org/xorg/xserver/-/issues/899
*/
/* NOTE: ALWAYS add a fallback rule, even if you are completely sure it won't be used */
static const MonitorRule monrules[] = {
    /* name       mfact  nmaster scale layout       rotate/reflect                x    y */
    /* example of a HiDPI laptop monitor:
    { "eDP-1",    0.5f,  1,      2,    &layouts[0], WL_OUTPUT_TRANSFORM_NORMAL,   -1,  -1 },
    */
    /* defaults */
    { "eDP-1",     0.5f, 1,      1.5f,    &layouts[3], WL_OUTPUT_TRANSFORM_NORMAL,   -1,  -1 },
    { NULL,        0.5f, 1,      1.5f,    &layouts[3], WL_OUTPUT_TRANSFORM_NORMAL,   -1,  -1 },
};

/* keyboard */
static const struct xkb_rule_names xkb_rules = {
    /* can specify fields: rules, model, layout, variant, options */
    /* example:
    .options = "ctrl:nocaps",
    */
    .layout = "de",
    .options = NULL,
};

static const int repeat_rate = 25;
static const int repeat_delay = 600;

/* Trackpad */
static const int tap_to_click = 1;
static const int tap_and_drag = 1;
static const int drag_lock = 1;
static const int natural_scrolling = 1;
static const int disable_while_typing = 1;
static const int left_handed = 0;
static const int middle_button_emulation = 0;
/* You can choose between:
LIBINPUT_CONFIG_SCROLL_NO_SCROLL
LIBINPUT_CONFIG_SCROLL_2FG
LIBINPUT_CONFIG_SCROLL_EDGE
LIBINPUT_CONFIG_SCROLL_ON_BUTTON_DOWN
*/
static const enum libinput_config_scroll_method scroll_method = LIBINPUT_CONFIG_SCROLL_2FG;

/* You can choose between:
LIBINPUT_CONFIG_CLICK_METHOD_NONE
LIBINPUT_CONFIG_CLICK_METHOD_BUTTON_AREAS
LIBINPUT_CONFIG_CLICK_METHOD_CLICKFINGER
*/
static const enum libinput_config_click_method click_method = LIBINPUT_CONFIG_CLICK_METHOD_BUTTON_AREAS;

/* You can choose between:
LIBINPUT_CONFIG_SEND_EVENTS_ENABLED
LIBINPUT_CONFIG_SEND_EVENTS_DISABLED
LIBINPUT_CONFIG_SEND_EVENTS_DISABLED_ON_EXTERNAL_MOUSE
*/
static const uint32_t send_events_mode = LIBINPUT_CONFIG_SEND_EVENTS_ENABLED;

/* You can choose between:
LIBINPUT_CONFIG_ACCEL_PROFILE_FLAT
LIBINPUT_CONFIG_ACCEL_PROFILE_ADAPTIVE
*/
static const enum libinput_config_accel_profile accel_profile = LIBINPUT_CONFIG_ACCEL_PROFILE_FLAT;
static const double accel_speed = 1.0;

/* You can choose between:
LIBINPUT_CONFIG_TAP_MAP_LRM -- 1/2/3 finger tap maps to left/right/middle
LIBINPUT_CONFIG_TAP_MAP_LMR -- 1/2/3 finger tap maps to left/middle/right
*/
static const enum libinput_config_tap_button_map button_map = LIBINPUT_CONFIG_TAP_MAP_LRM;

/* If you want to use the windows key for MODKEY, use WLR_MODIFIER_LOGO */
#define MODKEY WLR_MODIFIER_LOGO

#define TAGKEYS(KEY,SKEY,TAG) \
	{ MODKEY,                    KEY,            view,            {.ui = 1 << TAG} }, \
	{ MODKEY|WLR_MODIFIER_CTRL,  KEY,            toggleview,      {.ui = 1 << TAG} }, \
	{ MODKEY|WLR_MODIFIER_SHIFT, SKEY,           tag,             {.ui = 1 << TAG} }, \
	{ MODKEY|WLR_MODIFIER_CTRL|WLR_MODIFIER_SHIFT,SKEY,toggletag, {.ui = 1 << TAG} }

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static const char *termcmd[] = { "kitty", NULL };
static const char *menucmd[] = { "wmenu-run", NULL };
static const char *webbrowser[] = { "chromium", NULL };

/* brightness */
static const char *brightness_up[] = {"dwm-brightness", "up", NULL};
static const char *brightness_down[] = {"dwm-brightness", "down", NULL};

/* volume */
static const char *upvol[] = {"dwm-volume", "up", NULL};
static const char *downvol[] = {"dwm-volume", "down", NULL};
static const char *mutevol[] = {"dwm-volume", "toggle", NULL};
static const char *umutevol[] = {"dwm-volume", "umute", NULL};
static const char *mutemic[] = {"dwm-volume", "mic toggle", NULL};

/* screenshot */
static const char *screenshot[] = {"dwl-screenshot", NULL};

static const Key keys[] = {
    /* Note that Shift changes certain key codes: c -> C, 2 -> at, etc. */
    /* modifier                  key                 function        argument */
    { MODKEY,                    XKB_KEY_d,          spawn,          {.v = menucmd} },
    { MODKEY|WLR_MODIFIER_SHIFT, XKB_KEY_Return,     spawn,          {.v = termcmd} },
    { MODKEY,                    XKB_KEY_Return,     spawn,          {.v = webbrowser} },
    { MODKEY,                    XKB_KEY_b,          togglebar,      {0} },
    { MODKEY,                    XKB_KEY_j,          focusstack,     {.i = +1} },
    { MODKEY,                    XKB_KEY_k,          focusstack,     {.i = -1} },
    { MODKEY|WLR_MODIFIER_SHIFT, XKB_KEY_J,          movestack,      {.i = +1} },
    { MODKEY|WLR_MODIFIER_SHIFT, XKB_KEY_K,          movestack,      {.i = -1} },
    { MODKEY,                    XKB_KEY_i,          incnmaster,     {.i = +1} },
    { MODKEY,                    XKB_KEY_p,          incnmaster,     {.i = -1} },
    { MODKEY,                    XKB_KEY_h,          setmfact,       {.f = -0.05f} },
    { MODKEY,                    XKB_KEY_l,          setmfact,       {.f = +0.05f} },
    {
        MODKEY,                    XKB_KEY_Return,     zoom,           {0}
    },
    { MODKEY,                    XKB_KEY_Tab,        view,           {0} },
    { MODKEY,                    XKB_KEY_g,          togglegaps,     {0} },
    { MODKEY|WLR_MODIFIER_SHIFT, XKB_KEY_Q,          killclient,     {0} },
    { MODKEY,                    XKB_KEY_t,          setlayout,      {.v = &layouts[0]} },
    { MODKEY,                    XKB_KEY_f,          setlayout,      {.v = &layouts[1]} },
    { MODKEY,                    XKB_KEY_m,          setlayout,      {.v = &layouts[2]} },
    { MODKEY,                    XKB_KEY_s,          setlayout,      {.v = &layouts[3]} },
    { MODKEY,                    XKB_KEY_space,      setlayout,      {0} },
    { MODKEY|WLR_MODIFIER_SHIFT, XKB_KEY_space,      togglefloating, {0} },
    { MODKEY,                    XKB_KEY_e,         togglefullscreen, {0} },
    { MODKEY,                    XKB_KEY_0,          view,           {.ui = ~0} },
    { MODKEY|WLR_MODIFIER_SHIFT, XKB_KEY_equal, tag,            {.ui = ~0} },
    { MODKEY,                    XKB_KEY_comma,      focusmon,       {.i = WLR_DIRECTION_LEFT} },
    { MODKEY,                    XKB_KEY_period,     focusmon,       {.i = WLR_DIRECTION_RIGHT} },
    { MODKEY|WLR_MODIFIER_SHIFT, XKB_KEY_less,       tagmon,         {.i = WLR_DIRECTION_LEFT} },
    { MODKEY|WLR_MODIFIER_SHIFT, XKB_KEY_greater,    tagmon,         {.i = WLR_DIRECTION_RIGHT} },
    TAGKEYS(          XKB_KEY_1, XKB_KEY_exclam,                      0),
    TAGKEYS(          XKB_KEY_2, XKB_KEY_quotedbl,                    1),
    TAGKEYS(          XKB_KEY_3, XKB_KEY_section,                     2),
    TAGKEYS(          XKB_KEY_4, XKB_KEY_dollar,                      3),
    TAGKEYS(          XKB_KEY_5, XKB_KEY_percent,                     4),
    TAGKEYS(          XKB_KEY_6, XKB_KEY_ampersand,                   5),
    TAGKEYS(          XKB_KEY_7, XKB_KEY_slash,                       6),
    TAGKEYS(          XKB_KEY_8, XKB_KEY_parenleft,                   7),
    TAGKEYS(          XKB_KEY_9, XKB_KEY_parenright,                  8),
    { MODKEY|WLR_MODIFIER_SHIFT, XKB_KEY_Delete,          quit,           {0} },

    // Function Keys
    {0,                          XKB_KEY_XF86MonBrightnessUp,   spawn, {.v = brightness_up}},
    {0,                          XKB_KEY_XF86MonBrightnessDown, spawn, {.v = brightness_down}},
    {0,                          XKB_KEY_XF86AudioLowerVolume,  spawn, {.v = downvol}},
    {0,                          XKB_KEY_XF86AudioLowerVolume,  spawn, {.v = umutevol}},
    {0,                          XKB_KEY_XF86AudioMute,         spawn, {.v = mutevol}},
    {0,                          XKB_KEY_XF86AudioRaiseVolume,  spawn, {.v = upvol}},
    {0,                          XKB_KEY_XF86AudioRaiseVolume,  spawn, {.v = umutevol}},
    {0,                          XKB_KEY_XF86AudioMicMute,      spawn, {.v = mutemic}},
    {0,                          XKB_KEY_Print,                 spawn, {.v = screenshot}},

    /* Ctrl-Alt-Backspace and Ctrl-Alt-Fx used to be handled by X server */
    { WLR_MODIFIER_CTRL|WLR_MODIFIER_ALT,XKB_KEY_Terminate_Server, quit, {0} },
    /* Ctrl-Alt-Fx is used to switch to another VT, if you don't know what a VT is
     * do not remove them.
     */
#define CHVT(n) { WLR_MODIFIER_CTRL|WLR_MODIFIER_ALT,XKB_KEY_XF86Switch_VT_##n, chvt, {.ui = (n)} }
    CHVT(1), CHVT(2), CHVT(3), CHVT(4), CHVT(5), CHVT(6),
    CHVT(7), CHVT(8), CHVT(9), CHVT(10), CHVT(11), CHVT(12),
};

static const Button buttons[] = {
    { MODKEY, BTN_LEFT,   moveresize,     {.ui = CurMove} },
    { MODKEY, BTN_MIDDLE, togglefloating, {0} },
    { MODKEY, BTN_RIGHT,  moveresize,     {.ui = CurResize} },
};
