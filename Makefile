CXX        = g++
OUTPUT_DIR = build
OUTPUT_BIN = $(OUTPUT_DIR)/stupid_game
SOURCE     = source/main.C dependencies/glad/src/glad.c
STD        = -std=gnu++23
WARNINGS   = -Wall -pedantic
INC        = -Iinclude -Idependencies/include/SDL2-2.30.7 -Idependencies/include/glm1.0.1 -Idependencies/glad/include/glad
MISC       = -fno-rtti -fno-exceptions -DGLM_FORCE_SSE2 -DGLM_FORCE_DEFAULT_ALIGNED_GENTYPES
LINK       = -Ldependencies/libraries/SDL2-2.30.7 -lSDL2 -static
LINK_DEBUG = -Ldependencies/libraries/SDL2-2.30.7 -lSDL2 -static-libasan

release: build
	$(CXX) $(STD) -O3   $(WARNINGS) $(INC) $(MISC) $(SOURCE) -o $(OUTPUT_BIN) $(LINK)

debug: build
	$(CXX) $(STD) -ggdb -fsanitize=address,undefined $(WARNINGS) $(INC) $(MISC) $(SOURCE) -o $(OUTPUT_BIN) $(LINK_DEBUG)

build:
	mkdir -p $(OUTPUT_DIR)

clean:
	rm $(OUTPUT_DIR)

run:
	./$(OUTPUT_BIN)
