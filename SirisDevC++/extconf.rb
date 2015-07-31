require 'mkmf-rice'
$CXX += " -w  -std=c++0x" 
$INCFLAGS += " -I GLPK/include -I VLFEAT"
$LDFLAGS += " -L GLPK/lib -lglpk -L VLFEAT/bin/win32 -lvl"
create_makefile('Siris')


