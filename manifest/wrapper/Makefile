DEBUG ?= 0
ifeq ($(DEBUG), 1)
CPPFLAGS += -D DEBUG
endif

OSTYPE := $(shell gcc --version)
SONAME = libibmmanifest
MAJVER = 1
MINVER = 0
REVVER = 0
SOVER = $(MAJVER).$(MINVER).$(REVVER)
SOPATH = $(SONAME).so.$(SOVER)
DIST_FOLDER ?= ../dist
RES_FOLDER ?= ../res
INSTALL_FOLDER ?= /usr/lib
MD = mkdir -p
CP = cp -p
CPI = cp -i
OCP = objcopy -S
LN = ln -svf
DPKG =
CPPFLAGS += -D _GLIBCXX_USE_CXX11_ABI=0 -I ../../manifest-runtime/mbed-tls/include -I ../include -fPIC -shared
LDFLAGS += -z noexecstack -Wl,-soname,$(SONAME).so.$(MAJVER) -shared -lstdc++
CLEANEXTS += .o .d
PACKAGE_TS = $(RES_FOLDER)/package_timestamp.txt

ifneq (,$(findstring Red Hat, $(OSTYPE)))
	PACKAGE := rpm-package
	INSTALLCMD = rpm -i ../dist/rpm/RPMS/x86_64/libibmmanifest-1.0-1.x86_64.rpm
else ifneq (,$(findstring ubuntu, $(OSTYPE)))
	PACKAGE := deb-package
	INSTALLCMD = dpkg -i ../dist/libibmmanifest1.deb
else
	PACKAGE :=
endif

vpath %.cpp	src
vpath %.json $(RES_FOLDER)

SOURCES = servicemanifeststub.cpp \
		  lumberjackstub.cpp \
		  simplehttpstub.cpp

$(SOPATH): servicemanifeststub.o lumberjackstub.o simplehttpstub.o servicemanifest_json.o ../lib/manifest-runtime.a
	$(CXX) $(LDFLAGS) -o $@ $^

servicemanifest_json.o: servicemanifest.json
	$(LD) -r -b binary -o $@ $^

dist-deb: $(SOPATH) $(RES_FOLDER)/changelog
	$(MD) $(DIST_FOLDER)/deb/$(SONAME)
	$(CP) -r dist_shell/* $(DIST_FOLDER)/deb/$(SONAME)
	$(OCP) $< $(DIST_FOLDER)/deb/$(SONAME)$(INSTALL_FOLDER)/$<
	$(LN) $< $(DIST_FOLDER)/deb/$(SONAME)$(INSTALL_FOLDER)/$(SONAME).so.$(MAJVER)
	chmod -Rvf 0755 $(DIST_FOLDER)/deb
	chmod 0644 $(DIST_FOLDER)/deb/$(SONAME)$(INSTALL_FOLDER)/$<
	chmod 0644 $(DIST_FOLDER)/deb/$(SONAME)/DEBIAN/shlibs
	chmod 0644 $(DIST_FOLDER)/deb/$(SONAME)/DEBIAN/triggers
	chmod 0644 $(DIST_FOLDER)/deb/$(SONAME)/usr/share/doc/$(SONAME)$(MAJVER)/copyright
	gzip -n -f --best < $(RES_FOLDER)/changelog > $(DIST_FOLDER)/deb/$(SONAME)/usr/share/doc/$(SONAME)$(MAJVER)/changelog.gz
	chmod 0644 $(DIST_FOLDER)/deb/$(SONAME)/usr/share/doc/$(SONAME)$(MAJVER)/changelog.gz

dist-rpm: $(SOPATH) $(RES_FOLDER)/changelog
	$(MD) $(DIST_FOLDER)/rpm
	$(CP) -r rpm_shell/* $(DIST_FOLDER)/rpm/.
	$(MD) $(DIST_FOLDER)/rpm/$(SONAME)$(INSTALL_FOLDER)
	$(OCP) $< $(DIST_FOLDER)/rpm/$(SONAME)$(INSTALL_FOLDER)/$<
	chmod -Rvf 0755 $(DIST_FOLDER)/rpm
	chmod 0644 $(DIST_FOLDER)/rpm/$(SONAME)$(INSTALL_FOLDER)/$<
	$(CP) $(RES_FOLDER)/changelog $(DIST_FOLDER)/rpm/$(SONAME)/usr/share/doc/$(SONAME)$(MAJVER)/.
	chmod 0644 $(DIST_FOLDER)/rpm/$(SONAME)/usr/share/doc/$(SONAME)$(MAJVER)/changelog
	cd $(DIST_FOLDER)/rpm && tar -zcvf ../rpm/SOURCES/$(SONAME).tar.gz $(SONAME)/

dist-bin: $(SOPATH) $(RES_FOLDER)/changelog
	$(MD) --mode=0755 $(DIST_FOLDER)/tarball
	$(CP) -r bin_shell/* $(DIST_FOLDER)/tarball/.
	$(OCP) $< $(DIST_FOLDER)/tarball$(INSTALL_FOLDER)/$<
	$(LN) $< $(DIST_FOLDER)/tarball$(INSTALL_FOLDER)/$(SONAME).so.$(MAJVER)
	chmod -Rvf 0755 $(DIST_FOLDER)/tarball
	chmod 0644 $(DIST_FOLDER)/tarball$(INSTALL_FOLDER)/$<
	$(CP) $(RES_FOLDER)/changelog $(DIST_FOLDER)/tarball/usr/share/doc/$(SONAME)$(MAJVER)/.
	chmod 0644 $(DIST_FOLDER)/tarball/usr/share/doc/$(SONAME)$(MAJVER)/changelog

install: $(SOPATH)
	$(CP) $^ $(INSTALL_FOLDER)
	ldconfig $(INSTALL_FOLDER)/$^

$(RES_FOLDER)/changelog:
	../util/git_changelog.sh $(RES_FOLDER)/changelog $(PACKAGE_TS)

.PHONY:
test:

clean:
	$(RM) *.o
	$(RM) *.d
	$(RM) -Rvf $(DIST_FOLDER)
	$(RM) -Rvf $(DIST_FOLDER)_bin
	$(RM) $(RES_FOLDER)/changelog
	$(RM) $(RES_FOLDER)/changelog.gz

clean-manifest:
	$(RM) -Rvf $(RES_FOLDER)

generate:
	$(MD) $(RES_FOLDER)
	$(CPI) servicemanifest.json_tmpl $(RES_FOLDER)/servicemanifest.json

# Override the CPPFLAGS just for this target
validator: CPPFLAGS:= -D _GLIBCXX_USE_CXX11_ABI=0 -I ../include
validator: validator.o servicemanifest_json.o
	$(CXX) -o $@ $^

# Build the JSON blob into the validator then run it to check the JSON
validate:  validator force_trigger
	./validator

# Dummy dependency to force the validate target to always trigger its action
force_trigger:

timestamp:
	../util/timestamp_package.sh $(PACKAGE_TS)

bindep-package: dist-bin
	../util/dist_bin_deps.sh $(DIST_FOLDER)/tarball$(INSTALL_FOLDER)/$(SOPATH) $(DIST_FOLDER)/tarball
	cd $(DIST_FOLDER)/tarball && tar -zcvf ../$(DIST_FOLDER)/$(SONAME).tar.gz .

rpm-package: dist-rpm
	rpmbuild --define "_topdir $(CURDIR)/$(DIST_FOLDER)/rpm" -bb $(DIST_FOLDER)/rpm/SPECS/$(SONAME).spec

deb-package: dist-deb
	fakeroot dpkg-deb --build $(DIST_FOLDER)/deb/$(SONAME) ../dist/$(SONAME)$(MAJVER).deb
	lintian ../dist/$(SONAME)$(MAJVER).deb

package: $(PACKAGE) bindep-package timestamp

register: ./libibmmanifest.so.1.0.0
	../cegtools/scripts/registerservice.sh ./libibmmanifest.so.1.0.0

install-pkg: package
	$(INSTALLCMD)
include $(subst .cpp,.d,$(SOURCES))

%.d: %.cpp
	$(CC) -M $(CPPFLAGS) $< > $@.$$$$; \
		sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
		rm -f $@.$$$$
