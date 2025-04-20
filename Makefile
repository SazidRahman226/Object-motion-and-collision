all: compile link

compile: 
	g++ -c main.cpp -I"..\Object Motion and Collision\include"
 	
link:
	g++ main.o -o main.exe -L"..\Object Motion and Collision\lib" -lsfml-graphics -lsfml-window -lsfml-system

