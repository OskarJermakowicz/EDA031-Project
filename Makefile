SRC_DIR=src

.PHONY: all install clean

all:
	$(MAKE) -C $(SRC_DIR) all

install:
	mkdir -p bin
	cp src/client/newsclient ./bin/newsclient
	cp src/server/newsserver ./bin/newsserver

clean:
	$(MAKE) -C $(SRC_DIR) clean
	rm -rfd bin