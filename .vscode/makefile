#source file search paths
VPATH = ./src ../PathFinder/src

	# compiler include search paths
INCS = -I./include \
	-I../windex/include \
	-I../raven-set \
	-I../PathFinder/src \

LIBS = \
	-lgdiplus -lgdi32 -lcomdlg32 -lstdc++fs \
	# -L../../code_ext/boost/boost1_78/lib \
	# 	-lboost_system-mgw11-mt-x64-1_78 \
	# 	-lboost_program_options-mgw11-mt-x64-1_78 \
		# -lboost_thread-mgw11-mt-x64-1_7 \
	#-mwindows

static_link_flags = -static-libstdc++ -static-libgcc -static -lComctl32

# folders
ODIR = ./.vscode/obj

OPT = -g
CCflags = -std=c++17 $(OPT) -D_mingw_

# sources
OBJ_starter = \
	$(ODIR)/main.o \
		$(ODIR)/cGraph.o \
	$(ODIR)/GraphTheory.o \

### COMPILE

$(ODIR)/%.o: %.cpp corridors.h
	g++  $(CCflags) \
	-c -o $@ $< $(INCS)

starter: $(OBJ_starter)
	g++ $(OPT) \
	-o bin/corridors.exe $^  \
	$(static_link_flags) $(LIBS)

.PHONY: clean
clean:
	del /q .vscode\\obj\\*.o

