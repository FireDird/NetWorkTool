CC := g++

CFLAGS += -ggdb3  \
        -Wall \
        -lcurl
.PHONY: clean

OBJ := main.o 
AITRANSFORM:$(OBJ)
	$(CC)   $^  -o $@  $(CFLAGS)
%.o:%.cpp
	$(CC)   -c $^ -o $@  $(CFLAGS)
clean:
	$(RM) AITRANSFORM $(OBJ)
