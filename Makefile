build: main.cpp
	g++ -std=c++17 -o bot main.cpp -ldpp
recompile: main.cpp build
	./bot
effective-build: main.cpp
	g++ -std=c++17 -Ofast -o bot main.cpp -ldpp
