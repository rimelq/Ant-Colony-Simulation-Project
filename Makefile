# Definitions de macro
OUT = projet
CXX = g++
CXXFLAGS = -g3 -Wall -std=c++11
LINKING = `pkg-config --cflags gtkmm-3.0`
LDLIBS = `pkg-config --libs gtkmm-3.0`
CXXFILES = projet.cc simulation.cc fourmiliere.cc fourmi.cc nourriture.cc message.cc squarecell.cc error_squarecell.cc gui.cc graphic.cc
OFILES = $(CXXFILES:.cc=.o)

# Dependances

all: $(OUT)

projet.o: projet.cc simulation.h nourriture.h squarecell.h \
 error_squarecell.h message.h fourmiliere.h fourmi.h
	$(CXX) $(CXXFLAGS) $(LINKING) -c $< -o $@ $(LINKING)

simulation.o: simulation.cc simulation.h nourriture.h squarecell.h \
 error_squarecell.h message.h fourmiliere.h fourmi.h
	$(CXX) $(CXXFLAGS) $(LINKING) -c $< -o $@ $(LINKING)

graphic.o: graphic.cc graphic_gui.h graphic.h
	$(CXX) $(CXXFLAGS) $(LINKING) -c $< -o $@ $(LINKING)

fourmiliere.o: fourmiliere.cc fourmiliere.h fourmi.h squarecell.h \
 error_squarecell.h message.h constantes.h
	$(CXX) $(CXXFLAGS) $(LINKING) -c $< -o $@ $(LINKING)

fourmi.o: fourmi.cc fourmi.h squarecell.h error_squarecell.h \
 constantes.h message.h
	$(CXX) $(CXXFLAGS) $(LINKING) -c $< -o $@ $(LINKING)

nourriture.o: nourriture.cc nourriture.h squarecell.h error_squarecell.h constantes.h\
 message.h
	$(CXX) $(CXXFLAGS) $(LINKING) -c $< -o $@ $(LINKING)

message.o: message.cc message.h
	$(CXX) $(CXXFLAGS) $(LINKING) -c $< -o $@ $(LINKING)

squarecell.o: squarecell.cc squarecell.h error_squarecell.h graphic_gui.h graphic.h 
	$(CXX) $(CXXFLAGS) $(LINKING) -c $< -o $@ $(LINKING)

error_squarecell.o: error_squarecell.cc error_squarecell.h
	$(CXX) $(CXXFLAGS) $(LINKING) -c $< -o $@ $(LINKING)
	
gui.o: gui.cc gui.h graphic_gui.h simulation.h
	$(CXX) $(CXXFLAGS) $(LINKING) -c $< -o $@ $(LINKING)

$(OUT): $(OFILES)
	$(CXX) $(CXXFLAGS) $(LINKING) $(OFILES) -o $@ $(LDLIBS)

# Commandes
clean:
	@echo " *** EFFACE MODULES OBJET ET EXECUTABLE ***"
	@/bin/rm -f *.o *.x *.cc~ *.h~ $(OUT)
