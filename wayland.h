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

    // shared memory for rendering to the screen
    struct wl_shm* shm;
    struct wl_shm_pool* pool;

    // a surface is a rectangle of pixels, I guess?
    struct wl_surface* surface;

  

    struct xdg_wm_base* xdg_wm_base;
    struct xdg_surface* xdg_surface;
    struct xdg_toplevel* xdg_toplevel;

    struct zxdg_decoration_manager_v1* xdg_deco_manager;
    struct zxdg_toplevel_decoration_v1* xdg_toplevel_deco;
};

extern struct wayland_context way;


const extern struct wl_registry_listener registry_listener;
const extern struct xdg_wm_base_listener xdg_listener;
const extern struct xdg_surface_listener xdg_surface_listener;
const extern struct xdg_toplevel_listener xdg_toplevel_listener;
const extern struct zxdg_toplevel_decoration_v1_listener xdg_toplevel_deco_listener;

// an actual function wow
void create_shm_pool();


#endif
