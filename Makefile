default: main.o particlemap.o
	g++ -g -Wall $^ -o islandgen
%.o: %.cpp
	g++ -c $< -o $@
clean:
	rm island.txt islandgen *.o
