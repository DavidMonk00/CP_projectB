gcc -std=c99 thread2.c -o thread2.o -lpthread -lm
echo 'Compilation complete. Running code...'
time ./thread2.o
