CPPFLAGS=-g -I src/ -std=c++17 -Wall -Wextra -pedantic-errors
LDFLAGS=-g -L/usr/local/lib
LDLIBS=-L/usr/local/lib


ODIR=obj
BINDIR=bin

src_files = emulator opcode_handler_macro test opcode_handler
obj_files = emulator.o opcode_handler_macro.o test.o opcode_handler.o
obj_files_dir = $(ODIR)/emulator.o $(ODIR)/opcode_handler_macro.o $(ODIR)/test.o $(ODIR)/opcode_handler.o

all: createfolders emulator.o opcode_handler_macro.o test.o opcode_handler.o
	g++ $(CPPFLAGS) $(LDFLAGS) -o $(BINDIR)/test_emu.out  $(obj_files_dir) $(LDLIBS)

createfolders:
	mkdir obj -p
	mkdir bin -p

$(filter %.o,$(obj_files)): %.o: src/%.cpp
	g++ $(CPPFLAGS) -c src/$(basename $@).cpp -o $(ODIR)/$@