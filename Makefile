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
	@$(RM) -r $(BUILDDIR)/* $(TARGET)/* docs/*.png;
	@find data/ -maxdepth 2 -type f ! -name '.gitkeep' -delete

run:
	@clear
	@echo "Running the file...\n";
	@$(TARGET)/$(OUTPUT) $(ARGS)

graph:
	@echo "Generating the plot...";
	@python docs/graph.py

mean:
	@echo "Calculating means...";
	@python docs/mean.py

all:
	@$(MAKE) -s clean;
	@$(MAKE) -s;
	@$(MAKE) -s run ARGS="1 1";
	@$(MAKE) -s run ARGS="2 2";
	@$(MAKE) -s run ARGS="3 3";
	@$(MAKE) -s run ARGS="4 4";
	@$(MAKE) -s run ARGS="5 5";
	@$(MAKE) -s mean;
	@$(MAKE) -s graph;
	@echo "Finished!";