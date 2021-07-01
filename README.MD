# KingKernel?
x86 Architecture MicroKernel 만드는거 연습하는거;;

아 바이오스 기능은 보호모드에서 못쓴데;;

## 환경
macOS에서 진행하였기 때문에, 타 OS는 조금 다른 환경을 구축해야 할 수 있습니다 <br>
gcc-x86_64-linux-gnu, binutils-x86_64-linux-gnu 를 빌드하여 사용하였습니다.<br>
### 링커스크립트
커스텀 링크스크립터를 사용했는데, 일반적으로 GCC빌드 이후에는 파일내에 TEXT, DATA, BSS 이외에도 다른 부분이 포함 되어 있습니다. 그러기에, 링커스크립트와 유틸 ld를 이용하여 바이너리 파일의 TEXT, DATA, BSS 부분만 추출하여 사용합니다.<br>
### 실제로 컴파일은...
링커스크립트로 하려고 했으나.. binutils내의 objcopy를 이용하면 링커스크립트를 사용하지 않고도 손쉽게 TEXT, DATA, BSS를 추출하여 사용할 수 있습니다.<br>
아래는 루트디렉토리에 작성된 makefile입니다.
  i586-pc-linux-gcc -c -masm=intel -ffreestanding -o temp/kernel.o src/32kernel/kernel.c
	i586-pc-linux-gcc -c -masm=intel -ffreestanding -o temp/interrupt.o src/32kernel/interrupt.c
	i586-pc-linux-gcc -c -ffreestanding -o temp/keymapping.o src/32kernel/keyboard/keymapping.c
	i586-pc-linux-ld -Ttext 0x10000 -e main temp/kernel.o temp/interrupt.o temp/keymapping.o -o temp/ckernel.elf
	i586-pc-linux-objcopy -j .text -j .data -j .rodata -j .bss -S -O binary temp/ckernel.elf temp/ckernel.bin
gcc를 컴파일할때 -ffreestanding는 표준 라이브러리를 안 가져다 쓸 때 사용하며, -masm=intel은 C코드 내부에 작성된 어셈블리어를 intel방식으로 읽어들이도록 하겠다 라는 의미<br>
ld를 보면 -Ttext 0x10000은 프로그램(커널)이 0x10000에서 시작되도록 하는것이며(물리 주소로 알고 있습니다. 아직 MMU를 거치지 않음) gcc로 컴파일 한 오브젝트 파일을 합쳐서 elf파일로 출력합니다.(참고로 linux환경의 크로스 컴파일러이기 때문에 파일 구조는 ELF형태로 출력 된다.)<br>
이후 필요한 TEXT, DATA, BSS를 추출해내기 위해 objcopy를 이용하여 추출하고, ckernel.bin에 모다서 놓습니다.
