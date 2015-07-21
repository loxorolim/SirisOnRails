require 'mkmf-rice'
$CXX += " -w -I GLPK -std=c++0x -lglpk"
create_makefile('Siris')


