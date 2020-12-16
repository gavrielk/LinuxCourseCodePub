# Variables declaration
# every variable can potentially also be an array of strings
# by seperating the strings with space e.g. LIBS
CC = gcc
DEB_DIR = mta-utils-dev
SOS_DIR = $(DEB_DIR)/usr/lib/x86_64-linux-gnu
SOS = $(SOS_DIR)/libmta_rand.so $(SOS_DIR)/libmta_crypt.so
SOS_LIBS = $(patsubst lib%.so,-l%, $(notdir $(SOS)))
SOS_HEADERS_DIR = $(DEB_DIR)/usr/include/

.SECONDARY:

$(SOS_DIR)/lib%.so: %.c
	mkdir -p $(SOS_DIR)
	mkdir -p $(SOS_HEADERS_DIR)
	$(CC) -g -shared -fPIC $< -o $@ $(LIBS)
	$(eval HEADER := $(patsubst %.c,%.h,$<))
	cp $(HEADER) $(SOS_HEADERS_DIR)

.PHONY: all
all:  $(SOS)
	dpkg-deb --build $(DEB_DIR)

.PHONY: clean
clean:
	rm -rf ./*.o*
	rm -rf $(DEB_DIR)/usr
	rm -rf $(DEB_DIR).deb
