CC   = gcc
OBJ  = ./obj
SRC  = ./src
INC  = -I./include
OBJS = $(addprefix $(OBJ), $(subst .c,.o, $(subst $(SRC),,$(wildcard $(SRC)/*.c))))

all:
	mkdir -p $(OBJ)
	make editor

editor: $(OBJS)
	$(CC) -o $@ $^

$(OBJ)/%.o: $(SRC)/%.c
	$(CC) -o $@ -c $^ $(INC)

.PHONY: clean

clean:
	rm -f editor $(OBJ)/*.o
