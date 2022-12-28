TARGET=alarmpj-daemon
CC=gcc
WORKDIR=$(shell pwd)
WORKDIR_WINDOW=$(WORKDIR)/src/window
WORKDIR_FUNCTIONS=$(WORKDIR)/src/functions
WORKDIR_DAEMON=$(WORKDIR)/src/daemon
WORKDIR_INTERFACE=$(WORKDIR)/src/interface
LOG=/var/log/Logfile.log
SRC = $(wildcard */*.c)
LIBS = -lm -lalarm

# На релизе необходимо вместо daemon - install (+ расскомментить строки ниже)
$(TARGET): daemon interface

window:
	sudo qmake -o $(WORKDIR_WINDOW)/Makefile $(WORKDIR_WINDOW)/alarmpj.pro
	sudo $(MAKE) -C $(WORKDIR_WINDOW) install
	sudo mv $(WORKDIR_WINDOW)/alarmpj -t $(WORKDIR)/bin/
	rm -f $(WORKDIR_WINDOW)/*.o $(WORKDIR_WINDOW)/Makefile $(WORKDIR_WINDOW)/moc*.* $(WORKDIR_WINDOW)/ui*.h $(WORKDIR_WINDOW)/.qmake.stash

staticLib:
	$(CC) -c $(WORKDIR_FUNCTIONS)/printInLog.c $(WORKDIR_FUNCTIONS)/forkForWindow.c
	ar rc libalarm.a printInLog.o forkForWindow.o 
	ranlib libalarm.a
	mv $(WORKDIR)/libalarm.a -t $(WORKDIR)/lib/

daemon: window staticLib
	$(CC) -c $(WORKDIR_DAEMON)/alarmd.c
	$(CC) alarmd.o -L $(WORKDIR)/lib $(LIBS) -o alarmd
	sudo mv $(WORKDIR)/alarmd -t $(WORKDIR)/bin/
	sudo touch $(LOG)
	sudo chmod 777 $(LOG)
	rm -f *.o

interface: 
	$(CC) $(WORKDIR_INTERFACE)/alarmpj.c -std=c11 -o alarmpj
	echo $(WORKDIR)/bin/config.conf > /var/tmp/configdir.txt
	sudo mv $(WORKDIR)/alarmpj -t /usr/bin/

#install:
#	bash $(WORKDIR)/src/daemon/integration.sh
uninstall: clean

clean: 
	sudo rm -f $(WORKDIR)/bin/alarm* $(WORKDIR)/lib/lib*.a $(WORKDIR)/alarm* /usr/bin/alarmpj
