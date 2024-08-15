CXXFLAGS += -std=c++17

SRC = $(wildcard src/*.cpp) $(wildcard bots/*bot.cpp)
OBJ = $(patsubst %.cpp, %.o, $(SRC))

# make sim: src/main.cpp bots/%.cpp bots/include/%.h include/%.h
# 	$(CXX) -std=c++17

sim: $(OBJ) #tentataive for now because we don't want to compile test.cpp. compile with all object files.
	$(CXX) $(CXXFLAGS) -o HanaSim $^

test: test.o
	$(CXX) $(CXXFLAGS) -o HanaTest test.o

# bots/%.o: bots/%.cpp bots/include/%.h
# 	$(CXX) $(CXXFLAGS) $< -c -o $@ 

card.o: src/card.cpp include/card.h
	$(CXX) $(CXXFLAGS) src/card.cpp -c -o $@

game.o: src/game.cpp include/game.h
	$(CXX) $(CXXFLAGS) src/game.cpp -c -o $@

humanplayer.o: src/humanplayer.cpp include/humanplayer.h include/move.h
	$(CXX) $(CXXFLAGS) src/humanplayer.cpp -c -o $@

main.o: src/main.cpp include/*.h bots/include/*.h
	$(CXX) $(CXXFLAGS) src/main.cpp -c -o $@

move.o: src/move.cpp include/move.h
	$(CXX) $(CXXFLAGS) src/move.cpp -c -o $@

state.o: src/state.cpp include/state.h
	$(CXX) $(CXXFLAGS) src/state.cpp -c -o $@

%bot.o: bots/%bot.cpp bots/include/%bot.h
	$(CXX) $(CXXFLAGS) $< -c -o $@



# Complexity will need to be added to HanaTest as the testing code gets more dependencies like HanaSim
test.o: tests/tests.cpp # Included header files aren't actually used yet, so not needed.
	$(CXX) $(CXXFLAGS) tests/tests.cpp -c -o $@

.PHONY clean: 
	rm -f src/*.o 
	rm -f bots/*.o
	rm -f tests/*.o


#.PHONY clean: clean $(OBJ)
#	rm -f $(OBJ)
#	rm -f HanaSim

#clean %:  
#	rm -rf 