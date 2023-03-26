dsa_dir = ../dsa/
holdall_dir = ../holdall/
hashtable_dir = ../hashtable/
CC = gcc
CFLAGS = -std=c18 \
  -Wall -Wconversion -Werror -Wextra -Wpedantic -Wwrite-strings \
  -O2 \
  -I$(dsa_dir) -I$(holdall_dir) -I$(hashtable_dir) \
vpath %.c $(dsa_dir) $(holdall_dir) $(hashtable_dir)
vpath %.h $(dsa_dir) $(holdall_dir) $(hashtable_dir)
objects = dsa.o main.o hashtable.o holdall.o
executable = lnid
makefile_indicator = .\#makefile\#

.PHONY: all clean

all: $(executable)

clean:
	$(RM) $(objects) $(executable)
	@$(RM) $(makefile_indicator)

$(executable): $(objects)
	$(CC) $(objects) -o $(executable)

dsa.o: dsa.c dsa.h
holdall.o: holdall.c holdall.h
hashtable.o: hashtable.c hashtable.h
main.o: main.c dsa.h hashtable.h holdall.h

include $(makefile_indicator)

$(makefile_indicator): makefile
	@touch $@
	@$(RM) $(objects) $(executable)
