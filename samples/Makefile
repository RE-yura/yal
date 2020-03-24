SRCDIR = .
SUBDIRS = $(filter-out . ./.vscode, $(shell find $(SRCDIR) -type d))

all:
	$(foreach dir, $(SUBDIRS), $(MAKE) -C $(dir);)
#@echo $(SUBDIRS)
 
clean:
	$(foreach dir, $(SUBDIRS), $(MAKE) -C $(dir) clean;)
