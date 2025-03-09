#include <wayland-client.h>
#include "protocols/xdg-shell.h"

#include "wayland.h"

// handles getting events
static void global_registry_handler
( 
    void *data, 
    struct wl_registry *registry, 
    uint32_t id, 
    const char *interface, 
    uint32_t version
)
{
    // print that we got an event
    printf("Got a registry event for <%s>, id: %d, version: %d.\n",
        interface, id, version);

    if(strcmp(interface, wl_compositor_interface.name) == 0)
    {
        way.compositor = wl_registry_bind(registry, id, &wl_compositor_interface, 1);
        return;
    }
    if (strcmp(interface, xdg_wm_base_interface.name) == 0)
    {
        way.xdg_wm_base = wl_registry_bind(registry, id, &xdg_wm_base_interface, 1);
        return;
    }
    if (strcmp(interface, wl_shm_interface.name) == 0)
    {
        way.xdg_wm_base = wl_registry_bind(registry, id, &wl_shm_interface, 1);
        return;
    }
}

static void global_registry_remove_handler
(
    void *data, struct wl_registry *registry, uint32_t id
)
{
    printf("Got a registry losing event for <%d>\n", id);
}

const struct wl_registry_listener registry_listener =
{
    .global = global_registry_handler,
    .global_remove = global_registry_remove_handler,
};



static void xdg_wm_base_ping( void *data, struct xdg_wm_base *wm_base, uint32_t serial)
{
    printf("Pingpong!\n");
    xdg_wm_base_pong(wm_base, serial);
}


const struct xdg_wm_base_listener xdg_listener = {
    .ping = xdg_wm_base_ping,
};


static void xdg_surface_configure(void *data,
        struct xdg_surface *xdg_surface, uint32_t serial)
{
    printf("XDG surface configure event.\n");
    xdg_surface_ack_configure(xdg_surface, serial);
}

const struct xdg_surface_listener xdg_surface_listener = {
    .configure = xdg_surface_configure,
};



static void xdg_toplevel_configure(void *data,
        struct xdg_toplevel *toplevel, int32_t width, int32_t height,
        struct wl_array *states)
{
    fprintf(stderr, "XDG toplevel configure: %dx%d\n", width, height);
}

static void xdg_toplevel_close(void *data,
        struct xdg_toplevel *toplevel)
{
    printf("Goodbye!\n");
    wl_display_disconnect(way.display);
    exit(0);
}

const struct xdg_toplevel_listener xdg_toplevel_listener = {
    .configure = xdg_toplevel_configure,
    .close = xdg_toplevel_close,
};