use std::env;
use std::fs::File;
use std::io::{BufReader, Read};
use std::convert::TryInto;

const DATA_SIZE: usize = 30_000;

struct Bracket
{
    open: u64,
    close: u64,
}

fn find_bracket_index(src_code: &String, char_index: u64) -> Bracket {
    let mut lvl: u64 = 1;
    let mut index: u64 = char_index + 1;
    let src_code_size: usize = src_code.chars().count();
    while index < src_code_size as u64 && lvl != 0 {
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
    for char_index in 0..src_code.chars().count() {
        let bracket: Bracket = find_bracket_index(src_code, char_index as u64);
        brackets.push(bracket);
    }
    brackets
}

fn get_open_bracket(brackets: Vec<Bracket>, code_ptr: u64) -> u64 {
    for bracket in brackets {
        if bracket.open == code_ptr {
            return bracket.close;
        }
    }
    panic!("Unmatched bracket");
}

fn get_close_bracket(brackets: Vec<Bracket>, code_ptr: u64) -> u64 {
    for bracket in brackets {
        if bracket.close == code_ptr {
            return bracket.open;
        }
    }
    panic!("Unmatched bracket");
}

fn brainfuck(src_code: &String) {
    // prepare variables
    let brackets: Vec<Bracket> = find_code_brackets(src_code);
    let mut data: Vec<i32> = vec![0; DATA_SIZE];
    let mut data_ptr: usize = 0;
    let mut code_ptr: usize = 0;
    let src_code_size: usize = src_code.chars().count();
    let num_brackets: usize = brackets.len();
    println!("Brackets parsed");
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
            _ => {
                panic!("Invalid char: {}", c);
            }
        }
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
    // println!("{}", data);
    brainfuck(&src_code);
}
