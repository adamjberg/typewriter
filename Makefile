main:
	gcc -o typewriter main.c `pkg-config --cflags --libs gtk+-3.0`
