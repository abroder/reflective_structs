A collection of mildly interesting things I've learned while working on this project.

- Internal identifiers have a limit of 63 characters, and external identifiers have a limit of 31 characters - longer identifiers with shared prefixes might be treated as the same by conformant compilers ([N1570](http://www.open-std.org/jtc1/sc22/wg14/www/docs/n1570.pdf), §5.2.4.1)
- Structs and unions have a limit of 1023 members ([N1570](http://www.open-std.org/jtc1/sc22/wg14/www/docs/n1570.pdf), §5.2.4.1).