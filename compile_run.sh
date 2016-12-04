NPROC=$(nproc)

gcc -fPIC -std=c99 -o main.o main.c -lpthread -lm
echo "Complilation complete. Running program:"
time ./main.o $1 $1 $NPROC
#python sor.py
