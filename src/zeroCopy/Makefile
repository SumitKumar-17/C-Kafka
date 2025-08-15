CC = gcc
SRC = main.c \
    src/server.c \
    ../partitioning/src/broker.c \
    ../partitioning/src/producer.c \
    ../logger/src/logger.c

OUT = kafka_zeroCopy
LIBS = -pthread

$(OUT): $(SRC)
	$(CC) $(SRC) -o $(OUT) $(LIBS)

run:
	./$(OUT)
	
clean:
	rm -f $(OUT) orders*