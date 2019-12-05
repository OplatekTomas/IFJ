OBJS	= astnode.o code_gen.o error.o indent_stack.o keywords.o lex_analyzer.o main.o semantic_analyzer.o symtable.o syntax_analyzer.o syntax_stack.o tokens.o
SOURCE	= astnode.c code_gen.c error.c indent_stack.c keywords.c lex_analyzer.c main.c semantic_analyzer.c symtable.c syntax_analyzer.c syntax_stack.c tokens.c
HEADER	= astnode.h code_gen.h error.h indent_stack.h keywords.h lex_analyzer.h semantic_analyzer.h symtable.h syntax_analyzer.h syntax_stack.h tokens.h
OUT	= IFJ
CC	 = gcc
FLAGS	 = -g -c -Wall -DNDEBUG
LFLAGS	 = 

all: $(OBJS)
	$(CC) -g $(OBJS) -o $(OUT) $(LFLAGS)

astnode.o: astnode.c
	$(CC) $(FLAGS) astnode.c -std=c99

code_gen.o: code_gen.c
	$(CC) $(FLAGS) code_gen.c -std=c99

error.o: error.c
	$(CC) $(FLAGS) error.c -std=c99

indent_stack.o: indent_stack.c
	$(CC) $(FLAGS) indent_stack.c -std=c99

keywords.o: keywords.c
	$(CC) $(FLAGS) keywords.c -std=c99

lex_analyzer.o: lex_analyzer.c
	$(CC) $(FLAGS) lex_analyzer.c -std=c99

main.o: main.c
	$(CC) $(FLAGS) main.c -std=c99

semantic_analyzer.o: semantic_analyzer.c
	$(CC) $(FLAGS) semantic_analyzer.c -std=c99

symtable.o: symtable.c
	$(CC) $(FLAGS) symtable.c -std=c99

syntax_analyzer.o: syntax_analyzer.c
	$(CC) $(FLAGS) syntax_analyzer.c -std=c99

syntax_stack.o: syntax_stack.c
	$(CC) $(FLAGS) syntax_stack.c -std=c99

tokens.o: tokens.c
	$(CC) $(FLAGS) tokens.c -std=c99


clean:
	rm -f $(OBJS) $(OUT)
