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
	@find data/ -maxdepth 1 -type f ! -name '.gitkeep' -delete

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
	@$(MAKE) -s run;
	@$(MAKE) -s run ARGS="0 1";
	@$(MAKE) -s run ARGS="0 2";
	@$(MAKE) -s run ARGS="0 3";
	@$(MAKE) -s run ARGS="0 4";
	@$(MAKE) -s run ARGS="0 5";
	@$(MAKE) -s run ARGS="1 0";
	@$(MAKE) -s run ARGS="1 1";
	@$(MAKE) -s run ARGS="1 2";
	@$(MAKE) -s run ARGS="1 3";
	@$(MAKE) -s run ARGS="1 4";
	@$(MAKE) -s run ARGS="1 5";
	@$(MAKE) -s run ARGS="2 0";
	@$(MAKE) -s run ARGS="2 1";
	@$(MAKE) -s run ARGS="2 2";
	@$(MAKE) -s run ARGS="2 3";
	@$(MAKE) -s run ARGS="2 4";
	@$(MAKE) -s run ARGS="2 5";
	@$(MAKE) -s run ARGS="3 0";
	@$(MAKE) -s run ARGS="3 1";
	@$(MAKE) -s run ARGS="3 2";
	@$(MAKE) -s run ARGS="3 3";
	@$(MAKE) -s run ARGS="3 4";
	@$(MAKE) -s run ARGS="3 5";
	@$(MAKE) -s run ARGS="4 0";
	@$(MAKE) -s run ARGS="4 1";
	@$(MAKE) -s run ARGS="4 2";
	@$(MAKE) -s run ARGS="4 3";
	@$(MAKE) -s run ARGS="4 4";
	@$(MAKE) -s run ARGS="4 5";
	@$(MAKE) -s run ARGS="5 0";
	@$(MAKE) -s run ARGS="5 1";
	@$(MAKE) -s run ARGS="5 2";
	@$(MAKE) -s run ARGS="5 3";
	@$(MAKE) -s run ARGS="5 4";
	@$(MAKE) -s run ARGS="5 5";
	# @$(MAKE) -s mean;
	# @$(MAKE) -s graph;
	@echo "Finished!";