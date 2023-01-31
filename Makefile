
build: main.cpp
	g++ -std=c++17 -o bot main.cpp -ldpp -lcurl
recompile: main.cpp build
	./bot
effective-build: main.cpp
	g++ -std=c++17 -Ofast -o bot main.cpp -ldpp -lcurl
