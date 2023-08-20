TARGET = vm_riskxvii


CC = gcc
#CFLAGS     = -c -Wall -Wvla -Werror -O0 -g -std=c11
CFLAGS     = -c -Os -std=c11
#ASAN_FLAGS = -fsanitize=leak,address
SRC        = vm_riskxvii.c getInstructionSet.c OperationSet.c heapMemoryFuncSet.c
OBJ        = $(SRC:.c=.o)
COMPRESS=gzip

all:$(TARGET) 

$(TARGET):$(OBJ)
	$(CC) -o $@ $(OBJ)
	
.SUFFIXES: .c .o

.c.o:
	 $(CC) $(CFLAGS) $<


run:
	./$(TARGET) $1

test:
	echo what are we testing?!
	bash testScript.sh

coverage:
	$(CC) --coverage $(SRC) -o $(TARGET)
	
coverageReport:
	gcov $(SRC)	
clean:
	rm -f *.o *.obj *.gcov *.gz *.gcno *.gcda $(TARGET)