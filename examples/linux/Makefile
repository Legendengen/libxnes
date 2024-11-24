#
# Makefile for application
#

CROSS_COMPILE	?=/home/deng/9-t527-tina-aiot-v1.2/tina/out/t527/demo_linux_aiot/buildroot/buildroot/host/bin/aarch64-none-linux-gnu-

AS			:=	$(CROSS_COMPILE)gcc -x assembler-with-cpp
CC			:=	$(CROSS_COMPILE)gcc
CXX			:=	$(CROSS_COMPILE)g++
LD			:=	$(CROSS_COMPILE)ld
AR			:=	$(CROSS_COMPILE)ar
SZ			:=	$(CROSS_COMPILE)size
OC			:=	$(CROSS_COMPILE)objcopy
OD			:=	$(CROSS_COMPILE)objdump
STRIP		:=	$(CROSS_COMPILE)strip
MKDIR		:=	mkdir -p
RM			:=	rm -fr

ASFLAGS		:=	-g -ggdb -Wall -O3 -ffunction-sections -fdata-sections -ffreestanding -std=gnu99
CFLAGS		:=	-g -ggdb -Wall -O3 -ffunction-sections -fdata-sections -ffreestanding -std=gnu99
CXXFLAGS	:=	-g -ggdb -Wall -O3 -ffunction-sections -fdata-sections -ffreestanding
LDFLAGS		:=
OCFLAGS		:=	-v -O binary
ODFLAGS		:=
MCFLAGS		:=

LIBDIRS		:=	/home/deng/9-t527-tina-aiot-v1.2/tina/out/t527/demo_linux_aiot/buildroot/buildroot/host/aarch64-buildroot-linux-gnu/sysroot/usr/lib
LIBS 		:=	-lSDL2
INCDIRS		:=	/home/deng/9-t527-tina-aiot-v1.2/tina/out/t527/demo_linux_aiot/buildroot/buildroot/host/aarch64-buildroot-linux-gnu/sysroot/usr/include
SRCDIRS		:=	.

#CFLAGS		+=	$(shell sdl2-config --cflags) -D_GNU_SOURCE
#CXXFLAGS	+=	$(shell sdl2-config --cflags) -D_GNU_SOURCE
LIBDIRS		+=
#LIBS		+=	$(shell sdl2-config --libs)
sinclude	../../src/xnes.mk

OUTPUT		:=	output
NAME		:=	$(OUTPUT)/xnes

INCDIRS		:=	$(patsubst %, -I %, $(sort $(INCDIRS)))
LIBDIRS		:=	$(patsubst %, -L %, $(sort $(LIBDIRS)))
SRCDIRS		:=	$(sort $(SRCDIRS))
OBJDIRS		:=	$(patsubst %, .obj/%, $(SRCDIRS))

SFILES		:= $(foreach dir, $(SRCDIRS), $(wildcard $(dir)/*.S))
CFILES		:= $(foreach dir, $(SRCDIRS), $(wildcard $(dir)/*.c))
CPPFILES	:= $(foreach dir, $(SRCDIRS), $(wildcard $(dir)/*.cpp))

SDEPS		:= $(patsubst %, .obj/%, $(SFILES:.S=.o.d))
CDEPS		:= $(patsubst %, .obj/%, $(CFILES:.c=.o.d))
CPPDEPS		:= $(patsubst %, .obj/%, $(CPPFILES:.cpp=.o.d))
DEPS		:= $(SDEPS) $(CDEPS) $(CPPDEPS)

SOBJS		:= $(patsubst %, .obj/%, $(SFILES:.S=.o))
COBJS		:= $(patsubst %, .obj/%, $(CFILES:.c=.o))
CPPOBJS		:= $(patsubst %, .obj/%, $(CPPFILES:.cpp=.o)) 
OBJS		:= $(SOBJS) $(COBJS) $(CPPOBJS)

VPATH		:= $(OBJDIRS)

.PHONY: all clean

all : $(NAME)

$(NAME) : $(OBJS)
	@echo [LD] Linking $@
	@$(CC) $(LDFLAGS) $(LIBDIRS) $^ -o $@ $(LIBS)

$(SOBJS) : .obj/%.o : %.S
	@echo [AS] $<
	@$(AS) $(ASFLAGS) -MD -MP -MF $@.d $(INCDIRS) -c $< -o $@

$(COBJS) : .obj/%.o : %.c
	@echo [CC] $<
	@$(CC) $(CFLAGS) -MD -MP -MF $@.d $(INCDIRS) -c $< -o $@

$(CPPOBJS) : .obj/%.o : %.cpp
	@echo [CXX] $<
	@$(CXX) $(CXXFLAGS) -MD -MP -MF $@.d $(INCDIRS) -c $< -o $@

clean:
	@$(RM) .obj $(OUTPUT)

sinclude $(shell $(MKDIR) $(OBJDIRS) $(OUTPUT)) $(DEPS)
