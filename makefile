# Compilador
CXX = g++
# Flags de compilacao (Modo Debug, C++11, warnings)
CXXFLAGS = -g -Wall -std=c++11
# Diretorios
IDIR = include
SDIR = src
ODIR = obj
BINDIR = bin

# --- Flags de Linkagem Especificas do Sistema ---
# (Descomente a secao para o seu SO)

# Para LINUX 
LDFLAGS = -lGL -lglut -lGLU -lSDL2 -lSDL2_mixer

# Para WINDOWS (usando MSYS2/MinGW)
# LDFLAGS = -lfreeglut -lopengl32 -lglu32 -lmingw32 -lSDL2main -lSDL2 -lSDL2_mixer

# --- Restante do Makefile ---

# Encontra todos os arquivos .cpp em src
SOURCES = $(wildcard $(SDIR)/*.cpp)
# Gera nomes dos arquivos objeto .o em obj/
OBJECTS = $(patsubst $(SDIR)/%.cpp,$(ODIR)/%.o,$(SOURCES))

# Nome do executavel
TARGET = $(BINDIR)/space_invaders

# Regra principal
all: $(TARGET)

# Regra para linkar o executavel
$(TARGET): $(OBJECTS)
	@mkdir -p $(BINDIR)
	$(CXX) -o $(TARGET) $^ $(LDFLAGS)
	@echo "Build finalizado: $(TARGET)"

# Regra para compilar arquivos .cpp para .o
$(ODIR)/%.o: $(SDIR)/%.cpp
	@mkdir -p $(ODIR)
	$(CXX) $(CXXFLAGS) -I$(IDIR) -c $< -o $@

# Regra para limpar os arquivos compilados
clean:
	rm -f $(ODIR)/*.o $(TARGET)