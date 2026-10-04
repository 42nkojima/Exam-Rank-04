CFLAGS	= -Wall -Wextra -Werror
BINDIR	= bin

EXERCISES	= $(notdir $(wildcard level_*/*))

.PHONY: all clean $(EXERCISES)

all: $(EXERCISES)

$(EXERCISES): %: | $(BINDIR)
	$(CC) $(CFLAGS) $(wildcard level_*/$@/*.c) -o $(BINDIR)/$@

$(BINDIR):
	mkdir -p $@

clean:
	rm -rf $(BINDIR)
