global _acquireLock
global _releaseLock

SECTION .text

[BITS 32]

_acquireLock:
cli
	push ebx
	push ecx
	mov ebx, [esp + 4]					; ebx holds lock var addr on stack
	xor ecx, ecx
	inc ecx
__spin_wait:
	xor eax, eax
	lock cmpxchg [ebx], ecx				; reason to use assembly
	jnz __spin_wait
	pop ecx
	pop ebx
sti
	ret

_releaseLock:
cli
	push ebx
	push ecx
	mov ebx, [esp + 4]					; ebx holds lock var addr on stack
	xor ecx, ecx
__spin_wait2:
	xor eax, eax
	inc eax
	lock cmpxchg [ebx], ecx				; reason to use assembly
	jnz __spin_wait2
	pop ecx
	pop ebx
sti
	ret





