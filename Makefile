# http://make.mad-scientist.net/papers/multi-architecture-builds/
# used in conjuction with target.mk
ifeq (,$(filter _%,$(notdir $(CURDIR))))
include target.mk
else
#----- End Boilerplate

VPATH = $(SRCDIR)

# Normal makefile rules here
CXX=g++
# CXXFLAGS=-g -std=c++11 -fext-numeric-literals -Wall -pedantic
CXXFLAGS=-g -std=c++11 -fext-numeric-literals -Wall -Wno-comment -pedantic -O3
%: %.cpp
	$(CXX) $(CXXFLAGS) $< -o $@

#----- Begin Boilerplate
endif
