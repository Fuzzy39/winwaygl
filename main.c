#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <wayland-client.h>
#include "xdg-shell.h"


// the display represents our connection to the compositor
struct wl_display *display = NULL;

// the registry is the means through which we can access important wayland objects.
struct wl_registry *registry = NULL;

// a reference to the compositor.
struct wl_compositor* compositor = NULL;

// a surface is a rectangle of pixels, I guess?
struct wl_surface *surface;
struct wl_shell *shell = NULL;
struct wl_shell_surface *shell_surface;

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
        compositor = wl_registry_bind(registry, id, &wl_compositor_interface, 1);
        return;
    }
    if (strcmp(interface, "wl_shell") == 0)
    {
        shell = wl_registry_bind(registry, id, &wl_shell_interface, 1);
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

const static struct wl_registry_listener registry_listener =
{
    global_registry_handler,
    global_registry_remove_handler,
};

int main(int argc, char *argv[])
{
    display = wl_display_connect(NULL);
    if (display == NULL) {
        printf("Couldn't connect to display server.[Step 1]\n");
        exit(1);
    }

    registry = wl_display_get_registry(display);
    wl_registry_add_listener(registry, &registry_listener, NULL);

    int events = wl_display_dispatch(display);
    printf("Got %d events.\n", events);
    wl_display_roundtrip(display);

    if(compositor==NULL)
    {
        printf("Failed to get a reference to the compositor. [Step 2]\n");
        exit(1);
    }

    surface = wl_compositor_create_surface(compositor);
    if(surface == NULL)
    {
        printf("Failed to create a surface. [Step 3]\n");
        exit(1);
    }

    if(shell == NULL)
    {
        printf("Couldn't find a wayland shell. [Step 4]\n");
        exit(1);
    }



    wl_display_disconnect(display);

    return 0;
}