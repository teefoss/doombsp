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
	nextstep.o		\

$(TARGET): $(OBJ)
	$(CC) $^ -o $@ -lSDL2

%.o: %.cpp
	$(CC) -g -c $< -o $@

clean:
	@rm -rf *.o $(TARGET)
