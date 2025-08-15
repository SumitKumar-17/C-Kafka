#include "../partitioning/include/broker.h"
#include "../memory/include/mempool.h"
#include "include/logger.h"
#include <stdio.h>

int main() {
    // Create a memory pool: 10 blocks of 1KB each. Should be enough for our messages.
    MemoryPool* pool = pool_create(1024, 10);

    Broker *broker = create_broker();
    add_topic(broker, "orders", 1);

    Topic *topic = get_topic(broker, "orders");
    Partition *p0 = &topic->partitions[0];

    // Pass the pool to the logger
    append_message_to_log(p0, "Order#1001 - product X", pool);
    append_message_to_log(p0, "Order#1002 - product Y", pool);
    append_message_to_log(p0, "Order#1003 - product Z", pool);

    printf("Message written to disk based kafka log.\n");

    pool_destroy(pool);
    return 0;
}