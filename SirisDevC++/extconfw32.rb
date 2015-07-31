require 'mkmf-rice'
$CXX += " -w  -std=c++0x" 
$INCFLAGS += " -I GLPK -I VLFEAT"
$LDFLAGS += " -L GLPK/w32/ -lglpk_4_54 -L VLFEAT/bin/win32 -lvl"
create_makefile('Siris')


