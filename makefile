output: main.c handlers.c context.c shm.c wayland.h
	gcc -Wall main.c handlers.c context.c shm.c protocols/xdg-shell.c protocols/xdg-decoration-unstable-v1.c -l wayland-client -o output

.PHONY: clean
clean:
	rm output