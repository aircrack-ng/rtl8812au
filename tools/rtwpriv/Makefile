########################################################
#  MakeFile for Realtek Wireless LAN Private TOOL.     #
#                                                      #
########################################################

CC      ?= gcc
CCFLAGS += -Wall
#CCFLAGS += -DDEBUG
#BUILDFLAGS := -static

target	:= rtwpriv
object  := $(target).o

GENOBJ = $(CC) $(CCFLAGS) -c


all: $(target)

$(target): $(object)
	$(CC) $(CCFLAGS) $(BUILDFLAGS) $(LIB) $(object) -o $@

%.o: %.c
	$(GENOBJ) $<

.PHONY: clean
clean:
	rm -f *.o *~ $(target)
