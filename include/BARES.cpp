#include "BARES.h"

void BARES::extrair_dados(std::string bruto) {
    this->converter_bruto_fila(bruto);
    //this->Infx2Posfx(bruto, inicio, x);
}

void BARES::Infx2Posfx(std::string bruto, long int inicio, long int fim) {
    while(inicio < fim) {
        
        inicio++;
    }
}

void BARES::converter_bruto_fila(std::string bruto) {
    int *numeros = new int(bruto.size()); // matematicamente falando é impossivel que não caibam todos os valores numericos dessa linha nesse vetor
    this->extrair_numeros(bruto, numeros);
    long int ptr_numeros = 1; // cursor que controla qual numero deve ser lido a seguir
    unsigned long int inicio = 0;
    bool last_is_numeric = false; // guarda se o simbolo anterior é numero ou operador
    bool negativar = false;
    int escopo = 0; // controla a abertura e fechamento de parenteses

    while(inicio < bruto.size()) {
        if(bruto[inicio] == 32 || bruto[inicio] == 9) { // garantindo que espaços e tabulações seram ignorados
            while((bruto[inicio] == 32 || bruto[inicio] == 9) && inicio < bruto.size()) inicio++;
        }
        if(isdigit(bruto[inicio]) && inicio < bruto.size()) {
            if(last_is_numeric) throw(ERRO(ERRO::TYPE::EXTRANEOUS, inicio+1));
            while(isdigit(bruto[inicio]) && inicio < bruto.size()) inicio++; // percorrendo até sair do numero
            if(negativar) {
                negativar = false;
                numeros[ptr_numeros] *= -1;
            }
            this->simbolos->enqueue(numeros[ptr_numeros]); // pegando numero correspondente do vetor de numeros extraidos
            ptr_numeros++; // registrando que o numero foi tirado
            last_is_numeric = true;
        }
        else if(inicio < bruto.size()){ // nesse caso deve ser um simbolo ou um caractere invalido
            if(negativar && bruto[inicio] != BARES::simbolo::ABRIR) throw(ERRO(ERRO::TYPE::EXTRANEOUS, inicio));
            switch(bruto[inicio]) { // existe um valor inteiro negativo para cada operador
                case BARES::simbolo::ABRIR:
                if(last_is_numeric || (!this->simbolos->isEmpty() && this->simbolos->getLast() == BARES::simbolo_token::_FECHAR)) throw(ERRO(ERRO::TYPE::EXTRANEOUS, inicio+1));
                    this->simbolos->enqueue(BARES::simbolo_token::_ABRIR);
                    escopo--;
                    break;
                case BARES::simbolo::FECHAR:
                if(escopo >= 0) throw(ERRO(ERRO::TYPE::MISSMATCH_O, inicio+1));
                if(!last_is_numeric && this->simbolos->getLast() != BARES::simbolo_token::_FECHAR) throw(ERRO(ERRO::TYPE::LOST_OPERATOR, inicio+1));
                    this->simbolos->enqueue(BARES::simbolo_token::_FECHAR);
                    escopo++;
                    break;
                case BARES::simbolo::NEGATIVO: // caso especial pois pode ser menos ou negativo
                if(inicio == bruto.size()-1) throw(ERRO(ERRO::TYPE::LOST_OPERATOR, inicio+1)); //menos perdido no final
                if(!last_is_numeric) negativar = true;
                    this->simbolos->enqueue(BARES::simbolo_token::_NEGATIVO);
                    break;
                case BARES::simbolo::MULTIPLICAR:
                if(!last_is_numeric && (inicio == 0 || this->simbolos->getLast() != BARES::simbolo_token::_FECHAR)) throw(ERRO(ERRO::TYPE::LOST_OPERATOR, inicio+1));
                    this->simbolos->enqueue(BARES::simbolo_token::_MULTIPLICAR);
                    break;
                case BARES::simbolo::DIVIDIR:
                if(!last_is_numeric && (inicio == 0 || this->simbolos->getLast() != BARES::simbolo_token::_FECHAR)) throw(ERRO(ERRO::TYPE::LOST_OPERATOR, inicio+1));
                    this->simbolos->enqueue(BARES::simbolo_token::_DIVIDIR);
                    break;
                case BARES::simbolo::SOMAR:
                if(!last_is_numeric && (inicio == 0 || this->simbolos->getLast() != BARES::simbolo_token::_FECHAR)) throw(ERRO(ERRO::TYPE::LOST_OPERATOR, inicio+1));
                    this->simbolos->enqueue(BARES::simbolo_token::_SOMAR);
                    break;
                case BARES::simbolo::POTENCIA:
                if(!last_is_numeric && (inicio == 0 || this->simbolos->getLast() != BARES::simbolo_token::_FECHAR)) throw(ERRO(ERRO::TYPE::LOST_OPERATOR, inicio+1));
                    this->simbolos->enqueue(BARES::simbolo_token::_POTENCIA);
                    break;
                case BARES::simbolo::RESTO:
                if(!last_is_numeric && (inicio == 0 || this->simbolos->getLast() != BARES::simbolo_token::_FECHAR)) throw(ERRO(ERRO::TYPE::LOST_OPERATOR, inicio+1));
                    this->simbolos->enqueue(BARES::simbolo_token::_RESTO);
                    break;
                case '.':
                    throw(ERRO(ERRO::TYPE::INVALID_OPERAND, inicio+1));
                    break;
                case '=':
                    throw(ERRO(ERRO::TYPE::INVALID_OPERAND, inicio+1));
                    break;
                default:
                    if(!last_is_numeric) throw(ERRO(ERRO::TYPE::ILL_FORMED, inicio+1));
            }
            last_is_numeric = false;
         inicio++;
        }
    }
    if(escopo < 0) { // faltou fechar em algum lugar
       unsigned long int x = 0;
     for(; x < bruto.size() && bruto[x] != BARES::simbolo::ABRIR; x++);
     throw(ERRO(ERRO::TYPE::MISSMATCH_C, x+1));
    }
    if(!last_is_numeric && this->simbolos->getLast() != BARES::simbolo_token::_FECHAR && this->simbolos->getLast() != BARES::simbolo_token::_ABRIR) throw(ERRO(ERRO::TYPE::ILL_FORMED, bruto.size()+1));
    if(!last_is_numeric && this->simbolos->getLast() != BARES::simbolo_token::_FECHAR) throw(ERRO(ERRO::TYPE::ILL_FORMED, bruto.size()));
    delete [] numeros;
}

void BARES::extrair_numeros(std::string string, int *numeros) {
        int controle = 0;
        numeros[0] = 0; // por padrão todos os vetores usados armazenam seu próoprio tamanho em sua primeira posição
        bool encontrei = false;
       unsigned long int i = 0;
       unsigned long int coluna = 0;
        for(; i < string.size(); i++)  { // percorrer todos os caracteres
                if(isdigit(string[i])) {//  verificar se o carácter é numérico
                        if(!encontrei) { // caso o elemento anteior não tenha sido um numero
                           encontrei = true;
                           controle++;
                           numeros[controle] = 0;
                           coluna = i;
                        }
                        numeros[controle] += (int)(string[i])-'0'; // forma que encontrei para converter char em int
                        numeros[controle] *= 10;
                }
                else if(encontrei) { // caso o caracter atual NÃO seja um numero porem o ultimo caracter lido era um numero
                    encontrei = false;
                    numeros[controle] /= 10;
                    if(numeros[controle] > 32767) throw(ERRO(ERRO::TYPE::CONSTANT_OUT, coluna+1));
                    numeros[0]++; // esse elemento controla quantos numeros existem no array
                }
        }
        if(encontrei) {
             encontrei = false;
             numeros[controle] /= 10;
             if(numeros[controle] > 32767) throw(ERRO(ERRO::TYPE::CONSTANT_OUT, coluna+1));
             numeros[0]++; // esse elemento controla quantos numeros existem no array
        }
}

void BARES::processar(std::string nome) {
 std::ifstream arquivo; // instanciando um ponteiro pro arquivo
 arquivo.open(nome); // abrindo o arquivo

 if(!(arquivo.is_open() && arquivo.good()))  { // verificar se abriu e se está "usavel"
   std::cout << "\nFalha !!!  Não foi possivel abrir o arquivo \"" << nome << "\"\n\n";
   exit(0); // saida do programa por erro fatal
 }
 
    std::string linha  = "";
     while(!arquivo.fail()) { // esse loop vai percorrer linha a linha do arquivo
       std::getline(arquivo, linha); // lendo a linha e guardando na string linha
       if(linha == "") break;
       
       try {
       this->extrair_dados(linha);
       std::cout << linha << std::endl; // debug
       this->print();
       }
       catch(ERRO erro) {
           int coluna = 0;
           std::cout << erro.msg(coluna);
           if (coluna >= 0) std::cout << coluna << ".";
           std::cout << std::endl;
           this->simbolos->makeEmpty();
       }
     }
    arquivo.close();
}

void BARES::print() {
    std::cout << "{";
    while(!simbolos->isEmpty()) std::cout << simbolos->dequeue() << "|";
    std::cout << "}\n";
}