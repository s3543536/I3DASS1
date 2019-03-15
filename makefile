SOURCES=asdf.c
OBJECTS=$(SOURCES:.c=.o)
HEADERS=$(SOURCES:.c=.h)
DFILES=$(SOURCES:.c=.d)
PROGRAM=a.out
DFILERULES=-MMD -MP
CFLAGS= $(DFILERULES)
LDFLAGS=$(CFLAGS) -lGL -lGLU -lglut -lm 
CC=gcc


all: $(PROGRAM)

%.o: %.c $(HEADERS)
	gcc $(CFLAGS) -c $< -o $@

$(PROGRAM): $(OBJECTS)
	gcc $(LDFLAGS) -o $@ $^

-include $(SOURCES:.c=.d)
.PHONY:clean debug rebuild

sanitize:debug
sanitize:CFLAGS+= -fsanitize=address
sanitize:LDFLAGS+= -fsanitize=address

debug:clean all
debug:CFLAGS+= -g
debug:LDFLAGS+= -g

clean:
	-rm $(PROGRAM) $(OBJECTS) $(DFILES)

rebuild:clean all
