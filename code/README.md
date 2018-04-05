# Tutorial

## What you need to do in this mini-project?
1. Trace all of the code if you like to
* understand each detail
* review what you have learn during the courses.

2. Replace the current **evaluateTree()** to generate assembly code during the pre-order tree traversal.

3. If you encounter any violation of arithmetic rules, you should call **error()** to terminate the program. **You should consider all kinds of possibilities.** The following is some examples:

    * x = 5 +
    * y = 7 / 0
    * z = --2
    * ... and so on

4. Optimize your assembly code to get extra credits!

## A Simple Calculator - Interpreter Version

![Imgur](https://i.imgur.com/PgelT14.png)

## The Compilation Process

![](https://i.imgur.com/igaMjAg.png)

What is **Parser** ? You may refer to [11856](http://140.114.86.238/problem/11856/), [10961](http://140.114.86.238/problem/10961/) for similar concept.
What is **Code Generator** ? You may refer to [10950](http://140.114.86.238/problem/10950/) for similar concept.

## The Lexical Analyzer
The lexical analysis is the process of recognizing **which string of symbols** from the source program represent **a single entity called token** and **identifying types of them** such as *numeric values, words, arithmetic operators, and so on*.

``` cpp
static TokenSet getToken(void);
```

This function does three things:
* Extracts the *next token* from the input string
* Stores the token in *char lexeme[MAXLEN]*
* Identifies the token's type

``` cpp
typedef enum {UNKNOWN, END, INT, ID, ADDSUB, MULDIV, ASSIGN, LPAREN, RPAREN} TokenSet;
```

## The Parser
Group tokens into statements based on a set of rules, collectively called a grammar.

>> statement&nbsp;&nbsp;&nbsp;:= ENDFILE | END | expr END

>> expr&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;:= term expr_tail

>> expr_tail&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;:= ADDSUB term expr_tail | NiL

>> term&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;:= factor term_tail

>> term_tail&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;:= MULDIV factor term_tail | NiL

>> factor&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;:= INT | ADDSUB INT

>> &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;| ID | ADDSUB ID

>>  &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;| LPAREN expr RPAREN

>> &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;| ID ASSIGN expr


### Tail Recursion -> Loop
Recursion Version:
``` cpp
BTNode* expr(void)
{
    BTNode *node;
    node = term();
    return expr_tail(node);
}
BTNode* expr_tail(BTNode *left)
{
    BTNode *node;
    if (match(ADDSUB)) {
        node = makeNode(ADDSUB, getLexeme());
        advance();
        node->left = left;
        node->right = term();
        return expr_tail(node);
    }
    else
        return left;
}
```

Loop Version:
``` cpp
BTNode* expr(void)
{
    BTNode *retp, *left;
    //int retval;
    retp = left = term();
    while (match(ADDSUB)) {
        retp = makeNode(ADDSUB, getLexeme());
        advance();
        retp->right = term();
        retp->left = left;
        left = retp;
    }
    return retp;
}
```
Recursion Version:
``` cpp
BTNode* term(void)
{
    BTNode *node;
    node = factor();
    return term_tail(node);
}
BTNode* term_tail(BTNode *left)
{
    BTNode *node;
    if (match(MULDIV)) {
        node = makeNode(MULDIV, getLexeme());
        advance();
        node->left = left;
        node->right = factor();
        return term_tail(node);
    }
    else
        return left;
}
```

Loop Version:
``` cpp
BTNode* term(void)
{
    BTNode *retp, *left;
    retp = left = factor();

    while (match(MULDIV)) {
        retp = makeNode(MULDIV, getLexeme());
        advance();
        retp->right = factor();
        retp->left = left;
        left = retp;
    }
    return retp;
}
```
![](https://i.imgur.com/JsMO97b.png)
![](https://i.imgur.com/Ke8B5aD.png)
![](https://i.imgur.com/zAK6F9I.png)

## The Code Generator

Construct the machine-language instructions to implement the statements recognized by the parser and represented as syntax trees.

We provide a function:
``` cpp
int evaluateTree(BTNode *root)
```
which calculates the answer by pre-order traversal of the syntax tree (refer to HWs 11359, 11360 and 11362).

Note that, to deal with expressions including variables such as:
>> x=3

>> 4*x+(y=((10-2)/4))

, we use a symbol table to record variables' current values.
We use **getval()** and **setval()** to manipulate the symbol table.
The symbol tables are used both in the parser and the code generator.

``` cpp
#define TBLSIZE 65535
typedef struct {
    char name[MAXLEN];
    int val;
} Symbol;
Symbol table[TBLSIZE];
```


