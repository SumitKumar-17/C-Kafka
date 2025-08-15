#include "../include/logger.h"
#include "../../compression/include/compressor.h"
#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>

void append_message_to_log(Partition *p, char *message, MemoryPool* pool) {
    // ... (file setup code is the same) ...
    char log_file[300], index_file[300], timeindex_file[300];
    mkdir("temp", 0755);
    mkdir("temp/order1", 0755);

    uint64_t base_offset = 0;

    snprintf(log_file, sizeof(log_file), "%s/%020llu.log", "temp/order1", base_offset);
    snprintf(index_file, sizeof(index_file), "%s/%020llu.index", "temp/order1", base_offset);
    snprintf(timeindex_file, sizeof(timeindex_file), "%s/%020llu.timeindex", "temp/order1", base_offset);

    int log_fd = open(log_file, O_WRONLY | O_APPEND | O_CREAT, 0644);
    int idx_fd = open(index_file, O_WRONLY | O_APPEND | O_CREAT, 0644);
    int time_fd = open(timeindex_file, O_WRONLY | O_APPEND | O_CREAT, 0644);

    if (log_fd < 0 || idx_fd < 0 || time_fd < 0) {
        perror("open error");
        return;
    }

    uLong sourceLen = strlen(message);
    uLongf destLen = compressBound(sourceLen);
    
    // --- Use the Memory Pool ---
    Bytef *compressed_message = pool_alloc(pool);
    // --- Instead of: Bytef *compressed_message = malloc(destLen); ---

    if (!compressed_message) {
        fprintf(stderr, "Failed to allocate memory from pool for compressed message\n");
        close(log_fd);
        close(idx_fd);
        close(time_fd);
        return;
    }

    if (compress_message(compressed_message, &destLen, (const Bytef *)message, sourceLen) < 0) {
        fprintf(stderr, "Failed to compress message\n");
        // We don't free the block, the pool will be destroyed later.
        close(log_fd);
        close(idx_fd);
        close(time_fd);
        return;
    }

    off_t offset = lseek(log_fd, 0, SEEK_END);
    
    uint32_t compressed_size = (uint32_t)destLen;
    write(log_fd, &compressed_size, sizeof(compressed_size));
    write(log_fd, compressed_message, destLen);
    
    // --- We no longer need to free the memory individually ---
    // free(compressed_message);

    dprintf(idx_fd, "%llu,%lld\n", p->log_offset, (long long)offset);
    time_t now = time(NULL);
    dprintf(time_fd, "%ld,%lld\n", now, (long long)offset);

    fsync(log_fd);
    fsync(idx_fd);
    fsync(time_fd);

    close(log_fd);
    close(idx_fd);
    close(time_fd);

    p->log_offset++;
}