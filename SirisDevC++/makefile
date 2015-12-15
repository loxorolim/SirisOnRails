CC=g++
CFLAGS= -c -std=c++11
LDFLAGS= -L GLPK/lib/ -L VLFEAT/bin/glnxa64/ -lvl -lglpk
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




