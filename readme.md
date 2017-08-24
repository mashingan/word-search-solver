Word Solver
====

This repository is created as backup for attempting to solve the Word Solver problem within array of array of characters.  

Word Solver problem is a kind of problem to count how many words available within a map of characters when looked in horizontal, forward and backward read, also vertically and diagonally. Look for [Word Search](https://en.wikipedia.org/wiki/Word_search) entry in Wikipedia.

For example, ``cat`` is the word we want to search while below is the map

```
catt
aata
tatc
```

when we calculate it, the total available ``cat`` words are 4 which:

* the 1<sup>st</sup> line forward horizontal
* the 1<sup>st</sup> column forward vertical
* the 1<sup>st</sup> line and column forward diagonal
* the last column backward vertical

Result file is ``wordSolver.c``. File ``worldSolver.c`` served as a development file before the _(almost)_ same solution is written to file ``wordSolver.c``.  

The built executable optionally is able to accept one argument for input file. The default input file is ``input.in.example`` which in that file given several maps of words ready to be solved. Other files which are prefixed with ``input`` are kind of input too.

The input file consist of several lines which have their own functions:

1. The first line is the total of tests going to be done (in other words, it's the maps of words available)
2. The 2nd and 3rd line, those serve as row and column of map respectively.
3. The several lines after that, each of them represents row of map with width of column.  

For next subsequent lines, it follows the 2nd and 3rd direction above parsing method.  
The output is directly written to ``stdout``, so pipe it to file to save it if necessary.
