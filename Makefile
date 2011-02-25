# Makefile for GNU make
#
# Common Unix flags
#EXECUTABLE=myradclient
EXECUTABLE=${CND_DISTDIR}/myradclient
CFLAGS=-MMD -DWITH_OPENSSL -DWITH_GZIP -D_FILE_OFFSET_BITS=64 -DMT -DLB_MODULE_NAME=\"$(EXECUTABLE)\" -fomit-frame-pointer

LDFLAGS=
#/usr/lib/libboost_thread-mt.a -L/usr/local/instantclient_11_2 -locci -lclntsh -lm -lz -lcrypto -lssl -lrt
CC=g++
FOUT=-o
LINK=$(CC) -o $(BUILD_DIR)/$(EXECUTABLE)
STRIP=strip
BUILD_DIR=build
#SUBDIRS=collect_data settings dbconnection db_tables common api server_thread sha1 soap_wrapper
SUBDIRS=
DEPS=$(BUILD_DIR)/main.o $(BUILD_DIR)/config.o $(BUILD_DIR)/util.o $(BUILD_DIR)/clientid.o $(BUILD_DIR)/dict.o $(BUILD_DIR)/sendserver.o \
$(BUILD_DIR)/avpair.o $(BUILD_DIR)/ip_util.o $(BUILD_DIR)/buildreq.o $(BUILD_DIR)/lock.o $(BUILD_DIR)/md5.o

OBJECTS := $(wildcard $(BUILD_DIR)/*.o)
VPATH=
#../Lblib:../xmlapi:../Lblib/common
LD_LIBRARY_PATH=
#/usr/local/instantclient_11_2
ALLD=alld

##### OS detect
OS=$(shell uname)
ifeq ($(OS), Linux)
# Linux stuff
CFLAGS+=-DLINUX
LDFLAGS+=-ldl
MAKE=make
else
$(error Unknown OS)
endif
##### end of OS detect

# Detect platform
MACHINE=$(shell uname -m)
ifeq ($(MACHINE), i686)
LDFLAGS+=/usr/lib/gcc/i486-linux-gnu/4.4.3/libstdc++.a
else
ifeq ($(MACHINE), x86_64)
LDFLAGS+=/usr/lib/gcc/x86_64-pc-linux-gnu/4.3.4/libstdc++.a
else
$(error Unknown OS)
endif
endif

# Add debug flags for 'make debug'
CFLAGS+=-ggdb3
ifdef DEBUG
CFLAGS+=-DDEBUG -DNOCHECKLIC -Wall -O0
else
CFLAGS+=-O2 -DNDEBUG -w
endif

CFLAGS+=$(LBFLAGS)
export CC
export CFLAGS
export BUILD_DIR
export FOUT
export LD_LIBRARY_PATH

release: all strip

.PHONY: strip

strip: $(BUILD_DIR)/$(EXECUTABLE).debuginfo

$(BUILD_DIR)/$(EXECUTABLE).debuginfo: $(OBJECTS)
	objcopy --only-keep-debug $(BUILD_DIR)/$(EXECUTABLE) $(BUILD_DIR)/$(EXECUTABLE).debuginfo
	$(STRIP) $(BUILD_DIR)/$(EXECUTABLE)
	objcopy --add-gnu-debuglink $(BUILD_DIR)/$(EXECUTABLE).debuginfo $(BUILD_DIR)/$(EXECUTABLE)

.PHONY: subdirs $(SUBDIRS)

subdirs: $(SUBDIRS)

$(SUBDIRS):
	$(MAKE) -C $@

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

.PHONY: all
all: $(BUILD_DIR) subdirs $(BUILD_DIR)/$(EXECUTABLE) 

$(BUILD_DIR)/$(EXECUTABLE): $(DEPS) $(OBJECTS)
	$(LINK) $(BUILD_DIR)/*.o $(LDFLAGS)

$(BUILD_DIR)/%.o: %.cpp
	$(CC) $(CFLAGS) -c -MF $(BUILD_DIR)/$*.d $< $(FOUT)$@
	@cp $(BUILD_DIR)/$*.d $(BUILD_DIR)/$*.P; \
		sed -e 's/#.*//' -e 's/^[^:]*: *//' \
			-e 's/ *\\$$//' -e '/^$$/ d' -e 's/$$/ :/' \
			< $(BUILD_DIR)/$*.d >> $(BUILD_DIR)/$*.P; \
		rm -f $(BUILD_DIR)/$*.d

$(BUILD_DIR)/%.o: %.c
	$(CC) $(CFLAGS) -c -MF $(BUILD_DIR)/$*.d $< $(FOUT)$@
	@cp $(BUILD_DIR)/$*.d $(BUILD_DIR)/$*.P; \
		sed -e 's/#.*//' -e 's/^[^:]*: *//' \
			-e 's/ *\\$$//' -e '/^$$/ d' -e 's/$$/ :/' \
			< $(BUILD_DIR)/$*.d >> $(BUILD_DIR)/$*.P; \
		rm -f $(BUILD_DIR)/$*.d

-include $(DEPS:%.o=%.P)

debug:
	DEBUG=1 $(MAKE) all
	
clean:
	rm -rf $(BUILD_DIR) $(EXECUTABLE) $(EXECUTABLE).debuginfo
