setup:
	sudo mkdir -p /usr/lib/include/SDT

install: 
	sudo cp ./SDT/* /usr/lib/include/SDT/

valgrind-clean:
	rm -f vgcore.*
