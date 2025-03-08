#include <stdio.h>
#include <stdlib.h>

#include <wayland-client.h>

struct wl_display *display = NULL;
struct wl_registry *registry = NULL;

static void global_registry_handler
( 
    void *data, 
    struct wl_registry *registry, 
    uint32_t id, 
    const char *interface, 
    uint32_t version
)
{
    printf("Got a registry event for <%s>, id: %d, version: %d.\n",
        interface, id, version);
}

static void global_registry_remove_handler
(
    void *data, struct wl_registry *registry, uint32_t id
)
{
    printf("Got a registry losing event for <%d>\n", id);
}

const static struct wl_registry_listener registry_listener 
=
{
    .global = global_registry_handler,
    .global_remove = global_registry_remove_handler,
};

int main(int argc, char *argv[])
{
    display = wl_display_connect(NULL);
    if (display == NULL) {
        printf("Couldn't connect to display server.\n");
        exit(1);
    }

    registry = wl_display_get_registry(display);
    wl_registry_add_listener(registry, &registry_listener, NULL);

    int events = wl_display_dispatch(display);
    printf("Got %d events.\n", events);
    //wl_display_roundtrip(display);

    wl_display_disconnect(display);

    return 0;
}