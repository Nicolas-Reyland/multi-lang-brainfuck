use std::env;
use std::fs::File;
use std::io::{BufReader, Read};
use std::convert::TryInto;

const DATA_SIZE: usize = 30_000;

struct Bracket
{
    open: u32,
    close: u32,
}

fn find_bracket_index(src_code: &String, char_index: u32) -> Bracket {
    let mut lvl: u32 = 1;
    let mut index: u32 = char_index + 1;
    let src_code_size: usize = src_code.chars().count();
    while index < src_code_size as u32 && lvl != 0 {
        if src_code.chars().nth(index.try_into().unwrap()).unwrap() ==  '[' {
            lvl += 1;
        } else if src_code.chars().nth(index.try_into().unwrap()).unwrap() == ']' {
            lvl -= 1;
        }
        index += 1;
    }
    if lvl != 0 {
        panic!("Brackets do not match");
    }
    Bracket {
        open: char_index,
        close: index - 1,
    }
}

fn find_code_brackets(src_code: &String) -> Vec<Bracket> {
    let mut brackets: Vec<Bracket> = Vec::new();
    for (char_index, c) in src_code.chars().enumerate() {
        if c == '[' {
            let bracket: Bracket = find_bracket_index(src_code, char_index as u32);
            brackets.push(bracket);
        }
    }
    brackets
}

fn get_open_bracket(brackets: &Vec<Bracket>, code_ptr: u32) -> u32 {
    for bracket in brackets {
        if bracket.open == code_ptr {
            return bracket.close;
        }
    }
    panic!("Unmatched bracket");
}

fn get_close_bracket(brackets: &Vec<Bracket>, code_ptr: u32) -> u32 {
    for bracket in brackets {
        if bracket.close == code_ptr {
            return bracket.open;
        }
    }
    panic!("Unmatched bracket");
}

fn brainfuck(src_code: String) {
    // prepare variables
    let brackets: Vec<Bracket> = find_code_brackets(&src_code);
    let mut data: Vec<i32> = vec![0; DATA_SIZE];
    let mut data_ptr: usize = 0;
    let mut code_ptr: usize = 0;
    let src_code_size: usize = src_code.chars().count();
    // main code interpretation loop
    while code_ptr as usize != src_code_size {
        let c: char = src_code.chars().nth(code_ptr).unwrap();
        match c {
            '>' => {
                data_ptr += 1;
                if data_ptr == DATA_SIZE {
                    panic!("Data pointer overflow. Fixed data size: {}", DATA_SIZE);
                }
            }
            '<' => {
                if data_ptr == 0 {
                    panic!("Negative data pointer (-1).");
                }
                data_ptr -= 1;
            }
            '+' => {
                data[data_ptr] += 1;
            }
            '-' => {
                data[data_ptr] -= 1;
            }
            '.' => {
                print!("{}", std::char::from_u32(data[data_ptr] as u32).unwrap());
            }
            ',' => {
                let input: Option<i32> = std::io::stdin()
                    .bytes()
                    .next()
                    .and_then(|result| result.ok())
                    .map(|byte| byte as i32);
                data[data_ptr] = input.unwrap();
            }
            '[' => {
                if data[data_ptr] == 0 {
                    code_ptr = get_open_bracket(&brackets, code_ptr.try_into().unwrap()) as usize;
                }
            }
            ']' => {
                if data[data_ptr] != 0 {
                    code_ptr = get_close_bracket(&brackets, code_ptr.try_into().unwrap()) as usize;
                }
            }
            _ => {
                panic!("Invalid char: {}", c);
            }
        }
        code_ptr += 1;
    }
}

fn main() {
    let args: Vec<String> = env::args().collect();
    if args.len() != 2 {
        panic!("Usage: ./interpret input-file");
    }
    let file_path = &args[1];

    let mut src_code = String::new();
    let f = File::open(file_path).expect("Unable to open file");
    let mut br = BufReader::new(f);
    br.read_to_string(&mut src_code).expect("Unable to read string");
    src_code.retain(|src_code| "><+-[].,".contains(src_code));
    brainfuck(src_code);
}
