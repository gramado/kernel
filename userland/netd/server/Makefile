# NETD.BIN
# NET Control (Server-side)

VERSION = 0
PATCHLEVEL = 1
SUBLEVEL = 0
EXTRAVERSION = 
#NAME = 

CFLAGS = -m64 \
	-std=gnu89 \
	-nodefaultlibs \
	-nostdinc \
	-nostdlib \
	-static \
	-fgnu89-inline \
	-ffreestanding \
	-fno-builtin \
	-fno-pie \
	-fleading-underscore \
	-fno-stack-protector \
	-s \
	-Werror=strict-prototypes    
	

# rtl
LIBC    = ../../libs/rtl/include/
LIBCOBJ = ../../libs/rtl/obj

GWS_INCLUDES  = include/    

#RENDERER = renderer/include/  
#CMDLINE = cmdline/include/   
#TERMINAL  = terminal/include/   

# Objects
myObjects = crt0.o main.o rtl.o globals.o \
ctype.o stdio.o stdlib.o string.o conio.o socket.o signal.o unistd.o \
connect.o decode.o client.o \
termios.o ioctl.o fcntl.o    

.PHONY: all all-c gws-link finalize clean

all: main.o all-c gws-link finalize clean
	@echo "Ok?"
main.o:
	gcc  -c  main.c  $(CFLAGS) -I $(LIBC) -I $(GWS_INCLUDES) -I . -o main.o
all-c:
	gcc -c globals.c $(CFLAGS) -I $(LIBC) -I $(GWS_INCLUDES) -I . -o globals.o
	gcc -c connect.c $(CFLAGS) -I $(LIBC) -I $(GWS_INCLUDES) -I . -o connect.o
	gcc -c decode.c  $(CFLAGS) -I $(LIBC) -I $(GWS_INCLUDES) -I . -o decode.o
	gcc -c client.c  $(CFLAGS) -I $(LIBC) -I $(GWS_INCLUDES) -I . -o client.o
	
# Grab the objects

	cp $(LIBCOBJ)/crt0.o .
	cp $(LIBCOBJ)/rtl.o .

	cp $(LIBCOBJ)/ctype.o .
	cp $(LIBCOBJ)/stdio.o .
	cp $(LIBCOBJ)/stdlib.o .
	cp $(LIBCOBJ)/string.o .
	cp $(LIBCOBJ)/conio.o .
	cp $(LIBCOBJ)/unistd.o .
	cp $(LIBCOBJ)/socket.o .
	cp $(LIBCOBJ)/signal.o .
	cp $(LIBCOBJ)/termios.o .
	cp $(LIBCOBJ)/ioctl.o .
	cp $(LIBCOBJ)/fcntl.o .

gws-link:
	ld -m elf_x86_64 -T ../../link.ld -o NETD.BIN $(myObjects)    
finalize:
	cp NETD.BIN ../../bin
clean:
	-rm *.o
	-rm NETD.BIN 
	
