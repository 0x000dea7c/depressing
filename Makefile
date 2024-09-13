# I'm so sad I cannot use sanitizers because libraries leak bytes everywhere.
CXX          = g++
OUTPUT_DIR   = build
OUTPUT_BIN   = $(OUTPUT_DIR)/stupid_game
SOURCE       = source/main.C source/glad.c
STD          = -std=gnu++23
WARNINGS     = -Wall -Wpedantic -Wextra -Winline -Wuninitialized -Wshadow -Wconversion -Wstrict-aliasing=2  \
               -Wnon-virtual-dtor -Wnull-dereference -Woverloaded-virtual -Wformat -Wfloat-equal -Weffc++   \
               -Wstrict-overflow=5
INC          = -Iinclude -Idependencies/include
MISC         = -fno-rtti -fno-exceptions -DGLM_FORCE_SSE2 -DGLM_FORCE_DEFAULT_ALIGNED_GENTYPES
SDL          = `sdl2-config --cflags --libs`
LINK_RELEASE = -Wl,--as-needed -Wl,-O1 -Wl,--strip-all -Wl,-Bsymbolic -Wl,--no-undefined -Wl,-z,relro
LINK_DEBUG   = -Wl,-z,relro

release: build
	$(CXX) $(STD) -O3   $(WARNINGS) $(INC) $(MISC) $(SOURCE) -o $(OUTPUT_BIN) $(LINK) $(SDL)

debug: build
	$(CXX) $(STD) -ggdb $(WARNINGS) $(INC) $(MISC) $(SOURCE) -o $(OUTPUT_BIN) $(LINK_DEBUG) $(SDL)

build:
	mkdir -p $(OUTPUT_DIR)

clean:
	rm $(OUTPUT_DIR)

run:
	./$(OUTPUT_BIN)
