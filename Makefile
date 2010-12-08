CFLAGS=`sdl-config --libs --cflags` -lSDL_mixer -Wall -g
PREFIX=$$HOME
rdnt: main.o
	$(CC) -o $@ $< $(CFLAGS)
clean:
	rm -f *.o rdnt
install: rdnt
	cp $< $(PREFIX)/bin

