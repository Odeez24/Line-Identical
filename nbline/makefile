da_dir = ../da/
holdall_dir = ../holdall/
hashtable_dir = ../hashtable/
opt_dir = ../opt/
CC = gcc
CFLAGS = -std=c18 \
  -Wall -Wconversion -Werror -Wextra -Wpedantic -Wwrite-strings \
  -O2 \
  -I$(da_dir) -I$(holdall_dir) -I$(hashtable_dir) -I$(opt_dir)
vpath %.c $(da_dir) $(holdall_dir) $(hashtable_dir) $(opt_dir)
vpath %.h $(da_dir)  $(holdall_dir) $(hashtable_dir) $(opt_dir)
objects = da.o main.o hashtable.o holdall.o opt.o
executable = lnid
makefile_indicator = .\#makefile\#

.PHONY: all clean

all: $(executable)

clean:
	$(RM) $(objects) $(executable)
	@$(RM) $(makefile_indicator)

$(executable): $(objects)
	$(CC) $(objects) -o $(executable)

opt.o: opt.c opt.h
da.o: da.c da.h
holdall.o: holdall.c holdall.h
hashtable.o: hashtable.c hashtable.h
main.o: main.c da.h hashtable.h holdall.h opt.h

include $(makefile_indicator)

$(makefile_indicator): makefile
	@touch $@
	@$(RM) $(objects) $(executable)
