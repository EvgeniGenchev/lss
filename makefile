FILENAME 	= main
CC			= gcc
UID			= $(shell id -u)

default:compile move clean


compile:	
	${CC} ${FILENAME}.c -o lss

move:
	sudo cp lss /usr/bin/.

clean:
	rm -rf ./lss

uninstall:
	rm -rf /usr/bin/lss
