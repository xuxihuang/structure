
CC=gcc 

btree_test:btree_test.o btree.o
	$(CC) -o btree_test -g $^

btree_test.o:btree_test.c
	$(CC) -c -g -o $@ $<
btree.o:btree.c
	$(CC) -c -g -o $@ $<

clean:
	rm btree_test.o btree.o -rf
	rm btree_test -rf
	
