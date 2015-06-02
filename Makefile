CC=gcc
CFLAGS=$(shell pkg-config --cflags --libs webkitgtk-3.0)

vpath %.c src

.PHONY: all clean

all: bin/solar-webkitgtk

clean:
	rm -rf bin/*

bin/solar-webkitgtk: solar-webkitgtk.c
	gcc $^ -o $@ $(CFLAGS)
