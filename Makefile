# weizili
#
# The study test code project 
#
# Please use gmake but not make in FreeBSD OS
#

		
SUBDIRS=src

TOPDIR=`pwd`

TOP_CFLAGS="-g -O3 -DNDEBUG"
#TOP_CFLAGS="-g -D_DEBUG"
#TOP_CFLAGS="-g -D_DEBUG"
TOP_LDFLAGS=""

all: prepare
	for t in $(SUBDIRS); do $(MAKE) TOP_CFLAGS=$(TOP_CFLAGS) TOP_LDFLAGS=$(TOP_LDFLAGS) -C $$t ; $(MAKE) install -C $$t; if [ $$? -ne 0 ]; then exit -1; fi; done
#	cp install/lib/libwraputil.* ~/lib
#	cp install/lib/libnet.* ~/lib
#	cp install/lib/libnetproto.* ~/lib
#	cp install/lib/libqoslib.* ~/lib

check: prepare
	for t in $(SUBDIRS); do $(MAKE) check  TOP_CFLAGS=$(TOP_CFLAGS) TOP_LDFLAGS=$(TOP_LDFLAGS) -C $$t ;  if [ $$? -ne 0 ]; then exit -1; fi;done;
	$(MAKE) check -C src/cloud_rule

prepare:
	mkdir -p install/lib
	mkdir -p install/bin
	
clean:
	for t in $(SUBDIRS); do $(MAKE) clean -C $$t; done

.PHONY: clean all check prepare
