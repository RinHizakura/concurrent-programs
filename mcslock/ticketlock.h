#pragma once

#include <stdatomic.h>

typedef struct {
    atomic_char next;
    atomic_char now;
} ticket_lock_t;

void ticket_init (ticket_lock_t *lock);
void ticket_acquire (ticket_lock_t *lock);
void ticket_release (ticket_lock_t *lock);
