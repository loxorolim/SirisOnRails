ARCH = $(shell getconf LONG_BIT)
ifeq ($(ARCH),32)
	VLFEAT_DIR = VLFEAT/bin/glnx86/
	BIT_FLAG = -m32
else
	VLFEAT_DIR = VLFEAT/bin/glnxa64/
	BIT_FLAG = -m64
endif
	
CC=g++
CFLAGS= -c -std=c++11 $(BIT_FLAG)
LDFLAGS= -L GLPK/lib/ -L $(VLFEAT_DIR) -lvl -lglpk
INCDIRS= -I GLPK/include/ -I VLFEAT/ -I pugixml/ -I rapidjson/
SOURCES= Main.cpp AutoPlanningMethods.cpp auxiliar.cpp FatherMethods.cpp Grasp.cpp Grid.cpp HataSRD.cpp HeatgridMethods.cpp KMLMethods.cpp LinkCalculationMethods.cpp MetricCalculationMethods.cpp pugixml/pugixml.cpp TerminalSiris.cpp WebSiris.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=CompiledSiris

all: $(SOURCES) $(EXECUTABLE)
    
$(EXECUTABLE): $(OBJECTS)
	$(CC)  $(OBJECTS) $(INCDIRS) $(LDFLAGS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $(INCDIRS) $(LDFLAGS)    $< -o $@

.PHONY: clean

clean:
	rm -f $(binaries) *.o




