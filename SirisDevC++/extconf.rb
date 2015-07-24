require 'mkmf-rice'
$CXX += " -w  -std=c++0x" 
$INCFLAGS += " -I GLPK"
$LDFLAGS += " -L GLPK -lglpk"
create_makefile('Siris')


