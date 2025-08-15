#include "../partitioning/include/broker.h"
#include "../logger/include/logger.h"
#include "../appendLog/include/logger.h" 
#include "../memory/include/mempool.h"    

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>

#define PORT 8080
#define MAX_CLIENTS 10

Broker *g_broker;
MemoryPool* g_pool;
pthread_mutex_t g_broker_mutex = PTHREAD_MUTEX_INITIALIZER;

void *handle_client(void *socket_desc) {
    int sock = *(int*)socket_desc;
    free(socket_desc);
    
    char buffer[1024] = {0};
    read(sock, buffer, 1024);
    
    char command[16], topic_name[64];
    char *message;

    if (sscanf(buffer, "%15s %63s", command, topic_name) == 2 && strcmp(command, "PRODUCE") == 0) {
        message = strchr(buffer, ' ');
        if (message) {
            message = strchr(message + 1, ' ');
            if (message) {
                message++; 
                
                pthread_mutex_lock(&g_broker_mutex);
                
                Topic* topic = get_topic(g_broker, topic_name);
                if (!topic) {
                    add_topic(g_broker, topic_name, 1); // Auto-create topic with 1 partition
                    topic = get_topic(g_broker, topic_name);
                }

                if (topic) {
                    log_message(LOG_LEVEL_BROKER, "PRODUCE request for topic '%s': \"%s\"", topic_name, message);
                    append_message_to_log(&topic->partitions[0], message, g_pool);
                }
                
                pthread_mutex_unlock(&g_broker_mutex);
            }
        }
    } else {
        log_message(LOG_LEVEL_BROKER, "Received unknown command: %s", buffer);
    }

    close(sock);
    return NULL;
}

int main() {
    g_broker = create_broker();
    g_pool = pool_create(1024, 100); 
    
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);

    log_message(LOG_LEVEL_SYSTEM, "Starting C-Kafka Broker...");

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, MAX_CLIENTS) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    log_message(LOG_LEVEL_BROKER, "Broker listening on port %d", PORT);

    while (1) {
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
            perror("accept");
            continue;
        }
        
        pthread_t thread_id;
        int *new_sock = malloc(sizeof(int));
        *new_sock = new_socket;
        
        if (pthread_create(&thread_id, NULL, handle_client, (void*) new_sock) < 0) {
            perror("could not create thread");
            continue;
        }
        pthread_detach(thread_id);
        }

    pool_destroy(g_pool);
    return 0;
}