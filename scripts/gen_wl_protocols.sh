#!/bin/sh
# Generate wayland xdg shell protocol

printf "Generating wayland xdg shell protocol in $1\n"

if ! test -d /usr/share/wayland-protocols
then
    printf "Wayland protocols not found. Install them with 'sudo apt install wayland-protocols'\n"
	exit 1
fi

if ! test -d $1/wl_protocols
then
	mkdir $1/wl_protocols
	wayland-scanner client-header "/usr/share/wayland-protocols/stable/xdg-shell/xdg-shell.xml" "$1/wl_protocols/wayland_xdg_shell.h"
	wayland-scanner private-code "/usr/share/wayland-protocols/stable/xdg-shell/xdg-shell.xml" "$1/wl_protocols/wayland_xdg_shell.c"
fi

# Imported from project lv_port_linux