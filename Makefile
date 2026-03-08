CC = cc
CFLAGS = -std=c11 -Wall -Wextra -Werror

TARGET = robdd_app
SRC = main.c parser.c robdd.c robdd_table.c robdd_ops.c robdd_export.c
OBJ = $(SRC:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ)

%.o: %.c robdd.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)

cleanall: clean
	rm -f ./*.dot ./*.png
