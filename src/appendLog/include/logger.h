#pragma once
#include "../../partitioning/include/topic.h"
#include "../../memory/include/mempool.h"

// Update the function signature to accept the memory pool
void append_message_to_log(Partition *p, char *message, MemoryPool* pool);