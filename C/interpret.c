#include <stdlib.h>
#include <stdio.h>

#define BUFSIZE 4096
#define DATA_SIZE 30000

struct Bracket
{
    ssize_t open;
    ssize_t close;
};

void brainfuck(char *src_code, ssize_t src_code_size, int* data, ssize_t data_size);
struct Bracket* find_code_brackets(char* src_code, ssize_t src_code_size, ssize_t* num_brackets);
ssize_t get_open_bracket(struct Bracket* brackets, ssize_t num_brackets, ssize_t code_ptr);
ssize_t get_close_bracket(struct Bracket* brackets, ssize_t num_brackets, ssize_t code_ptr);

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

void brainfuck(char* src_code, ssize_t src_code_size, int* data, ssize_t data_size)
{
    ssize_t data_ptr = 0, code_ptr = 0, num_brackets = -1;
    struct Bracket* brackets = find_code_brackets(src_code, src_code_size, &num_brackets);
    while (code_ptr != src_code_size)
    {
        switch (src_code[code_ptr])
        {
            case '>':
                data_ptr++;
                if (data_ptr == data_size) {
                    fprintf(stderr, "Data pointer overflow. Fixed data size: %ld\n", data_size);
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
                data[data_ptr]++;
                break;
            case '-':
                data[data_ptr]--;
                break;
            case '.':
                putchar((char) data[data_ptr]);
                break;
            case ',':
                data[data_ptr] = getchar();
                break;
            case '[':
                if (data[data_ptr] == 0) {
                    code_ptr = get_open_bracket(brackets, num_brackets, code_ptr);
                }
                break;
            case ']':
                if (data[data_ptr] != 0) {
                    code_ptr = get_close_bracket(brackets, num_brackets, code_ptr);
                }
                break;
            default:
                fprintf(stderr, "Invalid char: %c\n", src_code[code_ptr]);
                exit(EXIT_FAILURE);
                break;
        }
        code_ptr++;
    }
}

struct Bracket find_bracket_index(char* src_code, ssize_t char_index, ssize_t src_code_size)
{
    ssize_t lvl = 1, index = char_index + 1;
    while (index < src_code_size && lvl != 0)
    {
        if (src_code[index] == '[') {
            lvl++;
        } else if (src_code[index] == ']') {
            lvl--;
        }
        index++;
    }

    if (lvl != 0) {
        fprintf(stderr, "Brackets do not match\n");
        exit(EXIT_FAILURE);
    }

    return (struct Bracket) {
        .open = char_index,
        .close = index - 1,
    };
}

struct Bracket* find_code_brackets(char* src_code, ssize_t src_code_size, ssize_t* num_brackets)
{
    // TODO: do a one-traversal technique : with reallocs & bracket-buffers->less realloc calls, on more brackets at once
    /* we traverse the src_code two times :
     * 1. number of brackets (for allocating memory)
     * 2. for detecting the pairs
     */

    *num_brackets = 0;
    for (char* c = src_code; *c != 0; c++) {
        if (*c == '[') {
            (*num_brackets)++;
        }
    }

    struct Bracket* brackets = malloc(*num_brackets * sizeof(struct Bracket));
    ssize_t bracket_index = 0, char_index = 0;
    while (bracket_index != *num_brackets)
    {
        if (src_code[char_index] == '[') {
            brackets[bracket_index] = find_bracket_index(src_code, char_index, src_code_size);
            bracket_index++;
        }
        char_index++;
    }

    return brackets;
}

ssize_t get_open_bracket(struct Bracket* brackets, ssize_t num_brackets, ssize_t code_ptr)
{
    for (ssize_t i = 0; i < num_brackets; i++) {
        if (brackets[i].open == code_ptr) {
            return brackets[i].close;
        }
    }
    fprintf(stderr, "Unmatched bracket\n");
    exit(EXIT_FAILURE);
    return -1;
}

ssize_t get_close_bracket(struct Bracket* brackets, ssize_t num_brackets, ssize_t code_ptr)
{
    for (ssize_t i = 0; i < num_brackets; i++) {
        if (brackets[i].close == code_ptr) {
            return brackets[i].open;
        }
    }
    fprintf(stderr, "Unmatched bracket\n");
    exit(EXIT_FAILURE);
    return -1;
}

