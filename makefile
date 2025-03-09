output: main.c handlers.c context.c
	gcc -Wall main.c handlers.c context.c protocols/xdg-shell.c -l wayland-client -o output

.PHONY: clean
clean:
	rm output