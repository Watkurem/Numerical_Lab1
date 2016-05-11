Numerical_Lab1
======

First lab assignment for Numerical Methods in university. You probably have zero interest in this. C.

The task was to write a program that would compute a specific function via power series. My assignment was the cosine function on [-31.3; -4.9].

Assignment also involved outputting results in a table. For that I had written several functions that would print nicely formatted table into stdout and put them into separate file table.c, so that it would be possible to write another output implementation should the need arise (e.g. for Windows terminal or simply one that does not support unicode).

Provided implementation of table output will definitely not work on Windows machine and may not work on other machines also, but is guaranteed to work on mine. It will probably work with POSIX compliant OS and a terminal that supports unicode.

This table output implementation proved very useful with other labs where I simply reused it.

Assingment *also* involved writing abundant documentation comments for pretty much everything following specific style. It was not required for other labs, so you will not see these comments there.