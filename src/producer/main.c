#include "../client/include/client.h"
#include <stdio.h>
#include <string.h>

int main(int argc, char const *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <topic> <message>\n", argv[0]);
        return 1;
    }
    
    int sock = connect_to_broker("127.0.0.1", 8080);
    if (sock < 0) return 1;

    char buffer[1024];
    snprintf(buffer, sizeof(buffer), "PRODUCE %s %s", argv[1], argv[2]);

    printf("Producer connected. Sending command: \"%s\"\n", buffer);
    send_message_to_broker(sock, buffer);
    close_broker_connection(sock);
    printf("Message sent and connection closed.\n");
    return 0;
}