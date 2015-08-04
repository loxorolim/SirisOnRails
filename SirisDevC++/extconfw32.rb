require 'mkmf-rice'
$CXX += " -w  -std=c++0x" 
$INCFLAGS += " -I GLPK/include -I VLFEAT"
$LDFLAGS += " -L GLPK/w32/ -lglpk -L VLFEAT/bin/mingw -lvl"
create_makefile('Siris')


