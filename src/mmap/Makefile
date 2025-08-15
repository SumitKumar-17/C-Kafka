CC = gcc
SRC = main.c \
src/mapper.c \
../compression/src/compressor.c

OUT = kafka_mmap
LIBS = -lz

$(OUT): $(SRC)
	$(CC) $(SRC) -o $(OUT) $(LIBS)

run:
	./$(OUT)
	
clean:
	rm -f $(OUT)