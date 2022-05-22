g++.exe -std=c++17 -fexceptions -g -std=c++17 -ISDL2\include\SDL2 -Isrc -c D:\C++\UET\Board-game\src\Chess_Board_query.cpp -o obj\Debug\src\Chess_Board_query.o
g++.exe -std=c++17 -fexceptions -g -std=c++17 -ISDL2\include\SDL2 -Isrc -c D:\C++\UET\Board-game\src\Chess_Piece.cpp -o obj\Debug\src\Chess_Piece.o
g++.exe -std=c++17 -fexceptions -g -std=c++17 -ISDL2\include\SDL2 -Isrc -c D:\C++\UET\Board-game\src\Chess_PieceLocations.cpp -o obj\Debug\src\Chess_PieceLocations.o
g++.exe -std=c++17 -fexceptions -g -std=c++17 -ISDL2\include\SDL2 -Isrc -c D:\C++\UET\Board-game\src\Game.cpp -o obj\Debug\src\Game.o
g++.exe -std=c++17 -fexceptions -g -std=c++17 -ISDL2\include\SDL2 -Isrc -c D:\C++\UET\Board-game\src\Window.cpp -o obj\Debug\src\Window.o
g++.exe -LSDL2\lib -o bin\Debug\Board-game.exe 
	obj\Debug\main.o 
	obj\Debug\src\Button.o 
	obj\Debug\src\Chess_Board.o 
	obj\Debug\src\Chess_Board_caculateMove.o 
	obj\Debug\src\Chess_Board_query.o 
	obj\Debug\src\Chess_Board_render.o 
	obj\Debug\src\Chess_Board_threats.o 
	obj\Debug\src\Chess_BoardState.o 
	obj\Debug\src\Chess_MoveStack.o 
	obj\Debug\src\Chess_Piece.o 
	obj\Debug\src\Chess_PieceLocations.o 
	obj\Debug\src\Chess_StoreMove.o obj\Debug\src\Game.o 
	obj\Debug\src\Window.o  
	-lmingw32 -lSDL2main -lSDL2 -lSDL2_image