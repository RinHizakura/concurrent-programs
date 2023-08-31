#include "ticketlock.h"
#include "macros.h"

void ticket_init (ticket_lock_t *lock) {
    atomic_init(&lock->now, 0);
    atomic_init(&lock->next, 0);
}

void ticket_acquire (ticket_lock_t *lock) {

    char my_ticket = atomic_fetch_add_explicit(&lock->next, 1, memory_order_relaxed);

    while (atomic_load_explicit(&lock->now, memory_order_acquire) != my_ticket)
        spin_wait();
}

void ticket_release (ticket_lock_t *lock) {
    atomic_fetch_add_explicit(&lock->now, 1, memory_order_release);
}
