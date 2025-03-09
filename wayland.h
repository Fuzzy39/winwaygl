# ifndef HANDLERS
#define HANDLERS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct wayland_context
{
    // the display represents our connection to the compositor
    struct wl_display* display;

    // the registry is the means through which we can access important wayland objects.
    struct wl_registry* registry;

    // a reference to the compositor.
    struct wl_compositor* compositor;

    // a surface is a rectangle of pixels, I guess?
    struct wl_surface* surface;

    struct xdg_wm_base* xdg_wm_base;
    struct xdg_surface* xdg_surface;
    struct xdg_toplevel* xdg_toplevel;
};

extern struct wayland_context way;


const extern struct wl_registry_listener registry_listener;
const extern struct xdg_wm_base_listener xdg_listener;
const extern struct xdg_surface_listener xdg_surface_listener;
const extern struct xdg_toplevel_listener xdg_toplevel_listener;


#endif
