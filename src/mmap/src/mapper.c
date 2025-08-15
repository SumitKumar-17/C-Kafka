#include "../include/mapper.h"
#include "../../compression/include/compressor.h"
#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// Helper function to ensure all bytes are read
ssize_t read_fully(int fd, void *buf, size_t count) {
    size_t bytes_read = 0;
    while (bytes_read < count) {
        ssize_t result = read(fd, (char *)buf + bytes_read, count - bytes_read);
        if (result < 0) {
            return -1; // Error
        }
        if (result == 0) {
            break; // End of file
        }
        bytes_read += result;
    }
    return bytes_read;
}

void read_log_with_mmap(char *filepath) {
    int fd = open(filepath, O_RDONLY);
    if (fd < 0) {
        perror("open error");
        return;
    }

    printf("Readfile using file I/O (decompressed):\n");

    while (1) {
        uint32_t compressed_size;
        ssize_t bytes_read = read_fully(fd, &compressed_size, sizeof(compressed_size));

        if (bytes_read == 0) {
            break; // End of file, clean exit
        }
        if (bytes_read < sizeof(compressed_size)) {
            fprintf(stderr, "Incomplete read for message size header\n");
            break;
        }

        Bytef *compressed_buffer = malloc(compressed_size);
        if (!compressed_buffer) {
            perror("Failed to allocate memory for compressed buffer");
            break;
        }

        bytes_read = read_fully(fd, compressed_buffer, compressed_size);
        if (bytes_read < compressed_size) {
            fprintf(stderr, "Incomplete read for message payload\n");
            free(compressed_buffer);
            break;
        }

        uLongf decompressed_size = compressed_size * 10; // A safe estimate
        Bytef *decompressed_buffer = malloc(decompressed_size);
        if (!decompressed_buffer) {
            perror("Failed to allocate memory for decompression");
            free(compressed_buffer);
            break;
        }

        if (decompress_message(decompressed_buffer, &decompressed_size, compressed_buffer, compressed_size) < 0) {
            fprintf(stderr, "Failed to decompress message chunk\n");
            free(decompressed_buffer);
            free(compressed_buffer);
            break;
        }
        
        fwrite(decompressed_buffer, 1, decompressed_size, stdout);
        printf("\n");

        free(decompressed_buffer);
        free(compressed_buffer);
    }

    close(fd);
}