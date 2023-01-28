build: main.cpp
	g++ -std=c++17 -o bot main.cpp -ldpp
<<<<<<< HEAD
recompile: main.cpp build
	./bot
effective-build: main.cpp
	g++ -std=c++17 -Ofast -o bot main.cpp -ldpp
=======

>>>>>>> parent of 8de8852 (auto recompile, shutdown from message, and bug fixes.)
