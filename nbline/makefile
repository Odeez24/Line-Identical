dsa_dir = ../dsa/
CC = gcc
CFLAGS = -std=c18 \
  -Wall -Wconversion -Werror -Wextra -Wpedantic -Wwrite-strings \
  -O2 \
  -I$(dsa_dir)
vpath %.c $(dsa_dir)
vpath %.h $(dsa_dir)
objects = dsa.o main.o op.o
executable = lnid
makefile_indicator = .\#makefile\#

.PHONY: all clean

all: $(executable)

clean:
	$(RM) $(objects) $(executable)
	@$(RM) $(makefile_indicator)

$(executable): $(objects)
	$(CC) $(objects) -o $(executable)

bst.o: dsa.c dsa.h
main.o: main.c bst.h

include $(makefile_indicator)

$(makefile_indicator): makefile
	@touch $@
	@$(RM) $(objects) $(executable)
