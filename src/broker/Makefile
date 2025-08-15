CC = gcc
# Broker now needs code from many modules to function
SRC = main.c \
    ../partitioning/src/broker.c \
    ../appendLog/src/logger.c \
    ../logger/src/logger.c \
    ../compression/src/compressor.c \
    ../memory/src/mempool.c

OUT = kafka_broker
# Link against pthreads for threading and zlib for compression
LIBS = -pthread -lz

$(OUT): $(SRC)
	$(CC) -I../appendLog/include -I../memory/include $(SRC) -o $(OUT) $(LIBS)

run:
	./$(OUT)
	
clean:
	rm -f $(OUT)
	rm -rf temp