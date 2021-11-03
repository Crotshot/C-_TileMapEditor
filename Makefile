CC := g++
LIBS=-lsfml-graphics -lsfml-window -lsfml-system
INC=-I"C:/SFML-2.5.1/include"
LIBFILES=-L"C:/SFML-2.5.1/lib"
SRC_FILES :=  $(wildcard *.cpp)
HEADER_SRC_FILERS := $(wildcard *.hpp)
all: sfmlProg1

sfmlProg1: main.o
	$(CC) main.o $(SRC_FILES) $(HEADER_SRC_FILERS) -o main $(LIBFILES) $(LIBS) $(INC) 
main.o: main.cpp
	$(CC) -c main.cpp $(SRC_FILES) $(HEADER_SRC_FILERS) $(INC) $(LIBS)
clean:
	rm *.o sfmlProg1