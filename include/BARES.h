#ifndef _BARES_H_
#define _BARES_H_

#include <string>
#include <sstream>
#include <stdexcept>
#include "queuear.h"
#include "stackint.h"

class BARES {
public:
    BARES(){this->simbolos = new QueueAr<int>;}
    ~BARES(){delete this->simbolos;}
    enum simbolo{
        ABRIR = '(',
        FECHAR = ')',
        NEGATIVO = '-',
        POTENCIA = '^',
        MULTIPLICAR = '*',
        RESTO = '%',
        DIVIDIR = '/',
        SOMAR = '+',
        SUBTRAIR = '-'      
    };
    enum simbolo_token{
// note que esses valores estão fora da faiza pemitida para um numero, sendo assim não seram confundidos com numeros quando retirados da fila
        _ABRIR = 40000,
        _FECHAR = 40001,
        _NEGATIVO = 40002,
        _POTENCIA = 40003,
        _MULTIPLICAR = 40004,
        _RESTO = 40005,
        _DIVIDIR = 40006,
        _SOMAR = 40007,
        _SUBTRAIR = 40008
    };
    std::string processar(std::string nome);

private:
    int size;
    QueueAr<int> *simbolos; // fila que armazena os valores
    
    // converte uma linha de string pura em uma fila de simbolos incluindo os operando e operadores na ordem que estavam na string original
    void converter_bruto_fila(std::string bruto);
    // converte a fila com os operando e operadores em duas pilhas separadas
    QueueAr<int> *Infx2Posfx(QueueAr<int> *fila);
    // executa as operações aritiméticas e retorna o resultado
    int AvalPosfixa(QueueAr<int> *fila);
};

class ERRO {
public:
    ERRO(int cod, int col = 0) {codigo = cod; coluna = col;}
    enum TYPE{
        CONSTANT_OUT = 1, //Um dos operandos da expressão está fora da faixa permitida.
        ILL_FORMED = 2, // Em alguma parte da expressão está faltando um operando ou existe algum operando em formato errado.
        INVALID_OPERAND = 3, // Existe um sımbolo correspondente a um operador que não está na lista de operadores válidos.
        EXTRANEOUS = 4, // Aparentemente o programa encontrou um sımbolo extra “perdido” na expressão.
        MISSMATCH_O = 5, // Está faltando um parentese de fechamento ’)’ para um parˆentese de abertura ‘(’ correspondente.
        MISSMATCH_C = 6, // Existem um parentese fechando sem ter um parentese abrindo correspondente.
        LOST_OPERATOR = 7, // Apareceu um operador sem seus operandos.
        DIVISION_0 = 8, // Houve divisão cujo quociente é zero.
        OVERFLOW = 9    // Acontece quando uma operação dentro da expressão ou a expressao inteira estoura o limite das constantes numéricas
    };
    std::string msg() {
        std::string mensagem;
        std::ostringstream oss;
        oss << this->coluna;
        switch(codigo) {
         case TYPE::CONSTANT_OUT:
          mensagem = "Numeric constant out of range: column " + oss.str() + ".";
          break;
          case TYPE::ILL_FORMED:
          mensagem = "Ill-formed expression or missing term detected: column " + oss.str() + ".";
          break;
          case TYPE::INVALID_OPERAND:
          mensagem = "Invalid operand: column " + oss.str() + ".";
          break;
          case TYPE::EXTRANEOUS:
          mensagem = "Extraneous symbol: column " + oss.str() + ".";
          break;
          case TYPE::MISSMATCH_O:
          mensagem = "Mismatch \')\': column " + oss.str() + ".";
          break;
          case TYPE::MISSMATCH_C:
          mensagem = "Missing closing \')\' to match opening \'(\' at: column " + oss.str() + ".";
          break;
          case TYPE::LOST_OPERATOR:
          mensagem = "Lost operator: column " + oss.str() + ".";
          break;
          case TYPE::DIVISION_0:
          mensagem = "Division by zero!";
          coluna = -1;
          break;
          case TYPE::OVERFLOW:
          mensagem = "Numeric overflow error!";
          coluna = -1;
          break;
        }
     return mensagem;
    }
private:
    int codigo;
    int coluna;
};

#include "BARES.cpp"
#endif
