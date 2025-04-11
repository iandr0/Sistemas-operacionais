#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <algorithm>
#include <limits>

using namespace std;


int fifo(const vector<int>& referencias, int numQuadros) {
    int paginasFalt = 0;
    queue<int> fifoQueue;
    vector<int> quadros;

    for (int pagina : referencias) {
        if (find(quadros.begin(), quadros.end(), pagina) == quadros.end()) {
            paginasFalt++;

            // isso aqui verifica, se a pagína já ta em algum quadro
            //o if condicional daqui serve pra checkar isso
            //se for igual a quadros.end entao n achou nada
            //se n achou, ent falta pagina, ai incrementa

            if (quadros.size() < numQuadros) {
                quadros.push_back(pagina);
                fifoQueue.push(pagina);
            } else {
                int removerPag = fifoQueue.front();
                //remove a bagaça que veio primeiro, se não tiver espaço
                fifoQueue.pop();
                //remove do vetor quadros
                auto it = find(quadros.begin(), quadros.end(), removerPag);
                if (it != quadros.end()) {
                    *it = pagina;
                    // aqui nem precisa pensar mt, se ele achar it(removerpag), então ele vai substituir
                }
                fifoQueue.push(pagina);
                //independente do que acontecer, a pagina vai pro fim da fila
            }
        }
        // Caso contrário, página já está na RAM: não faz nada
    }
    return paginasFalt;
}


int otimo(const vector<int>& referencias, int numQuadros) {
    int paginaFalt = 0;
    vector<int> quadros;

    int n = referencias.size();
    for (int i = 0; i < n; i++) {
        int pagAtual = referencias[i];

        if (find(quadros.begin(), quadros.end(), pagAtual) != quadros.end()) {
            continue;
        }
        //basicamente, se a pagina atual já ta em um dos quadros, ele faz nada

        paginaFalt++;


        if (quadros.size() < numQuadros) {
            quadros.push_back(pagAtual);
        } else {

            int indiceMod = -1;
            int distanciaLonga = -1;

            for (int j = 0; j < quadros.size(); j++) {
                int paginaDentro = quadros[j];
                int distancia = 0;
                bool encontrado = false;
                for (int k = i + 1; k < n; k++) {
                    distancia++;
                    if (referencias[k] == paginaDentro) {
                        // se isso aqui for verdadeiro, achamos a proxima vez que a pagina vai ser usada
                        encontrado = true;
                        break;
                    }
                }
                // Se a página não for utilizada no futuro, escolhe-a imediatamente
                if (!encontrado) {
                    indiceMod = j;
                    break;
                } else if (distancia > distanciaLonga) {
                    /* se ela for ser usada depois, a gente ve se a distancia dela é a mais longa, se for,
                     ela e a atual melhor candidata pra ser selecionada*/
                    distanciaLonga = distancia;
                    // att a distancia longa como essa
                    indiceMod = j;
                }
            }

            quadros[indiceMod] = pagAtual;
            //atualiza o valor do quadro com a melhor candidata
        }
    }
    return paginaFalt;
}


int LRU(const vector<int>& referencias, int numQuadros) {
    int paginaFalt = 0;
    vector<int> quadros;
    // Vetor para manter o índice da última utilização para cada página presente nos frames
    // A estratégia é, quando ocorrer uma falta e os quadros estiverem cheios,
    // procurar a página cujo último uso foi há mais tempo.
    vector<int> ultimoUsado;

    for (int i = 0; i < referencias.size(); i++) {
        int pagAtual = referencias[i];
        auto it = find(quadros.begin(), quadros.end(), pagAtual);
        //aqui a gente verifica se a pagatual ta nos quadros
        if (it != quadros.end()) {

            int indice = distance(quadros.begin(), it);
            ultimoUsado[indice] = i;
            //se tiver, atualiza o valor do ultimo usado
        } else {
            // Falta de página
            paginaFalt++;


            if (quadros.size() < numQuadros) {
                quadros.push_back(pagAtual);
                ultimoUsado.push_back(i);
                //se tiver espaço ainda, ele insere a pagina e registra o tempo de acesso
            } else {
                // Se os quadros estão cheios, encontra o índice do quadro cuja última utilização foi a mais antiga
                int lruIndice = 0;
                int usagemMin = ultimoUsado[0];
                for (int j = 1; j < ultimoUsado.size(); j++) {
                    if (ultimoUsado[j] < usagemMin) {
                        usagemMin = ultimoUsado[j];
                        lruIndice = j;
                        //encontra o indice mais antigo
                    }
                }

                quadros[lruIndice] = pagAtual;
                //att a pagina
                ultimoUsado[lruIndice] = i;
                // att o tempo
            }
        }
    }
    return paginaFalt;
}

int main() {
    string inputFileName = "entrada.txt";

    ifstream inputFile(inputFileName);
    if (!inputFile.is_open()) {
        cerr << "Erro ao abrir o arquivo: " << inputFileName << endl;
        return 1;
    }

    int numFrames;
    inputFile >> numFrames;

    vector<int> references;
    int page;
    while (inputFile >> page) {
        references.push_back(page);
    }
    inputFile.close();


    int fifoFaltas = fifo(references, numFrames);
    int otimoFaltas = otimo(references, numFrames);
    int lruFaltas = LRU(references, numFrames);

    cout << "FIFO " << fifoFaltas << endl;
    cout << "OTM " << otimoFaltas << endl;
    cout << "LRU " << lruFaltas << endl;

    return 0;
}
