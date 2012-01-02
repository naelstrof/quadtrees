all: quadtrees

quadtrees: main.o NWindow.o NText.o NShaderManager.o NTexture.o NQuadTree.o
	g++ main.o NWindow.o NText.o NShaderManager.o NTexture.o NQuadTree.o -o quadtrees -lX11 -lGL -lXext -lpng

NWindow.o: NWindow.h NWindow.cpp
	g++ -Wall -c NWindow.cpp
	
main.o: main.cpp NWindow.h
	g++ -Wall -Wno-unused-variable -c main.cpp
	
NShaderManager.o: NShaderManager.h NShaderManager.cpp
	g++ -Wall -c NShaderManager.cpp

NText.o: NText.h NText.cpp
	g++ -Wall -Wno-unused-variable -c NText.cpp

NTexture.o: NTexture.h NTexture.cpp
	g++ -Wall -Wno-unused-variable -c NTexture.cpp
	
NQuadTree.o: NQuadTree.h NQuadTree.cpp
	g++ -Wall -c NQuadTree.cpp