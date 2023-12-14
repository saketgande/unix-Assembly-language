CC=gcc
LEX=flex
YACC=bison
BIN_DIR=bin
SRC_DIR=lexyacc-code
YFLAGS=-y -d
LFLAGS=

all: $(BIN_DIR)/calc3i.exe

$(SRC_DIR)/y.tab.c $(SRC_DIR)/y.tab.h: $(SRC_DIR)/calc3.y
	$(YACC) $(YFLAGS) $<
	mv y.tab.c $(SRC_DIR)/y.tab.c
	mv y.tab.h $(SRC_DIR)/y.tab.h

$(SRC_DIR)/lex.yy.c: $(SRC_DIR)/calc3.l
	$(LEX) $(LFLAGS) -o $@ $<

$(BIN_DIR)/calc3i.exe: $(SRC_DIR)/y.tab.c $(SRC_DIR)/lex.yy.c $(SRC_DIR)/calc3i.c
	$(CC) $^ -o $@ -lfl  # Add -lfl to link with Flex's library

clean:
	rm -f $(SRC_DIR)/*.o $(SRC_DIR)/lex.yy.c $(SRC_DIR)/y.tab.c $(SRC_DIR)/y.tab.h $(BIN_DIR)/*.exe $(BIN_DIR)/*