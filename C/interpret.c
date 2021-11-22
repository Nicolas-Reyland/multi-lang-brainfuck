#include <stdlib.h>
#include <stdio.h>

#define BUFSIZE 4096
#define DATA_SIZE 30000

void brainfuck(char *src_code, ssize_t src_code_size, int* data, ssize_t data_size);

int main(int argc, char ** argv)
{
    // Verify usage of script
    if (argc != 2) {
        fprintf(stderr, "Usage: interpret input-file\n");
        exit(EXIT_FAILURE);
    }

    // Read input file content
    FILE *f = fopen(argv[1], "rb");
    fseek(f, 0, SEEK_END);
    long fsize = ftell(f);
    fseek(f, 0, SEEK_SET);  /* same as rewind(f); */

    char *src_code = malloc(fsize + 1);
    fread(src_code, 1, fsize, f);
    fclose(f);

    src_code[fsize] = 0;

    // Allocate data
    int* data = calloc(DATA_SIZE, sizeof(int));

    // interpret
    brainfuck(src_code, fsize, data, DATA_SIZE);
}

void brainfuck(char *src_code, ssize_t src_code_size, int* data, ssize_t data_size)
{
    ssize_t data_ptr = 0;
    ssize_t code_ptr = 0;
    while (code_ptr != src_code_size)
    {
        switch (src_code[code_ptr])
        {
            case '>':
                data_ptr++;
                if (data_ptr == data_size) {
                    fprintf(stderr, "Data pointer overflow. Fixed data size: %l\n", data_size);
                    exit(EXIT_FAILURE);
                }
                break;
            case '<':
                data_ptr--;
                if (data_ptr == -1) {
                    fprintf(stderr, "Negative data pointer (-1).\n");
                    exit(EXIT_FAILURE);
                }
                break;
            case '+':
                break;
            case '-':
                break;
            case '.':
                break;
            case ',':
                break;
            case '[':
                break;
            case ']':
                break;
            default:
                fprintf(stderr, "Invalid char: %c\n", src_code[code_ptr]);
                exit(EXIT_FAILURE);
                break;
        }
        code_ptr++;
    }
}
