################################################################################
# File:	Makefile
# 		Code compilation directives.
# 		Compile and install using
# 			$ make
# 			$ make install
#
# Author: Rambod Rahmani <rambodrahmani@autistici.org>
#         Created on 06/07/2019
#
################################################################################

PREFIX=$(HOME)/QEMU_LINUX_KERNEL

include $(PREFIX)/etc/libqlk.conf

################################################################################
#
################################################################################
SCRIPTS=scripts/compile scripts/boot scripts/debug

all: $(SCRIPTS) setenv

.PHONY: install

install: $(HD_PATH) $(SCRIPTS) setenv
	install -d			$(BIN)
	install -m 755  $(SCRIPTS)	$(BIN)

scripts/%: scripts/%.sh
	sed -e 's#@BIN@#$(BIN)#g' \
	    -e 's#@LIB@#$(LIB)#g' \
	    -e 's#@LIB64@#$(LIB64)#g' \
	    -e 's#@INCLUDE@#$(INCLUDE)#g' \
	    -e 's#@HD_SIZE@#$(HD_SIZE)#g' \
	    -e 's#@HD_PATH@#$(HD_PATH)#g' $^ > $@ \
	|| { rm $@; exit 1; }

setenv:
	echo "export PATH=\"$(PREFIX)/bin:\$$PATH\"" > setenv

clean:
	rm -f $(SCRIPTS)

