/* 
 * File:   main.cpp
 * Author: matheus
 *
 * Created on 24 de Abril de 2016, 10:11
 */

#include <iostream>
#include <fstream>
#include "BARES.h"

int main(int argc, char** argv) {
    std::string nome;
    if ( argc > 1 )
    {
        nome =  argv[1]; // recebe o nome do arquivo por parametro
    }
    else
    {
        std::cout << "\nErro !!! O local do arquivo de entrada não foi passado por parâmetro\n\n";
        return false;
    }
 
 BARES analizador;
 std::string retorno = analizador.processar(nome);
 std::cout << retorno;
 
  std::ofstream arquivo; // instanciando um ponteiro pro arquivo
  arquivo.open("SAIDA.txt"); // abrindo o arquivo
  arquivo << retorno;
  arquivo.close();
  
   std::cout << "\n\n>>>>>>>>>>>>> Arquivo gerado: \"SAIDA.txt\"\n";
    return (EXIT_SUCCESS);
}

