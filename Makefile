# https://en.wikipedia.org/wiki/Makefile

SRCDIR 			:= ./src
OBJDIR			:= ./out

EXECUTABLE    	:= renderer
SRCFILES	 	:= $(shell find $(SRCDIR) -name "*.c")
SRCNAMES 		:= $(filter-out test%.c, $(notdir $(SRCFILES)))
OBJFILES 	    := $(SRCNAMES:%.c=$(OBJDIR)/%.o)

TSTEXECUTABLE 	:= test_renderer
TSTSRCNAMES 	:= $(filter-out main.c, $(notdir $(SRCFILES)))
TSTOBJFILES 	:= $(TSTSRCNAMES:%.c=$(OBJDIR)/%.o)

# LDFLAGS       	:= -lX11 -lXi -lSDL2 -lm
LDFLAGS       	:= -lX11 -lXi -lm -lX11
space 			:=
VPATH 			:= $(subst $(space),:,$(shell find . -type d))
GCCFLAGS      	:= -std=c17 -Wall -Wno-pointer-sign -Wextra -Wshadow

.PHONY: all
all: out/$(EXECUTABLE) out/$(TSTEXECUTABLE)

out/$(EXECUTABLE): $(OBJFILES)
	gcc $(GCCFLAGS) $(OBJFILES) -o $@ $(LDFLAGS)

out/$(TSTEXECUTABLE): $(TSTOBJFILES)
	gcc $(GCCFLAGS) $(TSTOBJFILES) -o $@ $(LDFLAGS)

$(OBJDIR)/%.o: %.c
	gcc $(GCCFLAGS) -c $< -o $@


.PHONY: clean
clean:
	@rm -f out/* && echo "[CL]  out/"