NPROC=$(nproc)

gcc -fPIC -std=c99 -o main.o main.c -lpthread -lm -fopenmp
echo "Complilation complete. Running program:"
time ./main.o $1 $2 $NPROC
python sor.py $2
