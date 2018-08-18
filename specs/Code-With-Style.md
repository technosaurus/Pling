# Coding With Style

## Code Formatting

  * Tabs are for tabulating, spaces are for spacing
    - Each level of indentation is one tab.
    - To align text, tab to previous line and add space from there.
```c
int example(int x,int y){
	if (!x) return bary(y);
	else if (!y) return barx(x);
	else return bar( x /*comment that takes up the line*/,
	                 y /*some comment*/ );
}
```

  * Use formatting that takes condenses vertical space
    - Put opening braces on the same line.
    - "Hug" your `else`s and `else if`s.
    - Write short `if` and else statements on a single line.
    - Use 1 blank line before function implementations, but none within.
    - Only wrap to 80 columns to make code more readable (i.e. don't wrap tables)
    - Don't repeat yourself.
      1. Factor out common code into functions.
      2. Use XMacros to generate similar, repeated data.
      3. If a function exceeds ~25 lines, try replacing a block with an inline function.

  * Name stuff
    - Use a (namespacing) prefix for externally visible objects (for library code).
    - Start with a verb and make it succint verb<<_adjective>_noun> `init, setup_table, get_max_value`
    - Group asterisks with the variable name except in function prototypes.
      1. `char* src, dest;` is easily misread as being equivalent to `char *src, *dest;`
      2. Since this is not allowed in prototypes, it can simplify grepping operations.
