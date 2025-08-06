# Definindo variáveis
CXX = g++
CXXFLAGS = -std=c++17 -I/opt/homebrew/include
LDFLAGS = -L/opt/homebrew/lib -lraylib -framework CoreVideo -framework IOKit -framework Cocoa -framework OpenGL
SRC = main.cpp NeuralNet.cpp SnakeGame.cpp
OBJ = $(SRC:.cpp=.o)
EXEC = main

# Regra para compilar o programa
$(EXEC): $(OBJ)
	$(CXX) $(OBJ) -o $(EXEC) $(LDFLAGS)

# Regra para compilar os arquivos .cpp em .o
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Limpeza dos arquivos gerados
clean:
	rm -f $(OBJ) $(EXEC)