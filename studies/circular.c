#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
    int num_cadeiras;
    int i;
    int esq;
    int dir;


    if (argc != 2)
        return (1);

    num_cadeiras = atoi(argv[1]);
    i = 0;
    while ( i < num_cadeiras)
    {
        esq = i;
        dir = (i + 1) % num_cadeiras;
        printf("Coder %d | Dongle Esq: %d | Dongle Dir: %d\n", i, esq, dir);
        i++;    
    }

    return (0);
}
