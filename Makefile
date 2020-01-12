CC := g++ # This is the main compiler
SRCDIR := src
BUILDDIR := build
TARGET := bin
OUTPUT := hidenseek
	
SRCEXT := cpp
SOURCES := $(shell find $(SRCDIR) -type f -name *.$(SRCEXT))
OBJECTS := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.o))
CFLAGS := -g # -Wall
INC := -I include

$(TARGET): $(OBJECTS)
	@echo " Linking...";
	$(CC) $^ -o $(TARGET)/$(OUTPUT)

$(BUILDDIR)/%.o: $(SRCDIR)/%.$(SRCEXT)
	$(CC) $(CFLAGS) $(INC) -c -o $@ $<

clean:
	@echo " Cleaning..."; 
	@$(RM) -r $(BUILDDIR)/* $(TARGET)/* data/* docs/*.png;

run:
	@clear
	@echo "Running the file...\n";
	@$(TARGET)/$(OUTPUT) $(ARGS)

all:
	@$(MAKE) -s clean;
	@$(MAKE) -s;
	@$(MAKE) -s run