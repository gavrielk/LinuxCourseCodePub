CC = gcc
LIBS := 
DEB_DIR = mta-utils-dev
ARCH ?= $(shell uname -p)
SOS_DIR = $(DEB_DIR)/usr/lib/$(ARCH)-linux-gnu
SOS = $(SOS_DIR)/libmta_rand.so $(SOS_DIR)/libmta_crypt.so
SOS_LIBS = $(patsubst lib%.so,-l%, $(notdir $(SOS)))
SOS_HEADERS_DIR = $(DEB_DIR)/usr/include/

.SECONDARY:

$(SOS_DIR)/lib%.so: %.c
	mkdir -p $(SOS_DIR)
	mkdir -p $(SOS_HEADERS_DIR)
	$(if $(findstring crypt,$<), $(eval LIBS += -lcrypto),)
	$(CC) -g -shared -fPIC $< -o $@ $(LIBS)
	$(eval HEADER := $(patsubst %.c,%.h,$<))
	cp $(HEADER) $(SOS_HEADERS_DIR)

.PHONY: all
all:  $(SOS)
	dpkg-deb --build $(DEB_DIR)
	mv $(DEB_DIR).deb $(DEB_DIR)-$(ARCH).deb

.PHONY: clean
clean:
	rm -rf ./*.o*
	rm -rf $(DEB_DIR)/usr
	rm -rf $(DEB_DIR)-$(ARCH).deb
