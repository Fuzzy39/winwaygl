#include <wayland-client.h>
#include "protocols/xdg-shell.h"

#include "wayland.h"


int main(int argc, char *argv[])
{
    way.display = wl_display_connect(NULL);
    if (way.display == NULL) {
        printf("Couldn't connect to display server.[Step 1]\n");
        exit(1);
    }

    way.registry = wl_display_get_registry(way.display);
    wl_registry_add_listener(way.registry, &registry_listener, NULL);

    int events = wl_display_dispatch(way.display);
    printf("Got %d events.\n", events);
    wl_display_roundtrip(way.display);

    //Add listeners
 

    if(way.compositor==NULL)
    {
        printf("Failed to get a reference to the compositor. [Step 2]\n");
        exit(1);
    }

    way.surface = wl_compositor_create_surface(way.compositor);
    if(way.surface == NULL)
    {
        printf("Failed to create a surface. [Step 3]\n");
        exit(1);
    }

  
    // gets here
    if(way.xdg_wm_base == NULL)
    {
        printf("Couldn't get xdg_wm_base [Step 4]\n");
        exit(1);
    }
    xdg_wm_base_add_listener(way.xdg_wm_base, &xdg_listener, NULL);


  



    // Create a XDG surface based our surface.
    way.xdg_surface = xdg_wm_base_get_xdg_surface(way.xdg_wm_base, way.surface);
    printf("At this point I'm pretty confused\n");

    if(way.xdg_surface == NULL)
    {
        printf("Failed to make an xdg_surface. [Step 5]\n");
        exit(1);
    }
     
    xdg_surface_add_listener(way.xdg_surface, &xdg_surface_listener, NULL);
    

    // Not here
    // Set toplevel role to our XDG surface.
    way.xdg_toplevel = xdg_surface_get_toplevel(way.xdg_surface);
    if(way.xdg_toplevel == NULL)
    {
        printf("Failed to assign the xdg_toplevel role to our service. [Step 6]\n");
        exit(1);
    }



    // Does not get here
    xdg_toplevel_add_listener(way.xdg_toplevel, &xdg_toplevel_listener, NULL);
    xdg_toplevel_set_title(way.xdg_toplevel, "Hello Wayland!");

    printf("We HAVE to have gotten here\n");
    //create_shm_pool();





    // Apply our changes.
    wl_surface_commit(way.surface);
    
    // main loop
    while (wl_display_dispatch(way.display) != -1);

    wl_display_disconnect(way.display);

    return 0;
}