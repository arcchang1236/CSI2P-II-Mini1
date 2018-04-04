#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

/*
Something like Python
>> y = 2
>> z = 2
>> x = 3*y + 4/(2*z)
*/

/*
the only type: integer
everything is an expression
  statement   := END | expr END
  expr        := term expr_tail
  expr_tail   := ADDSUB term expr_tail | NIL
  term        := factor term_tail
  term_tail := MULDIV factor term_tail | NIL
  factor      := INT | ADDSUB INT | ADDSUB ID | ID ASSIGN expr | ID | LPAREN expr RPAREN
*/

#define MAXLEN 256

#define TBLSIZE 65535

typedef enum {UNKNOWN, END, INT, ID, ADDSUB, MULDIV, ASSIGN, LPAREN, RPAREN} TokenSet;

typedef struct {
	char name[MAXLEN];
	int val;
} Symbol;
Symbol table[TBLSIZE];
int sbcount = 0;

typedef struct _Node {
	char lexeme[MAXLEN];
	TokenSet token;
	int val;
	struct _Node *left, *right;
} BTNode;

void statement(void);
BTNode* expr(void);
BTNode* term(void);
BTNode* factor(void);
int getval(void);
int setval(char*, int);
void error(void);

/* Lexical-related function */
int match (TokenSet token);
void advance(void);
char* getLexeme(void);
static TokenSet getToken(void);
static TokenSet lookahead = UNKNOWN;
static char lexeme[MAXLEN];

/* create a node without any child */
BTNode* makeNode(TokenSet tok, const char *lexe)
{
	BTNode *node = (BTNode*) malloc(sizeof(BTNode));
	strcpy(node->lexeme, lexe);
	node->token= tok;
	node->val = 0;
	node->left = NULL;
	node->right = NULL;
	return node;
}

/* clean a tree */
void freeTree(BTNode *root)
{
	if (root!=NULL) {
		freeTree(root->left);
		freeTree(root->right);
		free(root);
	}
}

/* print a tree by pre-order */
void printPrefix(BTNode *root)
{
	if (root != NULL) {
		printf("%s ", root->lexeme);
		printPrefix(root->left);
		printPrefix(root->right);
	}
}

/* traverse the syntax tree by pre-order
   and evaluate the underlying expression */
int evaluateTree(BTNode *root)
{
	int retval = 0, lv, rv;
	if (root != NULL) {
		switch (root->token) {
		case ID:
		case INT:
			retval = root->val;
                /* TODO */
			break;
		case ASSIGN:
		case ADDSUB:
		case MULDIV:
                /* TODO */
                lv = evaluateTree(root->left);
                rv = evaluateTree(root->right);
                if (strcmp(root->lexeme, "+") == 0)
                    retval = lv + rv;
                else if (strcmp(root->lexeme, "-") == 0)
                    retval = lv - rv;
                else if (strcmp(root->lexeme, "*") == 0)
                    retval = lv * rv;
                else if (strcmp(root->lexeme, "/") == 0)
                    retval = lv / rv;
                 else if (strcmp(root->lexeme, "=") == 0)
                    retval = setval(root->left->lexeme, rv);
                break;
		default:
			retval = 0;
		}
	}
	return retval;
}

int getval(void)
{
	int i, found, retval = 0;

	if (match(INT)) {
		retval = atoi(getLexeme());
	} else if (match(ID)) {
		i = 0;
		found = 0;
		retval = 0;
		while (i<sbcount && !found) {
			if (strcmp(getLexeme(), table[i].name)==0) {
				retval = table[i].val;
				found = 1;
				break;
			} else {
				i++;
			}
		}
		if (!found) {
			if (sbcount < TBLSIZE) {
				strcpy(table[sbcount].name, getLexeme());
				table[sbcount].val = 0;
				sbcount++;
			} else {
				error();
			}
		}
	}
	return retval;
}

int setval(char *str, int val)
{
	int i, retval = 0;
	i = 0;
	while (i<sbcount) {
		if (strcmp(str, table[i].name)==0) {
			table[i].val = val;
			retval = val;
			break;
		} else {
			i++;
		}
	}
	return retval;
}

//  expr        := term expr_tail
//  expr_tail   := ADDSUB term expr_tail | NIL
BTNode* expr(void)
{
	BTNode *retp, *left;
	retp = left = term();
	while (match(ADDSUB)) { // tail recursion => while
		retp = makeNode(ADDSUB, getLexeme());
		advance();
		retp->right = term();
		retp->left = left;
		left = retp;
	}
	return retp;
}

//  term        := factor term_tail
//  term_tail := MULDIV factor term_tail | NIL
BTNode* term(void)
{
	BTNode *retp, *left;
	retp = left = factor();
	while (match(MULDIV)) { // tail recursion => while
		retp = makeNode(MULDIV, getLexeme());
		advance();
		retp->right = factor();
		retp->left = left;
		left = retp;
	}
	return retp;
}

BTNode* factor(void)
{
	BTNode* retp = NULL;
	char tmpstr[MAXLEN];

	if (match(INT)) {
		retp =  makeNode(INT, getLexeme());
		retp->val = getval();
		advance();
	} else if (match(ID)) {
		BTNode* left = makeNode(ID, getLexeme());
		left->val = getval();
		strcpy(tmpstr, getLexeme());
		advance();
		if (match(ASSIGN)) {
			retp = makeNode(ASSIGN, getLexeme());
			advance();
			retp->right = expr();
			retp->left = left;
		} else {
			retp = left;
		}
	} else if (match(ADDSUB)) {
		strcpy(tmpstr, getLexeme());
		advance();
		if (match(ID) || match(INT)) {
			retp = makeNode(ADDSUB, tmpstr);
			if (match(ID))
				retp->right = makeNode(ID, getLexeme());
			else
				retp->right = makeNode(INT, getLexeme());
			retp->right->val = getval();
			retp->left = makeNode(INT, "0");
			retp->left->val = 0;
			advance();
		}
	} else if (match(LPAREN)) {
		advance();
		retp = expr();
		if (match(RPAREN)) {
			advance();
		}
	}
	return retp;
}

void error(void)
{
    /* TODO:
     *
     * Error-Handler,
     * You should deal with the error that happened in calculator
     * An example is x = 5 / 0, which is divide zero error.
     * You should call error() when any error occurs
     *
     */
    
    printf("EXIT 1\n");
    exit(0);
}

void statement(void)
{
	BTNode* retp;

	if (match(END)) {
		advance();
	} else {
		retp = expr();
		if (match(END)) {
            evaluateTree(retp);
			//printPrefix(retp);
			freeTree(retp);
			advance();
		}
	}
}

TokenSet getToken(void)
{
    int i;
    char c;
    
    while ( (c = fgetc(stdin)) == ' ' || c== '\t' );
    
    if (isdigit(c)) {
        lexeme[0] = c;
        c = fgetc(stdin);
        i = 1;
        while (isdigit(c) && i<MAXLEN) {
            lexeme[i] = c;
            ++i;
            c = fgetc(stdin);
        }
        ungetc(c, stdin);
        lexeme[i] = '\0';
        return INT;
    } else if (c == '+' || c == '-') {
        lexeme[0] = c;
        lexeme[1] = '\0';
        return ADDSUB;
    } else if (c == '*' || c == '/') {
        lexeme[0] = c;
        lexeme[1] = '\0';
        return MULDIV;
    } else if (c == '\n') {
        lexeme[0] = '\0';
        return END;
    } else if (c == '=') {
        strcpy(lexeme, "=");
        return ASSIGN;
    } else if (c == '(') {
        strcpy(lexeme, "(");
        return LPAREN;
    } else if (c == ')') {
        strcpy(lexeme, ")");
        return RPAREN;
    } else if (isalpha(c) || c == '_') {
        lexeme[0] = c;
        c = fgetc(stdin);
        i = 1;
        while (isalpha(c) || isdigit(c) || c == '_') {
            lexeme[i] = c;
            ++i;
            c = fgetc(stdin);
        }
        ungetc(c, stdin);
        lexeme[i] = '\0';
        return ID;
    } else {
        return UNKNOWN;
    }
}

void advance(void)
{
    lookahead = getToken();
}

int match(TokenSet token)
{
    if (lookahead == UNKNOWN) advance();
    return token == lookahead;
}

char* getLexeme(void)
{
    return lexeme;
}

int main()
{
    char c;
	
	while (1) {
		statement();
	}
	return 0;
}
