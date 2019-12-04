CC = gcc
CXX = g++
ECHO = echo
RM = rm
TAR = tar cvfz

CFLAGS = -Wall -ggdb3
CPPFLAGS = -Wall -ggdb3 -lpthread

BIN = redBlackTest
OBJS = redBlackTest.o redBlack.o parser.o treeBuilder.o reportWriter.o

all: $(BIN)

$(BIN): $(OBJS) redBlackTest.o
	@$(ECHO) Linking $@
	$(CXX) $^ -o $@ -Wall -ggdb3

-include $(OBJS:.o=.d)

%.o: %.cpp %.h
	@$(ECHO) Compiling $<
	$(CXX) -Wall -ggdb3 -MMD -MF $*.d -c $<

#Clean up!
clean:
	@$(ECHO) Deleting the junk
	@$(RM) *.o $(BIN) *.d