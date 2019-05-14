binary: riktest.o 
	$(CC) -o riktest riktest.o  

riktest.o: riktest.c
	$(CC) -c -I /home/tohas/rikor_openbmc/rikor-aztec-video/rikor-aztec/meta-facebook/meta-yosemite/recipes-yosemite/riktest/files riktest.c

clean:
	rm -f *.o binary


