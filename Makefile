# https://en.wikipedia.org/wiki/Makefile

SRCDIR 			:= .
OBJDIR			:= ./out
EXECUTABLE    	:= renderer
SRCFILES	 	:= $(shell find $(SRCDIR) -name "*.c")
SRCNAMES		:= $(notdir $(SRCFILES))
OBJFILES 	    := $(SRCNAMES:%.c=$(OBJDIR)/%.o)
LDFLAGS       	:= -lX11 -lXi -lSDL2
space 			:=
VPATH 			:= $(subst $(space),:,$(shell find . -type d))
GCCFLAGS      	:= -std=c17

.PHONY: all
all: out/$(EXECUTABLE)

out/$(EXECUTABLE): $(OBJFILES)
	gcc $(GCCFLAGS) $(OBJFILES) -o $@ $(LDFLAGS)

$(OBJDIR)/%.o: %.c
	gcc $(GCCFLAGS) -c $< -o $@


.PHONY: clean
clean:
	@rm -f out/* && echo "[CL]  out/"