# multi-lang-brainfuck
Interpreters for the brainfuck language in multiple programming languages

Do not hesitate to add your own implementations through pull requests !

## Usages

### Example Usage
If the interpreter is a compiled binary file, it should be used like this :
```
./interpreter ../examples/helloworld.b
./interpreter .../examples/fibonacci.b
./interpreter ../examples/life.b
```

### Python
```
python interpret.py brainfuck-source-file
```

### Haskell
```
ghc interpret.hs
./interpret brainfuck-source-file
```

### C
```
gcc interpreter.c -o interpreter
./interpret brainfuck-source-file
```

### C++
```
g++ interpreter.cpp -o interpreter
./interpret brainfuck-source-file
```

### 

### Haskell
```
rustc interpret.hs
./interpret brainfuck-source-file
```


## TODO

 - [x] Python
 - [x] C
 - [x] C++
 - [x] Haskell
 - [ ] JavaScript
 - [x] Rust
 - [ ] OCaml
 - [ ] Ruby
 - [ ] C#

