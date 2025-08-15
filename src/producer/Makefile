CC = gcc
SRC = main.c \
    ../client/src/client.c

OUT = kafka_producer

$(OUT): $(SRC)
	$(CC) $(SRC) -o $(OUT)

run:
	./$(OUT) orders "My first networked message!"
	
clean:
	rm -f $(OUT)