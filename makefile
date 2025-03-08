output: 
	gcc -Wall main.c xdg-shell.c -l wayland-client -o output

.PHONY: clean
clean:
	rm output