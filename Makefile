CC = gcc
CXX = g++
ECHO = echo
RM = rm
TAR = tar cvfz

CFLAGS = -Wall -ggdb3
CPPFLAGS = -Wall -ggdb3 -lpthreads

BIN = redBlackTest
OBJS = redBlackTest.o

all: $(BIN)

$(BIN): $(OBJS) redBlackTest.o
	@$(ECHO) Linking $@
	@g++ $^ -o $@ -Wall -ggdb3

-include $(OBJS:.o=.d)

%.cpp.o: %.cpp %.h
	@$(ECHO) Compiling $<
	@g++ -Wall -ggdb3 -MMD -MF $*.d -c $<

#Clean up!
clean:
	@$(ECHO) Deleting the junk
	@$(RM) *.o $(BIN) *.d