CPPFLAGS += -D DEBUG -D _GLIBCXX_USE_CXX11_ABI=0 -fPIC -I ../include -L/usr/local/lib 
LOADLIBES += -lstdc++ -ldl

MD = mkdir -p 

vpath %.cpp	src
vpath %.c src
vpath %.h ../include

SOURCES = servicemanifestwrapper.cpp 

package dist all: ../lib servicemanifestwrapper.a

servicemanifestwrapper.a: servicemanifestwrapper.o
	$(AR) $(ARFLAGS) ../lib/$@ $^

../lib:
	$(MD) ../lib

.PHONY: clean
clean:
	$(RM) *.o
	$(RM) ../lib/servicemanifest.a

include $(subst .cpp,.d,$(SOURCES))

%.d: %.cpp
	$(CC) -M $(CPPFLAGS) $< > $@.$$$$; \
		sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
		rm -f $@.$$$$
