
OBJ_DIR = ./obj
SRC_DIR = ./src
INCLUDE_DIR = ./include

SDL_CONFIG_BIN = sdl2-config
SDL_INSTALL_FOLDER = ~/SDL

SRCS = \
	${SRC_DIR}/Scenario.c\
	${SRC_DIR}/Utils.c\
	${SRC_DIR}/CSV.c\
	${SRC_DIR}/GUI.c
LFLAGS += `$(SDL_CONFIG_BIN) --libs` -lSDL2_image 
CFLAGS += `$(SDL_CONFIG_BIN) --cflags` 

# Uncomment the above 3 lines if you want to use GUI mode.
# Recompile the whole project with `make clean all`
# You need to install SDL2 first!

OBJS = $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)


BIN_ST = Sim
SRC_ST = ${SRC_DIR}/Sim.c
OBJ_ST = $(SRC_ST:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)
OBJS_ST = $(OBJS) $(OBJ_ST)

BIN_MT = SimThreaded
SRC_MT = ${SRC_DIR}/SimThreaded.c
OBJ_MT = $(SRC_MT:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)
OBJS_MT = $(OBJS) $(OBJ_MT)

DEP = $(OBJS:%.o=%.d) $(OBJ_ST;%.o=%.d) $(OBJ_MT;%.o=%.d)

CC	   = gcc
CFLAGS += -g -Wall -Wextra -pedantic -std=c99 -lm -I${INCLUDE_DIR}
LFLAGS += -lm -pthread

RM     = rm -f

all: $(BIN_ST) $(BIN_MT)

-include $(DEP)

$(BIN_ST): $(OBJS_ST)
	$(CC) $(CFLAGS) -o $(BIN_ST) $(OBJS_ST) $(LFLAGS)

$(BIN_MT): $(OBJS_MT)
	$(CC) $(CFLAGS) -o $(BIN_MT) $(OBJS_MT) $(LFLAGS)


$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -MMD -c $< -o $@

clean:
	$(RM) $(BIN_ST) $(OBJS_ST) $(BIN_MT) $(OBJS_MT) ${OBJ_DIR}/*.d

.PHONY:  all clean


install:
	./tars/install_sdl.sh $(SDL_INSTALL_FOLDER)
