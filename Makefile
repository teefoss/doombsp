TARGET=doombsp
CC=clang
OBJ=Storage.o 		\
	Wadfile.o 		\
	buildbsp.o 		\
	cmdlib.o 		\
	doombsp.o 		\
	doomload.o 		\
	drawing.o		\
	saveblocks.o	\
	savebsp.o 		\
	saveconnect.o 	\
	savesectors.o 	\

$(TARGET): $(OBJ)
	$(CC) $^ -o $@ -lSDL2 -framework Foundation

%.o: %.c %.m
	$(CC) -c $< -o $@

clean:
	@rm -rf *.o $(TARGET)
