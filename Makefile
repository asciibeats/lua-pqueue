CFLAGS := $(shell pkg-config lua --cflags)
CFLAGS += -Isrc -fmax-errors=1 -Wall -Werror
CFLAGS += -O3 -finline-functions -fPIC
LDLIBS := $(shell pkg-config lua --libs)

LUA_VERSION	:= $(shell lua -e "_,_,v=string.find(_VERSION,'Lua (.+)');print(v)")
ifeq ($(LUA_VERSION),)
	LUA_VERSION := 5.3
endif
LUA_LIBDIR := /usr/lib/lua/$(LUA_VERSION)

SOURCES=$(wildcard src/*.cc)
OBJECTS=$(patsubst %.cc,%.o,$(SOURCES))
OUTFILE=pqueue.so

$(OUTFILE): $(OBJECTS)
	g++ -shared -o $(OUTFILE) $(OBJECTS) $(LDLIBS)

$(OBJECTS): src/%.o: src/%.cc
	g++ $(CFLAGS) -c $< -o $@

install:
	mkdir -p $(LUA_LIBDIR)
	cp $(OUTFILE) $(LUA_LIBDIR)

uninstall:
	rm -f $(LUA_LIBDIR)/$(OUTFILE)

clean:
	rm -f $(OUTFILE) src/*.o
