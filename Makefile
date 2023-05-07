
build: main.cpp
	g++ -std=c++20 -o bot main.cpp -ldpp -lcurl
recompile: main.cpp build
	./bot
effective-build: main.cpp
	g++ -std=c++20 -Ofast -o bot main.cpp -ldpp -lcurl
