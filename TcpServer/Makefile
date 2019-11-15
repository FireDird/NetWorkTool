CC := g++

CFLAGS += -ggdb3  \
        -Wall \
.PHONY: clean

OBJ := main.o 
TcpServer:$(OBJ)
	$(CC)   $^  -o $@  $(CFLAGS)
%.o:%.cpp
	$(CC)   -c $^ -o $@  $(CFLAGS)
clean:
	$(RM) TcpServer $(OBJ)
