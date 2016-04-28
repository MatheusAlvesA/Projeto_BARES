#ifndef _STACK_INT_
#define _STACK_INT_

#include <stdexcept>
const int MAXS = 10000;

class StackINT {

public:

StackINT () {pilha = new int[MAXS];} // começa com MAX porem cresce dinamicamente
// Default constructor
~ StackINT () {delete [] pilha;} // Default destructor
// Basic members
void push (int);
int pop ();
int top ( ) const;
bool isEmpty ( ) const {return tamanho == 0;};
void makeEmpty ( );

private:

int tamanho = 0;
int capacidade = MAX;
int *pilha;
void realocar();
};

#include "stackint.cpp"
#endif
