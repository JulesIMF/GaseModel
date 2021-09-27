#+++
# 
# Copyright (c) 2021 JulesIMF, MIPT
#
# Module name:
#
#		makefile
#
# Abstract:
#
#		My default makefile. Allows to compile the project
#		changing only FILES (and other flags if needed).
#		
#		Build this project with "make" or "make mol".
#		
#
#---

#
# Defines
#

CXX	    = g++ -std=c++17
INC	    = -Iincludes -Iwrapper/includes
SFML    = -lsfml-graphics -lsfml-window -lsfml-system
OPT	    = -O2
DEF		= -DNDEBUG
CFLAGS	= $(OPT) $(DEF) -g $(INC)
OBJ	    = obj/

FILES   = $(OBJ)main.o \
          $(OBJ)wrapper/Wrapper.o \
          $(OBJ)vector/Vector2.o \
          $(OBJ)molecules/Molecule.o $(OBJ)molecules/Manager.o
		  

#
# Rules
#

mol: $(FILES)
	$(CXX) $(CFLAGS) $^ $(SFML) -o $@
#	(make clean;)

obj/%.o: %.cpp
	$(CXX) $(CFLAGS) $^ -c
	mkdir -p $(@D)
	mv $(@F) $@

clean:
	rm $(FILES)

%: %.cpp
	(make obj/$@.o;)
