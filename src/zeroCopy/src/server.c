#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/stat.h>

#ifdef __APPLE__
#include <sys/uio.h> // macOS sendfile()
#else
#include <sys/sendfile.h> // Linux sendfile()
#endif

#include "../include/server.h"

void start_server(char* filepath, int port) {
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("socket");
        exit(1);
    }

    struct sockaddr_in addr = {
        .sin_family = AF_INET,
        .sin_addr.s_addr = INADDR_ANY,
        .sin_port = htons(port)
    };

    if (bind(server_fd, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        perror("bind");
        close(server_fd);
        exit(1);
    }

    if (listen(server_fd, 1) < 0) {
        perror("listen");
        close(server_fd);
        exit(1);
    }

    printf("Server Listening at Port %d\n", port);

    while (1) {
        int client_fd = accept(server_fd, NULL, NULL);
        if (client_fd < 0) {
            perror("accept");
            continue;
        }

        printf("Client connected, sending the file %s\n", filepath);

        int file_fd = open(filepath, O_RDONLY);
        if (file_fd < 0) {
            perror("open");
            close(client_fd);
            continue;
        }

        struct stat st;
        if (fstat(file_fd, &st) < 0) {
            perror("fstat");
            close(file_fd);
            close(client_fd);
            continue;
        }

#ifdef __APPLE__
        off_t len = st.st_size;
        if (sendfile(file_fd, client_fd, 0, &len, NULL, 0) < 0) {
            perror("sendfile (macOS)");
        }
#else
        off_t offset = 0;
        if (sendfile(client_fd, file_fd, &offset, st.st_size) < 0) {
            perror("sendfile (Linux)");
        }
#endif

        printf("Sent %ld bytes from %s using zero-copy\n", (long)st.st_size, filepath);

        close(file_fd);
        close(client_fd);
    }
}
