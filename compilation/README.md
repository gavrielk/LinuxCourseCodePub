gcc -shared -fPIC utils.c -o libutils.so

gcc main.c -L`pwd` -lutils -o main.out

export LD_LIBRARY_PATH=`pwd`
./main.out
