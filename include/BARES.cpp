#include "BARES.h"

QueueAr<int>* BARES::Infx2Posfx(QueueAr<int> *fila) {
 StackINT pilha;
 int symb;
 QueueAr<int> *fila_saida = new QueueAr<int>;
  while(!fila->isEmpty()) {
    symb = fila->dequeue();
    if(symb < 40000) {
      fila_saida->enqueue(symb);
    }
    else {
        while(!pilha.isEmpty() && pilha.top() <= symb){
          if(pilha.top() <= symb) {
            fila_saida->enqueue(pilha.pop());
          }
        }
        pilha.push(symb);
    }
  }
  while(!pilha.isEmpty())
    fila_saida->enqueue(pilha.pop());
    return fila_saida;
}

int BARES::AvalPosfixa(QueueAr<int> *fila) {
 StackINT pilha;
 int operandoA, operandoB, resultado;
 int symb;
  while(!fila->isEmpty()) {
    symb = fila->dequeue();
    if(symb < 40000) {
      pilha.push(symb);
    }
    else {
        operandoB = pilha.pop();
        operandoA = pilha.pop();
        
        switch(symb) {
            case BARES::simbolo_token::_SUBTRAIR:
                resultado = operandoA - operandoB;
            break;
            case BARES::simbolo_token::_SOMAR:
                resultado = operandoA + operandoB;
            break;
            case BARES::simbolo_token::_MULTIPLICAR:
                resultado = operandoA * operandoB;
            break;
            case BARES::simbolo_token::_DIVIDIR:
                if(operandoB == 0) {
                    fila->makeEmpty(); // fila é um ponteiro alocado dinamicamente que se não for esvaziado vai dar erro
                    throw(ERRO(ERRO::TYPE::DIVISION_0));
                }
                resultado = operandoA / operandoB;
            break;
            case BARES::simbolo_token::_RESTO:
                resultado = operandoA % operandoB;
            break;
            case BARES::simbolo_token::_POTENCIA:
              if(operandoB < 0) return 0;
              else if(operandoB == 0) return 1;
              else {
                resultado = operandoA;
                for(;operandoB > 1; operandoB--)
                  resultado *= operandoA;
              }
            break;
        }
        pilha.push(resultado);
        }
    }
    resultado = pilha.pop();
    if(resultado > 32767) throw(ERRO(ERRO::TYPE::OVERFLOW));
    return resultado;
}

void BARES::converter_bruto_fila(std::string bruto) {
    long int numero = 0; // cursor que controla qual numero deve ser lido a seguir
    unsigned long int inicio = 0;
    bool last_is_numeric = false; // guarda se o simbolo anterior é numero ou operador
    bool negativar = false;
    int escopo = 0; // controla a abertura e fechamento de parenteses
     unsigned long int x;

    while(inicio < bruto.size()) {
        if(bruto[inicio] == 32 || bruto[inicio] == 9) { // garantindo que espaços e tabulações seram ignorados
            while((bruto[inicio] == 32 || bruto[inicio] == 9) && inicio < bruto.size()) inicio++;
        }
        if(isdigit(bruto[inicio]) && inicio < bruto.size()) {
            if(last_is_numeric) throw(ERRO(ERRO::TYPE::EXTRANEOUS, inicio+1));
            x = inicio;
            for(x = inicio;isdigit(bruto[x]) && x < bruto.size(); x++) { // converter em numero
                numero += (bruto[x] - '0');
                numero *= 10;
            }
            numero /= 10; // coriguindo a "volta extra"
            if(numero > 32767) throw(ERRO(ERRO::TYPE::CONSTANT_OUT, inicio+1)); // exeção prevenida
            if(negativar) {
                negativar = false;
                numero *= -1;
            }
            inicio = x;
            this->simbolos->enqueue(numero); // pegando numero correspondente do vetor de numeros extraidos
            numero = 0;
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
                   else this->simbolos->enqueue(BARES::simbolo_token::_SUBTRAIR);
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
}

std::string BARES::processar(std::string nome) {
    std::string retorno = "";
 std::ifstream arquivo; // instanciando um ponteiro pro arquivo
 arquivo.open(nome); // abrindo o arquivo

 if(!(arquivo.is_open() && arquivo.good()))  { // verificar se abriu e se está "usavel"
   std::cout << "\nFalha !!!  Não foi possivel abrir o arquivo \"" << nome << "\"\n\n";
   exit(0); // saida do programa por erro fatal
 }
 
    QueueAr<int>  *posfix = nullptr;
    long int ptr_interno = -1;
    long int ptr_final_interno;
    int *fila = new int[10000];
    QueueAr<int> *auxiliar = new QueueAr<int>(10);
    long int capacidade = 0;
    int resultado = 0;
 
    std::string linha  = "";
     while(!arquivo.fail()) { // esse loop vai percorrer linha a linha do arquivo
       std::getline(arquivo, linha); // lendo a linha e guardando na string linha
       if(linha == "") break;
       delete posfix;
       posfix = nullptr;
       try {
       this->converter_bruto_fila(linha);
    // executando o processamento

    for(capacidade = 0; !this->simbolos->isEmpty(); capacidade++) fila[capacidade] = this->simbolos->dequeue();

     ptr_interno = -1;
    for(ptr_final_interno = 0; ptr_final_interno < capacidade && fila[ptr_final_interno] != BARES::simbolo_token::_FECHAR; ptr_final_interno++)
        if(fila[ptr_final_interno] == BARES::simbolo_token::_ABRIR)
            ptr_interno = ptr_final_interno;
    
    while(ptr_interno >= 0) { // loop principal, retira e processa tudo que estiver entrre parenteses
        for(long int x = ptr_interno+1; fila[x] != BARES::simbolo_token::_FECHAR; x++) {
            auxiliar->enqueue(fila[x]);
        }

        posfix = this->Infx2Posfx(auxiliar);

        // processar posfix e recolocar no vetor
        resultado = this->AvalPosfixa(posfix);
        delete posfix;
        posfix = nullptr;
        fila[ptr_interno] = resultado;
        long int x = ptr_interno+1;
        for(; (ptr_final_interno+1) + (x - (ptr_interno+1)) < capacidade;x++)
            fila[x] = fila[(ptr_final_interno+1) + (x - (ptr_interno+1))];
        capacidade -= (ptr_final_interno - ptr_interno);
        
        ptr_interno = -1;
       for(ptr_final_interno = 0; ptr_final_interno < capacidade && fila[ptr_final_interno] != BARES::simbolo_token::_FECHAR; ptr_final_interno++)
        if(fila[ptr_final_interno] == BARES::simbolo_token::_ABRIR)
            ptr_interno = ptr_final_interno;
        }

      for(long int x = 0; x < capacidade; x++) {
            auxiliar->enqueue(fila[x]);
        }
        posfix = this->Infx2Posfx(auxiliar);
        resultado = this->AvalPosfixa(posfix);
        
         std::ostringstream *oss = new std::ostringstream;
         *oss << resultado;
         retorno += oss->str() + "\n";
         delete oss;
       } // fim da captura de erro
       catch(ERRO erro) {
           retorno += erro.msg();
           retorno += "\n";
           this->simbolos->makeEmpty();
       }
     }
    arquivo.close();
    
    delete auxiliar;
    delete posfix;
    delete [] fila;
    
    return retorno;
}
