CC=gcc
FLAGS=-lreadline

SRCDIR=./src
OBJDIR=./obj

TARGET=yash

all: yash

yash: yash.o history.o pipes.o tokenizer.o cd.o prompt.o builtins.o separator.o
	$(CC) -o $(TARGET) $(OBJDIR)/yash.o $(OBJDIR)/history.o $(OBJDIR)/pipes.o $(OBJDIR)/tokenizer.o $(OBJDIR)/cd.o $(OBJDIR)/prompt.o $(OBJDIR)/builtins.o $(OBJDIR)/separator.o $(FLAGS)

yash.o: ./src/yash.c ./src/pipes.h ./src/tokenizer.h ./src/builtins.h
	$(CC) -c ./src/yash.c -o $(OBJDIR)/yash.o

separator.o: ./src/separator.c ./src/separator.h ./src/tokenizer.h ./src/pipes.h
	$(CC) -c ./src/separator.c -o $(OBJDIR)/separator.o

builtins.o: ./src/builtins.c ./src/builtins.h ./src/history.h ./src/cd.h
	$(CC) -c ./src/builtins.c -o $(OBJDIR)/builtins.o

history.o: ./src/history.c ./src/history.h
	$(CC) -c ./src/history.c -o $(OBJDIR)/history.o

pipes.o: ./src/pipes.c ./src/pipes.h ./src/tokenizer.h
	$(CC) -c ./src/pipes.c -o $(OBJDIR)/pipes.o

tokenizer.o: ./src/tokenizer.c ./src/tokenizer.h
	$(CC) -c ./src/tokenizer.c -o $(OBJDIR)/tokenizer.o

cd.o: ./src/cd.c ./src/cd.h
	$(CC) -c ./src/cd.c -o $(OBJDIR)/cd.o

prompt.o: ./src/prompt.c ./src/prompt.h
	$(CC) -c ./src/prompt.c -o $(OBJDIR)/prompt.o

clean:
	rm $(OBJDIR)/*.o yash