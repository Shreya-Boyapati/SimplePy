SimplePy
======

SimplePy is a Python to C++ interpreter that runs Python code in a C++ compiler. Python code is read in using filestream and is tokenized. The tokens are then processed and mapped to their cpp equivalents through various execute statements and support functions.

Supported Actions
---
- Print with python formatting
- Read keyboard input
- Index and return variables
- Return type of variable
- Convert variable from int to string
- Convert variable from string to int
- Arithmetic operations (+, -, *, **, /, %)
- Numerical comparisons (<, <=, >, >=, ==, !=)
- String comparisons (<, <=, >, >=, ==, !=)
- String concatenation

Running compiler
---
- Set agrv[1] as name of python file to be converted and argv[2] as file size
- Run main.cpp with defined arguments
- Output of python code will be displayed in terminal like so
  
```
**starting**

x = 1
y = 99
z cubed is 1000.
st = csiscool!
condition c1 is 1, c2 is 0
```
