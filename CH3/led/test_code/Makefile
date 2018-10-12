CC=$(CROSS_COMPILE)gcc
#CC=gcc
file = ledtest
ALL:
	$(CC) -o $(file) $(file).c

install:
	sudo cp $(file) $(INSTALLDIR)

clean:
	rm -rf $(file)

