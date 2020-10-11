#==============================================================================================
# Makefile for the circle packaging simulation program.
# Author: Dmitry Safonov
# Date: 24.08.2020
#==============================================================================================

CC = gcc
CFLAGS = -c -Wall -Wextra -Wsign-conversion -pedantic-errors -g -o
LFLAGS = -o
SDIR = src
ODIR = build/obj
DDIR = data
EDIR = build
IDIR = include
LDIR = lib
LIBS = -luser32 -lgdi32

all: Directories Data Program

#==============================================================================================
# Rule for creating neccessary directories
#==============================================================================================

.PHONY: Directories
Directories:
	mkdir -p build
	mkdir -p build/obj
	mkdir -p build/data

#==============================================================================================
# Rule for copying data files to the ./build/data directory
#==============================================================================================

.PHONY: Data
Data: $(DDIR)
	cp -u $(DDIR)/*.* $(EDIR)/data/

#==============================================================================================
# Dependences (for further creating the object *.o files):
#==============================================================================================

# Module for the win32 platform (creating the window with win32API)
_DEPS_win32_platform = utils.h simulation.h input_treatment.h
DEPS_win32_platform = $(patsubst %,$(IDIR)/%,$(_DEPS_win32_platform))

# Module for the work with input treatment (from mouse and keyboard):
_DEPS_input_treatment = input_treatment.h utils.h
DEPS_input_treatment = $(patsubst %,$(IDIR)/%,$(_DEPS_input_treatment))

# Module for the software rendering functions
_DEPS_software_rendering = software_rendering.h utils.h file_io.h
DEPS_software_rendering = $(patsubst %,$(IDIR)/%,$(_DEPS_software_rendering))

# Module for the file_io (working with files)
_DEPS_file_io = file_io.h utils.h
DEPS_file_io = $(patsubst %,$(IDIR)/%,$(_DEPS_file_io))

# Module for the font
_DEPS_font = font.h utils.h file_io.h software_rendering.h 
DEPS_font = $(patsubst %,$(IDIR)/%,$(_DEPS_font))

# Module for the misc functions
_DEPS_misc = utils.h
DEPS_misc = $(patsubst %,$(IDIR)/%,$(_DEPS_misc))

# Module for the world class
_DEPS_world = world.h utils.h software_rendering.h
DEPS_world = $(patsubst %,$(IDIR)/%,$(_DEPS_world))

# Module for the text_panel class
_DEPS_text_panel = text_panel.h utils.h software_rendering.h world.h font.h box.h
DEPS_text_panel = $(patsubst %,$(IDIR)/%,$(_DEPS_text_panel))

# Module for the box class
_DEPS_box = box.h utils.h world.h circles_list.h software_rendering.h
DEPS_box = $(patsubst %,$(IDIR)/%,$(_DEPS_box))

# Module for the circle class
_DEPS_circle = circle.h utils.h misc.h software_rendering.h
DEPS_circle = $(patsubst %,$(IDIR)/%,$(_DEPS_circle))

# Module for the circles_list class
_DEPS_circles_list = circles_list.h circle.h utils.h misc.h
DEPS_circles_list = $(patsubst %,$(IDIR)/%,$(_DEPS_circles_list))

# Module for the pso_algorithm class
_DEPS_pso_algorithm = pso_algorithm.h utils.h circle.h circles_list.h box.h misc.h
DEPS_pso_algorithm = $(patsubst %,$(IDIR)/%,$(_DEPS_pso_algorithm))

# Module for the simualtion class
_DEPS_simualtion = simulation.h software_rendering.h utils.h file_io.h font.h misc.h \
world.h box.h text_panel.h circle.h circles_list.h pso_algorithm.h
DEPS_simualtion = $(patsubst %,$(IDIR)/%,$(_DEPS_simualtion))

# Module for the sort_array class
_DEPS_sort_array = sort_array.h 
DEPS_sort_array = $(patsubst %,$(IDIR)/%,$(_DEPS_sort_array))

#==============================================================================================
# Rules for compilation of the object *.o files:
#==============================================================================================

$(ODIR)/%.o: $(SDIR)/%.c $(DEPS_%) 
	$(CC) $(CFLAGS) $@ $< -I$(IDIR)

#==============================================================================================
# Rules for the linker:
#==============================================================================================

_OBJ = win32_platform.o input_treatment.o software_rendering.o file_io.o font.o misc.o world.o \
text_panel.o box.o circle.o circles_list.o pso_algorithm.o simulation.o sort_array.o 
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

Program: $(OBJ)
	$(CC) $(LFLAGS) $(EDIR)/$@ $^ -L$(LDIR) $(LIBS)
	ctags -f ./TAGS -e -R ./include ./src
	ctags -f ./src/TAGS -e -R ./include ./src
# use flag -s for stipping the executable (to reduce the size)

#==============================================================================================
# Rules for the clean the build firectory:
#==============================================================================================

.PHONY: clean
clean:
	rm -f $(ODIR)/*.o
