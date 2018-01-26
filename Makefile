CC=/root/opt/cross/bin/i686-elf-gcc

CFLAGS= -fstrength-reduce -fomit-frame-pointer -fno-asynchronous-unwind-tables -finline-functions -nostdinc -fno-builtin  -ffixed-ebp

INCLUDES = -I/home/matthijs/OS/bran/include
 
#cpuid.h multiboot.h system_def.h system.h

SRC = render.c smp.c main.c common.c scrn.c gdt.c idt.c isrs.c irq.c timer.c kb.c vga.c math.c mouse.c vbe.c mem.c cpudet.c cpu.c acpi.c main_ap.c apic.c
#OBJ = start.o int32_beta.o cpuid.o memasm.o pit.o main.o common.o scrn.o gdt.o idt.o isrs.o irq.o timer.o kb.o vga.o math.o mouse.o vbe.o mem.o cpudet.o cpu.o acpi.o main_ap.o
OBJ = $(SRC:.c=.o)
OBJASM = multiboot_header.o start.o gdtasm.o int32_beta.o cpuid.o memasm.o ap_boot.o stack.o pit.o pic.o lock.o

multiboot_header.o: multiboot_header.asm
	nasm -f elf32 multiboot_header.asm -o multiboot_header.o
start.o: start.asm
	nasm -f elf32 start.asm -o start.o
gdtasm.o: gdtasm.asm
	nasm -f elf32 gdtasm.asm -o gdtasm.o
int32_beta.o: int32_beta.asm
	nasm -f elf32 int32_beta.asm -o int32_beta.o
cpuid.o: cpuid.asm
	nasm -f elf32 cpuid.asm -o cpuid.o
memasm.o: memasm.asm
	nasm -f elf32 memasm.asm -o memasm.o
ap_boot.o: ap_boot.asm
	nasm -f elf32 ap_boot.asm -o ap_boot.o
stack.o: stack.asm
	nasm -f elf32 stack.asm -o stack.o
pit.o: pit.asm
	nasm -f elf32 pit.asm -o pit.o
pic.o: pic.asm
	nasm -f elf32 pic.asm -o pic.o
lock.o: lock.asm
	nasm -f elf32 lock.asm -o lock.o
	
	
all: clean $(OBJASM) $(OBJ)
	#$(CC) $(CFLAGS) -o kernel $(OBJ)

.c.o:
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@
	
clean:
	rm -f *.o
	


