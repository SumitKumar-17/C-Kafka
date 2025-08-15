CC = gcc
SRC = main.c \
    src/broker.c \
    src/consumer.c \
    src/producer.c \
    ../logger/src/logger.c

OUT = kafka_sim
LIBS = -pthread

$(OUT): $(SRC)
	$(CC) $(SRC) -o $(OUT) $(LIBS)

run:
	./$(OUT)
	
clean:
	rm -f $(OUT)