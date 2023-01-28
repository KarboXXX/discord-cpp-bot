build: main.cpp
	g++ -std=c++17 -o bot main.cpp -ldpp
recompile: main.cpp build
	./bot
