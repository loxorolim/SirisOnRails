require 'mkmf-rice'
$CXX += " -w  -std=c++0x" 
$INCFLAGS += " -I GLPK -I vlfeat-0.9.20"
$LDFLAGS += " -L GLPK -lglpk -llibvl.so"
create_makefile('Siris')


