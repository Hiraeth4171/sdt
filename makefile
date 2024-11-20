setup:
	sudo mkdir /usr/lib/include/SDT

install: 
	sudo cp ./sdt.h /usr/lib/include/SDT/sdt.h

valgrind-clean:
	rm -f vgcore.*
