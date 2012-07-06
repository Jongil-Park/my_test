CC=/usr/local/Cavium_Networks/OCTEON-SDK-2.1/tools/bin/mips64-octeon-linux-gnu-gcc
LD=/usr/local/Cavium_Networks/OCTEON-SDK-2.1/tools/bin/mips64-octeon-linux-gnu-gcc

INCDIR = ../realtek
SRCDIR1 = ../src
SRCDIR2 = ../realtek

VPATH = $(SRCDIR1) $(SRCDIR2)

CFLAGS = -O2 -I$(INCDIR)

TARGET = realtek_spd100

SRCS = $(foreach dir, . $(SRCDIR1) $(SRCDIR2), $(wildcard $(dir)/*.c))
SRCS := $(notdir $(SRCS))

OBJS = $(SRCS:.c=.o)

all: $(TARGET)

$(TARGET) : $(OBJS)
	$(LD) $^ -o$(TARGET) $(LIBS)
	cp $(TARGET) ../../
	#mv $(TARGET) ../../

%o:%c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	-rm -rf $(OBJS)
	-rm -f ../$(TARGET)
	-rm -f ../../$(TARGET)

depend: $(SRCS)
	$(CC) -M $(CFLAGS) $^ > $@

-include depend

