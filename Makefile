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

ALL_SAMPLE_BIN=ex01_localtime_get ex02_getch ex03_str_reverse ex04_get_module_path ex05_hostname_get ex06_str_random ex07_str_combine_path_file ex08_str_array ex09_str_split_to_array ex10_is_process_running ex11_uptime_get ex12_process_launch ex13_write_buffer_to_file ex14_read_file_to_buffer ex15_remove_file

sample: $(ALL_SAMPLE_BIN)

ex01_localtime_get: ex01_localtime_get.o sccu.o
	$(CC) $(LDFLAGS) -o $@ $^
ex01_localtime_get.o: ex01_localtime_get.c
	$(CC) $(CFLAGS) -c $<

ex02_getch: ex02_getch.o sccu.o
	$(CC) $(LDFLAGS) -o $@ $^
ex02_getch.o: ex02_getch.c
	$(CC) $(CFLAGS) -c $<

ex03_str_reverse: ex03_str_reverse.o sccu.o
	$(CC) $(LDFLAGS) -o $@ $^
ex03_str_reverse.o: ex03_str_reverse.c
	$(CC) $(CFLAGS) -c $<

ex04_get_module_path: ex04_get_module_path.o sccu.o
	$(CC) $(LDFLAGS) -o $@ $^
ex04_get_module_path.o: ex04_get_module_path.c
	$(CC) $(CFLAGS) -c $<

ex05_hostname_get: ex05_hostname_get.o sccu.o
	$(CC) $(LDFLAGS) -o $@ $^
ex05_hostname_get.o: ex05_hostname_get.c
	$(CC) $(CFLAGS) -c $<

ex06_str_random: ex06_str_random.o sccu.o
	$(CC) $(LDFLAGS) -o $@ $^
ex06_str_random.o: ex06_str_random.c
	$(CC) $(CFLAGS) -c $<

ex07_str_combine_path_file: ex07_str_combine_path_file.o sccu.o
	$(CC) $(LDFLAGS) -o $@ $^
ex07_str_combine_path_file.o: ex07_str_combine_path_file.c
	$(CC) $(CFLAGS) -c $<

ex08_str_array: ex08_str_array.o sccu.o
	$(CC) $(LDFLAGS) -o $@ $^
ex08_str_array.o: ex08_str_array.c
	$(CC) $(CFLAGS) -c $<

ex09_str_split_to_array: ex09_str_split_to_array.o sccu.o
	$(CC) $(LDFLAGS) -o $@ $^
ex09_str_split_to_array.o: ex09_str_split_to_array.c
	$(CC) $(CFLAGS) -c $<

ex10_is_process_running: ex10_is_process_running.o sccu.o
	$(CC) $(LDFLAGS) -o $@ $^
ex10_is_process_running.o: ex10_is_process_running.c
	$(CC) $(CFLAGS) -c $<

ex11_uptime_get: ex11_uptime_get.o sccu.o
	$(CC) $(LDFLAGS) -o $@ $^
ex11_uptime_get.o: ex11_uptime_get.c
	$(CC) $(CFLAGS) -c $<

ex12_process_launch: ex12_process_launch.o sccu.o
	$(CC) $(LDFLAGS) -o $@ $^
ex12_process_launch.o: ex12_process_launch.c
	$(CC) $(CFLAGS) -c $<

ex13_write_buffer_to_file: ex13_write_buffer_to_file.o sccu.o
	$(CC) $(LDFLAGS) -o $@ $^
ex13_write_buffer_to_file.o: ex13_write_buffer_to_file.c
	$(CC) $(CFLAGS) -c $<

ex14_read_file_to_buffer: ex14_read_file_to_buffer.o sccu.o
	$(CC) $(LDFLAGS) -o $@ $^
ex14_read_file_to_buffer.o: ex14_read_file_to_buffer.c
	$(CC) $(CFLAGS) -c $<

ex15_remove_file: ex15_remove_file.o sccu.o
	$(CC) $(LDFLAGS) -o $@ $^
ex15_remove_file.o: ex15_remove_file.c
	$(CC) $(CFLAGS) -c $<
