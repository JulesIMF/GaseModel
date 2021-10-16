# What is this?

This is an educational project within C++ course. The goal is to get a "gas model" --- molecules that fly in a 2-d window interacting with each other and wint the walls (bounds of the window).

# What are the smodules?

## JG
A JG for SFML to make life easier. It allows to abstract away from a specific graphics library while solving these type of tasks (constant drawing simple shapes on a window).

## Vector modules
Simple classes for 2-d vectors and 3-d vectors

## Molecules
Task-specific module. In this task we have a Molecule abstract class from which other molecules classes are derived and being described specifically. Manager holds pointers to molecules and is due for physics --- it reflects molecules from the walls and from each other. It is also a graphical manager --- that means that it is due to draw molecules on the screen.

# How to build?

1) Download and install SFML.
2) Download these sources.
3) Enter the directory and type "make" or "make mol"
4) Run the program typing "./mol"
5) Enjoy if u wish!
