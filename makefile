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

ifeq ($(ASAN), 1)
ASAN =-fsanitize=address
else
ASAN = 
endif

CXX	    = g++ -std=c++17
INC	    = -Iincludes -IJG/includes
SFML    = -lsfml-graphics -lsfml-window -lsfml-system
OPT	    = -O0
DEF		= -DNDEBUG
CFLAGS	= $(OPT) $(DEF) $(ASAN) -g $(INC)
OBJ	    = obj/

FILES   = $(OBJ)main.o \
          $(OBJ)vector/Vector2.o \
          $(OBJ)molecules/Molecule.o \
		  $(OBJ)molecules/Manager.o \
		  $(OBJ)molecules/physics.o \
		  JG/libJG.a

#
# Rules
#

mol: $(FILES)
	$(CXX) $(CFLAGS) $(FILES) -pthread $(SFML) -lX11 -o $@
#	(make clean;)

JG/libJG.a: 
	(cd JG; OPT=$(OPT) ASAN=$(ASAN) make $@)

obj/%.o: %.cpp
	$(CXX) $(CFLAGS) $^ -c
	mkdir -p $(@D)
	mv $(@F) $@

clean:
	rm $(FILES)
	(cd JG; make clean)

%: %.cpp
	(make obj/$@.o;)
