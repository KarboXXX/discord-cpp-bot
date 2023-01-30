
build: main.cpp
	g++ -std=c++17 -o bot main.cpp -ldpp
recompile: main.cpp build
	./bot
effective-build: main.cpp
	g++ -std=c++17 -Ofast -o bot main.cpp -ldpp

img: avatar-manipulation.cpp
	g++ -std=c++17 -o testi avatar-manipulation.cpp

