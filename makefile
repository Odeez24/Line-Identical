.PHONY: clean dist

dist: clean
	tar -hzcf "$(CURDIR).tar.gz" da/* da_test/* hashtable/* holdall/* nbline/* opt/* makefile

clean:
	$(MAKE) -C nbline clean
