CC = gcc
SRC = main.c \
../partitioning/src/broker.c \
../compression/src/compressor.c \
src/logger.c

OUT = kafka_appendLog
LIBS = -lz

$(OUT): $(SRC)
	$(CC) $(SRC) -o $(OUT) $(LIBS)

run:
	./$(OUT)
	
clean:
	rm -f $(OUT) 
	rm -rf temp