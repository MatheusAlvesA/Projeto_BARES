#ifndef QUEUEAR_H
#define QUEUEAR_H

#include <iostream>
#include <stdexcept>
#include "AbsQueue.h"

const int MAX = 50;
template <typename Object>
class QueueAr : AbsQueue<Object>{
    private:
        Object *fila;
        int fim;
        int capacidade;
        int inicio;

      void realocar() {
          capacidade *= 2;
           Object *nova = new Object[capacidade];
           if(fim >= inicio) {
              for(int x = inicio; x  <= fim; x++)
                nova[x] = fila[x];
           }
           else {
            int y = 0;
              for(int x = inicio; x  <= (capacidade/2)-1; x++) {// capacidade sebre 2 é o final do antigo array
                nova[y] = fila[x];
                y++;
           }
            for(int x = 0; x  <= fim; x++) {// capacidade sebre 2 é o final do antigo array
                nova[y] = fila[x];
                y++;
           }
           inicio = 0;
           fim = y-1;
        }
        delete [] fila;
        fila = nova;
        // Perceba que se antes a fila estiver deslocada ao final dessa função a fila estará alinhada corretamente
      }
    public:
        QueueAr (int Tamanho = MAX) {
            fila = new Object[Tamanho];
            inicio = -1;
            fim = -1;
            capacidade = Tamanho;
        }
         ~QueueAr() {delete [] fila;}

       void enqueue(const Object &x) {
           if((fim < inicio && fim+1 == inicio) || (fim+1 == capacidade && inicio == 0)) {realocar();}

           if(fim == -1 && inicio == -1) {
               inicio = 0;
           }
           if(fim+1 == capacidade) fim = -1;

               fila[fim+1] = x;
               fim++;
       }

       Object dequeue() {
           if(fim == -1 && inicio == -1) {throw std::length_error( "[QueueAr->dequeue()()]: tentativa de remover da fila vazia!");}

           if(fim == inicio) { // nesse caso só tem 1 elemento
               int temp = inicio;
               inicio = fim = -1;
               return fila[temp];
           }
              if(inicio == capacidade-1) inicio = 0;
              else inicio++;

               return fila[inicio-1]; // ATENÇÃO retornando o primeiro cara da fila e em seguida decrementando onde fica o inicio da fila
       }
       Object getFront () const {
          if(fim == -1 && inicio == -1) {throw std::length_error( "[QueueAr->dequeue()()]: tentativa de ler da fila vazia!");}

               return fila[inicio];
       };
       Object getLast () const {
          if(fim == -1 && inicio == -1) {throw std::length_error( "[QueueAr->getLast()]: tentativa de ler da fila vazia!");}

               return fila[fim];
       };
       bool isEmpty () const {
          if(fim == -1 && inicio == -1) return true;
          else return false;
       };
       void makeEmpty() {
          fim = inicio = -1;
           delete [] fila;
           fila = new Object[10];
           capacidade = 10;
           
       }
};

#endif
