DEBUG ?= 0
ifeq ($(DEBUG), 1)
CPPFLAGS += -D DEBUG
endif

DIST_FOLDER ?= ../dist/libibmmanifest_1.0.0
INSTALL_FOLDER ?= /usr/bin
CPPFLAGS += -D _GLIBCXX_USE_CXX11_ABI=0 -I ../../include -L/usr/lib 
LDLIBS += -lstdc++ -ldl
OCP = objcopy
MD = mkdir -p
CP = cp --preserve

vpath %.cpp src
vpath %.h ../../include

SOURCES = ceg_deploy.cpp

all: ceg_deploy

ceg_deploy: ceg_deploy.o ../../lib/servicemanifestwrapper.a

ceg_deploy.o: servicemanifestwrapper.h

../../lib/servicemanifestwrapper.a:
	$(MAKE) -C ../../dlwrapper $(MAKECMDGOALS)

install: ceg_deploy
	ln -svf ceg_deploy cegLog
	ln -svf ceg_deploy cegInstrument
	ln -svf ceg_deploy cegRegister

test: ceg_deploy
	./ceg_deploy

.PHONY: clean
clean:
	$(RM) *.o
	$(RM) *.d
	$(RM) ceg_deploy
	$(RM) cegLog
	$(RM) cegInstrument
	$(RM) cegRegister
	$(RM) $(DIST_FOLDER)$(INSTALL_FOLDER)/ceg_deploy

bindep-package: ceg_deploy
	$(MD) --mode=0755 $(DIST_FOLDER)$(INSTALL_FOLDER)
	$(MD) --mode=0755 $(DIST_FOLDER)_bin$(INSTALL_FOLDER)
	$(OCP) ceg_deploy $(DIST_FOLDER)$(INSTALL_FOLDER)/ceg_deploy
	$(OCP) ceg_deploy $(DIST_FOLDER)_bin$(INSTALL_FOLDER)/ceg_deploy
	../../util/dist_bin_deps.sh ./ceg_deploy $(DIST_FOLDER)_bin$(INSTALL_FOLDER)

package: all install bindep-package
	
include $(subst .cpp,.d,$(SOURCES))

%.d: %.cpp
	$(CC) -M $(CPPFLAGS) $< > $@.$$$$; \
		sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
		rm -f $@.$$$$
