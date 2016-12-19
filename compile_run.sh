NPROC=$(nproc)

gcc -W -fPIC -std=c99 -o main.o main.c -lpthread -lm -fopenmp
echo "Complilation complete. Running program:"
time ./main.o $1 $2 $3 $4 $NPROC
python sor.py $3
