#include "stackint.h"

void StackINT::push (int x ) {
  if(tamanho >= capacidade) realocar();

  pilha[tamanho] = x;
  tamanho++;
}

int StackINT::pop () {
  if(tamanho <= 0) {throw std::length_error( "[StackINT->pop()]: Pilha vazia!");}

  tamanho--; // diminuindo o tamnho da pilha para poder remover o dado
  return pilha[tamanho];
}

int StackINT::top () const{
  if(tamanho <= 0) {throw std::length_error( "[StackINT->top()]: Pilha vazia!");};
  return pilha[tamanho-1]; // não vou diminuir o tamanho da pilha
}

void StackINT::realocar () {
  capacidade = capacidade*2; //Dobrando a capacidade
  
  int *temp = new int[capacidade]; // alocando nova memoria
  for(int x = 0; x < tamanho; x++) // copiando dados 
   temp[x] = pilha[x];
   
  delete [] pilha; // deletando anterior
  pilha = temp;   // resetando o ponteiro
}

void StackINT::makeEmpty () {
 tamanho = 0; // tamanho agora é zero
 capacidade = MAX; // capacidade volta a ser MAX
 delete [] pilha; // deleta a memoria
 pilha = new int [MAX]; // recia a memora do zero
}
