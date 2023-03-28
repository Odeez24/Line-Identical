da_dir = ../da/
holdall_dir = ../holdall/
hashtable_dir = ../hashtable/
CC = gcc
CFLAGS = -std=c18 \
  -Wall -Wconversion -Werror -Wextra -Wpedantic -Wwrite-strings \
  -O2 \
  -I$(da_dir) -I$(holdall_dir) -I$(hashtable_dir)
vpath %.c $(da_dir) $(holdall_dir) $(hashtable_dir)
vpath %.h $(da_dir)  $(holdall_dir) $(hashtable_dir)
objects = da.o dsa.o main.o hashtable.o holdall.o
executable = lnid
makefile_indicator = .\#makefile\#

.PHONY: all clean

all: $(executable)

clean:
	$(RM) $(objects) $(executable)
	@$(RM) $(makefile_indicator)

$(executable): $(objects)
	$(CC) $(objects) -o $(executable)

da.o: da.c da.h
holdall.o: holdall.c holdall.h
hashtable.o: hashtable.c hashtable.h
main.o: main.c da.h hashtable.h holdall.h

include $(makefile_indicator)

$(makefile_indicator): makefile
	@touch $@
	@$(RM) $(objects) $(executable)
