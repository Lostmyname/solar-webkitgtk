CC=gcc
CFLAGS=$(shell PKG_CONFIG_PATH=/opt/webkitgti-2.4.9/lib/pkgconfig pkg-config --cflags --libs webkit2gtk-3.0)

vpath %.c src

.PHONY: all clean

all: bin/solar-webkitgtk

clean:
	rm -rf bin/*

bin/solar-webkitgtk: solar-webkitgtk.c
	gcc $^ -o $@ $(CFLAGS)
