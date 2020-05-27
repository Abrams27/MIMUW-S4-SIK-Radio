CC = g++-9
CFLAGS = -Wall -Wextra -O2
LDFLAGS =

.PHONY: all tests clean

all: radio-proxy tests


radio-proxy: radio-proxy.o
	$(CC) $(LDFLAGS) -o $@ $^

radio-proxy.o: src/main/cc/radio-proxy.cc
	$(CC) $(CFLAGS) -c $<


tests: test1 test2

test1: test1.o
	$(CC) $(LDFLAGS) -o $@ $^

test1.o: src/test/cc/test1.cc
	$(CC) $(CFLAGS) -c $<

test2: test2.o
	$(CC) $(LDFLAGS) -o $@ $^

test2.o: src/test/cc/test2.cc
	$(CC) $(CFLAGS) -c $<


clean:
	rm -f *.o radio-proxy test1 test2