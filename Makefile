ifneq ($(KERNELRELEASE),)
# kbuild part of makefile

obj-m += hello1.o
obj-m += hello2.o

# headers in ./include so we can do: #include "hello1.h"
ccflags-y += -I$(src)/include

# debug-friendly build (appendix1 style)
ccflags-y += -g

else
# normal makefile

KDIR ?= /lib/modules/`uname -r`/build

default:
	$(MAKE) -C $(KDIR) M=$$PWD
	# keep unstripped copies for debugging
	cp -f hello1.ko hello1.ko.unstripped
	cp -f hello2.ko hello2.ko.unstripped
	# strip only debug info from modules (keep symbols needed to load)
	$(CROSS_COMPILE)strip -g hello1.ko
	$(CROSS_COMPILE)strip -g hello2.ko

clean:
	$(MAKE) -C $(KDIR) M=$$PWD clean
	rm -f *.ko.unstripped

# just use: make hello1.s / make hello1.i / make hello2.s / make hello2.i
%.s %.i: %.c
	$(MAKE) -C $(KDIR) M=$$PWD $@

endif

