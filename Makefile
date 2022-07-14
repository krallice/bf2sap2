CC=g++
CFLAGS=-fdiagnostics-color=always -g -Wall -std=c++2a -lstdc++

LINKER=g++ -g -o
LFLAGS=-Wall -I. -lm

SRCDIR=src
BINDIR=bin
OBJDIR=obj

TARGET=bfc

SOURCES  := $(wildcard $(SRCDIR)/*.cpp)
INCLUDES := $(wildcard $(SRCDIR)/*.h)
OBJECTS := $(SOURCES:$(SRCDIR)/%.cpp=$(OBJDIR)/%.o)
DEPS := $(OBJECTS:%.o=%.d)

$(BINDIR)/$(TARGET): $(OBJECTS)
	    @$(LINKER) $@ $(LFLAGS) $(OBJECTS)
	        @echo "Linking complete!"

-include $(DEPS)

$(OBJECTS): $(OBJDIR)/%.o : $(SRCDIR)/%.cpp
	    @$(CC) -MMD $(CFLAGS) -c $< -o $@
	    @echo "Compiled "$<" successfully!"


.PHONY : clean
clean:
	@- $(RM) $(OBJECTS) $(DEPS)
	@- $(RM) $(BINDIR)/$(TARGET)
	@echo "Cleanup Complete!"