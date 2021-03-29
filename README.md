# doombsp
id Software's node builder for Doom, ported to macOS

This is the original node builder used by id Software in the development of Doom. doombsp converts a dwd file to a wad.

doombsp runs in the terminal, but if the `-draw` option is specified, it opens a window and displays the build process. This was originally achieved used Nextstep's AppKit. To convert to modern Cocoa would be somewhat complicated, so SDL2 is used.

## Building

Get SDL2 if you don't have it. The simplest way to do that is likely with Homebrew: `brew install sdl2`.

Clone the repo, then type `make`.
