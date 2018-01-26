[BITS 32]

global acquireLock
global releaseLock


SECTION .text

_spin_wait:
    test dword [esp], 1      ;Is the lock free?
    jnz _spin_wait           ;no, wait

acquireLock:
    lock bts [esp], 0        ;Attempt to acquire the lock (in case lock is uncontended)
    jc _spin_wait            ;Spin if locked ( organize code such that conditional jumps are typically not taken )
    ret                      ;Lock obtained

releaseLock:
    mov dword [esp], 0
    ret
