C_SOURCES = $(wildcard kernel/*.c drivers/*.c cpu/*.c libc/*.c boot/*.c )
HEADERS = $(wildcard include/*.h cpu/*.h libc/*.h )
# 文件扩展替换
OBJ = ${C_SOURCES:.c=.o} 
OBJ += cpu/interrupt.o 


# 交叉编译环境
CC = /usr/local/i386elfgcc/bin/i386-elf-gcc
AS = /usr/local/i385elfgcc/bin/i386-elf-as
GDB = gdb
# -g: 调试  -std=gnu99使用gnu99 扩展
CFLAGS =  -I ./include -fno-pic -g -m32 -ffreestanding -Wall -Wextra -Werror -fno-exceptions -std=gnu99  


%.o: %.c ${HEADERS}
	${CC} ${CFLAGS}  -c $< -o $@

init: kernel/init.S
	$(CC) $(CFLAGS) -nostdinc  -c $^ -o init.o
	i386-elf-ld  -N -e start -Ttext 0 -o init.out init.o
	i386-elf-objcopy  -S -O binary init.out $@ 
# '--oformat binary' 去除所有标签  kernel.ld  规定链接规则
kernel.bin: boot/kernel_entry.o init kernel/swtch.o  ${OBJ}
	i386-elf-ld -o $@ -T  kernel.ld  boot/kernel_entry.o ${OBJ} kernel/swtch.o -b binary init  --oformat binary
echo:
	echo ${OBJ}
	echo ${C_SOURCES}

# 调试
kernel.elf: boot/kernel_entry.o  kernel/swtch.o init ${OBJ}
	i386-elf-ld -T kernel.ld  -o $@  boot/kernel_entry.o ${OBJ} kernel/swtch.o -b binary init    

boot/loader.elf: boot/loader.o boot/start.o
	i386-elf-ld  -Ttext 0x7c00  -e bootstart -o $@ $^

QEMUPARAMS = -drive file=os.img,index=0,media=disk,format=raw  -m 512 -drive file=fs.img,index=1,media=disk,format=raw 
#运行
run: os.img fs.img
	qemu-system-i386  ${QEMUPARAMS} 

run-nox: os.img fs.img
	qemu-system-i386  -nographic ${QEMUPARAMS} 

debug-nox: os.img kernel.elf   boot/loader.elf fs.img
	qemu-system-i386   -${QEMUPARAMS}  nographic  -drive -gdb tcp::1245 -S -D int,cpu_reset 
	
debug: os.img kernel.elf   boot/loader.elf fs.img
	qemu-system-i386   -${QEMUPARAMS}  gdb tcp::1245 -S -D int,cpu_reset 



%.o: %.asm
	nasm $< -f elf -o $@
	
boot/kernel_entry.o: boot/kernel_entry.S
	${CC}  ${CFLAGS} -c  $< -o $@

kernel/swtch.o: kernel/swtch.S
	${CC}  ${CFLAGS} -c  $< -o $@

%.bin: %.asm
	nasm $< -f bin -o $@


boot/loader.o: boot/start.S
	nasm $< -f elf -o $@
boot/start.o: boot/start.c
	${CC} ${CFLAGS} -O2 -I ./include -c $< -o $@

#  bootloader  512字节 magic code：0xaa55
boot/loader.bin: boot/loader.o boot/start.o  
	i386-elf-ld  -Ttext 0x7c00 -e bootstart  -S   -o bootloader.o $^
	i386-elf-objcopy -S -O binary -j .text  bootloader.o boot/bootloader
	dd if=/dev/zero of=$@ bs=1 count=510
	dd if=boot/bootloader of=$@ conv=notrunc
	echo -e -n "\x55\xaa" >>$@
	rm boot/bootloader

#制作系统镜像
os.img: boot/loader.bin kernel.elf
	dd if=/dev/zero of=os.img count=10000
	dd if=boot/loader.bin of=$@ conv=notrunc
	dd if=kernel.elf of=$@  seek=1 conv=notrunc

#制作文件系统
fs.img: mkfs
	echo "fs test" > fs.txt
	./mkfs  $@  fs.txt 

mkfs: utils/mkfs.c 
	gcc -I include -Werror -Wall  -o mkfs $<
clean:
	rm -rf *.img *.dis *.o  *.elf init init.out fs.txt
	rm -rf kernel/*.o boot/*.bin drivers/*.o boot/*.o cpu/*.o boot/*.elf  libc/*.o
