gcc -c Nokia/Nokia.c -o Nokia/Nokia.o -lwiringPi -lt -std=c99 -D _BSD_SOURCE
gcc -c main.c -o main.o -lwiringPi -std=c99
gcc main.o Nokia/Nokia.o -o lcd -lwiringPi -std=c99
