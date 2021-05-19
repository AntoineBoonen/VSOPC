# VSOPCompiler

## Description

**vsopc** is compiler crafted to translated [VSOP](vsop_manual.pdf) code into [LLVM](https://llvm.org) and/or make an executable. Its implementation is part of the course : [INFO0085 - Compilers](https://www.programmes.uliege.be/cocoon/20202021/cours/INFO0085-1.html)
of the University of Liège (Belgium).

### VSOP Language
The language is clearly specified in this [manual](vsop_manual.pdf).

## File organisation

- All declaration and implementation files can be found in the [lib](lib/) directory.
- Some VSOP test files are present in the [test](test/) directory.
- The [runtime](runtime/) directory contains the external code required by the compiler.

## Requirements

We will need to have [clang](https://clang.llvm.org) compiler installed and [llvm](https://llvm.org) and [llvm-c](https://llvm.org) librairies installed on your machine before attempting anything with the compiler.

## Installation and Usage

1. Run the command:

```bash
make install-tools
```
This will pre-compile the external code required by the compiler and store the object files into `/tmp/vsopc`.

 
2. Run the following to get the `vsopc` executable of the compiler. 

```bash
make vsopc
```

3. Use this to get a detailed inteface on how to use the executable. 
```bash
./vsopc -h
```

## Important to know

If you want to rebuild the `Parser.cpp`, you will need to have [bison](https://www.gnu.org/software/bison/manual/bison.html#Bison-Declarations) installed. Then use either of the followings:
```bash
make lib/parser.cpp
```
```bash
bison -v -d --output=lib/parser.cpp lib/parser.yy
```

Afterwards, you will need to add the following lines in `Parser.hpp` public method interface.
```c++
    std::vector<Error> get_errors();
    Node get_ast();
```

Finally, build the `vsopc` executable.
```bash
make vsopc
```
## Contributor(s)

It has been implemented by **Antoine Boonen** (antoine.boonen@gmail.com).
