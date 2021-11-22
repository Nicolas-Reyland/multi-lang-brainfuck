#include <iostream>
#include <fstream>
#include <vector>

/* Possible upgrades :
 * - better bracket-couple allocation
 * - other things
 */

#define BUFSIZE 4096
#define DATA_SIZE 30000

struct Bracket
{
    ssize_t open;
    ssize_t close;
};

void brainfuck(std::vector<char> src_code, std::vector<int> data);
std::vector<struct Bracket> find_code_brackets(std::vector<char> src_code);
ssize_t get_open_bracket(std::vector<struct Bracket> brackets, ssize_t code_ptr);
ssize_t get_close_bracket(std::vector<struct Bracket> brackets, ssize_t code_ptr);

int main(int argc, char** argv)
{
    // Verify usage of script
    if (argc != 2) {
        fprintf(stderr, "Usage: ./interpret input-file\n");
        exit(EXIT_FAILURE);
    }

    // Read input file content
    std::string filename(argv[1]);
    std::vector<char> src_code;
    char byte = 0;

    std::ifstream input_file(filename);
    if (!input_file.is_open()) {
        std::cerr << "Could not open the file - '"
             << filename << "'" << std::endl;
        return EXIT_FAILURE;
    }

    while (input_file.get(byte)) {
        if (byte != ' ' && byte != '\n')
            src_code.push_back(byte);
    }
    input_file.close();

    // Allocate data
    std::vector<int> data(DATA_SIZE);

    // interpret
    brainfuck(src_code, data);

    return EXIT_SUCCESS;
}


void brainfuck(std::vector<char> src_code, std::vector<int> data)
// void brainfuck(char* src_code, ssize_t src_code_size, int* data, ssize_t data_size)
{
    std::vector<struct Bracket> brackets = find_code_brackets(src_code);
    ssize_t data_ptr = 0,
            code_ptr = 0,
            src_code_size = src_code.size(),
            data_size = data.size(),
            num_brackets = brackets.size();
    while (code_ptr != src_code_size)
    {
        switch (src_code[code_ptr])
        {
            case '>':
                data_ptr++;
                if (data_ptr == data_size) {
                    std::cerr << "Data pointer overflow. Fixed data size: " <<  data_size << std::endl;
                    exit(EXIT_FAILURE);
                }
                break;
            case '<':
                data_ptr--;
                if (data_ptr == -1) {
                    std::cerr << "Negative data pointer (-1)." << std::endl;
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
                getchar(); // catch trailing '\n'
                break;
            case '[':
                if (data[data_ptr] == 0) {
                    code_ptr = get_open_bracket(brackets, code_ptr);
                }
                break;
            case ']':
                if (data[data_ptr] != 0) {
                    code_ptr = get_close_bracket(brackets, code_ptr);
                }
                break;
            case ' ': case '\n':
                break;
            default:
                fprintf(stderr, "Invalid char: %c\n", src_code[code_ptr]);
                exit(EXIT_FAILURE);
                break;
        }
        code_ptr++;
    }
}

/*std::vector<struct Bracket> find_code_brackets(std::vector<char> src_code);
ssize_t get_open_bracket(std::vector<struct Bracket> brackets, ssize_t code_ptr);
ssize_t get_close_bracket(std::vector<struct Bracket> brackets, ssize_t code_ptr);*/


struct Bracket find_bracket_index(std::vector<char> src_code, ssize_t char_index)
{
    ssize_t lvl = 1,
            index = char_index + 1,
            src_code_size = src_code.size();
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

std::vector<struct Bracket> find_code_brackets(std::vector<char> src_code)
{
    // TODO: do a one-traversal technique : with reallocs & bracket-buffers->less realloc calls, on more brackets at once
    // we traverse the src_code two times :
    // 1. number of brackets (for allocating memory)
    // 2. for detecting the pairs

    ssize_t num_brackets = 0;
    for (const char c: src_code) {
        if (c == '[') {
            (num_brackets)++;
        }
    }

    std::vector<struct Bracket> brackets(num_brackets);
    ssize_t bracket_index = 0, char_index = 0;
    while (bracket_index != num_brackets)
    {
        if (src_code[char_index] == '[') {
            brackets[bracket_index] = find_bracket_index(src_code, char_index);
            bracket_index++;
        }
        char_index++;
    }

    return brackets;
}

ssize_t get_open_bracket(std::vector<struct Bracket> brackets, ssize_t code_ptr)
{
    ssize_t num_brackets = brackets.size();
    for (ssize_t i = 0; i < num_brackets; i++) {
        if (brackets[i].open == code_ptr) {
            return brackets[i].close;
        }
    }
    fprintf(stderr, "Unmatched bracket\n");
    exit(EXIT_FAILURE);
    return -1;
}

ssize_t get_close_bracket(std::vector<struct Bracket> brackets, ssize_t code_ptr)
{
    ssize_t num_brackets = brackets.size();
    for (ssize_t i = 0; i < num_brackets; i++) {
        if (brackets[i].close == code_ptr) {
            return brackets[i].open;
        }
    }
    fprintf(stderr, "Unmatched bracket\n");
    exit(EXIT_FAILURE);
    return -1;
}

