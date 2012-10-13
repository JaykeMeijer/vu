#include <stdio.h>

void show_options()
{
    fputs("INFO\n", stdout);
}


int main(int argc, const char *argv[])
{
    int i;

    if(argc < 2)
        show_options();
    else {
        for(i = 1; i < argc; i++)
            printf("%s\n", argv[i]);


    }
    return 0;
}
