; Here is the definition of our BSS section. Right now, we'll use
; it just to store the stack. Remember that a stack actually grows
; downwards, so we declare the size of the data before declaring
; the identifier '_sys_stack'

;TODO: Get these from include
%define MAX_CPU 128
%define STACK_SIZE 0x1000

global end_stack

ALIGN 16
SECTION .bss
	resb STACK_SIZE * MAX_CPU
end_stack:
