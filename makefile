all:
	nasm -o temp\boot.bin src\boot.asm
	nasm -o temp\Entry.bin src\Entry.asm
	i686-elf-gcc -c -masm=intel -ffreestanding -o temp\kernel.o src\32kernel\kernel.c
	i686-elf-gcc -c -masm=intel -ffreestanding -o temp\interrupt.o src\32kernel\interrupt.c
	i686-elf-gcc -c -ffreestanding -o temp\keymapping.o src\32kernel\keyboard\keymapping.c
	i686-elf-ld -Ttext 0x10000 -e main temp\kernel.o temp\interrupt.o temp\keymapping.o -o temp\ckernel.elf
	i686-elf-objcopy -j .text -j .data -j .rodata -j .bss -S -O binary temp\ckernel.elf temp\ckernel.bin
	copy temp\boot.bin + temp\Entry.bin + temp\ckernel.bin /b Disk.img
	qemu-system-i386 -s -L . -m 128 -fda Disk.img -M pc