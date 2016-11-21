gcc -fPIC -std=c99 -o main.o main.c `pkg-config --cflags --libs gsl` -lpthread

echo "Complilation complete. Running program:"
time ./main.o $1 $2
python sor.py
