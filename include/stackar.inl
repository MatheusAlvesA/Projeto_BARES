#include "stackAr.h"

template <typename Object>
void AbsStack<Object>::push (const Object &x)  {
  if(this->tamanho >= this->capacidade) throw std::length_error( "[StackAr->pop()]: Pilha cheia!");;

  this->pilha[this->tamanho] = x;
  this->tamanho++;
}

template <typename Object>
Object AbsStack<Object>::pop () {
  if(this->tamanho <= 0) {throw std::length_error( "[StackAr->pop()]: Pilha vazia!");}

  this->tamanho--; // diminuindo o tamnho da pilha para poder remover o dado
  return this->pilha[this->tamanho];
}

template <typename Object>
Object AbsStack<Object>::top () const{
  if(this->tamanho <= 0) {throw std::length_error( "[StackAr->top()]: Pilha vazia!");};
  return this->pilha[this->tamanho-1]; // não vou diminuir o tamanho da pilha
}

template <typename Object>
void AbsStack<Object>::makeEmpty () {
 this->tamanho = 0; // tamanho agora é zero
}

template <typename Object>
bool AbsStack<Object>::isEmpty () const{
 return this->tamanho == 0; // retorna se está vazio
}
