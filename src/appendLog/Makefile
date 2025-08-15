CC = gcc
SRC = main.c \
    src/logger.c \
    ../partitioning/src/broker.c \
    ../compression/src/compressor.c \
    ../logger/src/logger.c \
    ../memory/src/mempool.c

OUT = kafka_appendLog
LIBS = -lz

$(OUT): $(SRC)
	$(CC) $(SRC) -o $(OUT) $(LIBS)

run:
	./$(OUT)
	
clean:
	rm -f $(OUT) 
	rm -rf temp