# doombsp
id Software's node builder for Doom, ported to OS X

This is the original node builder used by id Software in the development of Doom. Doombsp converts a .dwd file to .wad.

### Usage

Run doombsp from the command line with the following format:

**doombsp [-draw] inmappath outdir**

* [-draw]: an optional argument that displays the node building process visually in a window. (**Not yet working**)
* inmappath: the full path to the .dwd to be processed.
* outdir: the full path to the directory where the generated wad should be placed.

#### Example

doombsp /users/tom/documents/e1m1.dwd /users/tom/documents
