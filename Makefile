all: libsccu sample

SYMBONAME = libsccu.so
MAJOR = .1
MINOR = .0.0
SONAME = $(SYMBONAME)$(MAJOR)
FILENAME = $(SONAME)$(MINOR)

DEPEND = sccu.o

libsccu: $(DEPEND)
	$(CC) $(CFLAGS) -shared -fPIC -Wl,-soname,$(SONAME) -o $(FILENAME) $^ -lc
	ln -sf $(FILENAME) $(SONAME) ; \
	ln -sf $(SONAME) $(SYMBONAME)

sccu.o: sccu.c
	$(CC) $(CFLAGS) -fPIC -c $<

install:
	mkdir -p $(DESTDIR)/usr/lib
	mkdir -p $(DESTDIR)/usr/include
	cp -f $(FILENAME) $(DESTDIR)/usr/lib
	cp -f sccu.h $(DESTDIR)/usr/include
	cd $(DESTDIR)/usr/lib ; \
	ln -sf $(FILENAME) $(SONAME) ; \
	ln -sf $(SONAME) $(SYMBONAME)

clean:
	rm -f *.o libsccu.so* $(ALL_SAMPLE_BIN)

ALL_SAMPLE_BIN=ex01_localtime_get ex02_getch

sample: $(ALL_SAMPLE_BIN)

ex01_localtime_get: ex01_localtime_get.o sccu.o
	$(CC) $(LDFLAGS) -o $@ $^
ex01_localtime_get.o: ex01_localtime_get.c
	$(CC) $(CFLAGS) -c $<

ex02_getch: ex02_getch.o sccu.o
	$(CC) $(LDFLAGS) -o $@ $^
ex02_getch.o: ex02_getch.c
	$(CC) $(CFLAGS) -c $<
