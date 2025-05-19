#include <stdio.h>
 #include <stdlib.h>
 #include <string.h>
 #include <stdbool.h>
 #include <time.h>
 #include <stddef.h>
 #include "trabalhoeda2.h"  
 
#pragma warning (disable : 4996)
#pragma warning (disable : 6031)


#pragma region ANTENAS

// Cria uma nova antena
/**
 * @brief Cria e inicializa uma nova antena com coordenadas e frequência especificadas.
 *
 * Esta função é responsável por alocar dinamicamente memória para uma nova antena,
 * definir as suas coordenadas (x, y), a frequência de funcionamento, e inicializar
 * os restantes campos, como o estado de visita, a lista de adjacências e apontador
 * para a próxima antena.
 *
 * @param x Coordenada X da antena.
 * @param y Coordenada Y da antena.
 * @param frequencia Carácter que representa a frequência da antena (ex.: 'A', 'B', ...).
 * @return Retorna um apontador para a antena criada, ou NULL em caso de falha na alocação de memória.
 */
Antena* CriarAntena(int x, int y, char frequencia) {
    Antena* nova_Antena = (Antena*)malloc(sizeof(Antena));
    if (!nova_Antena) return NULL;

    nova_Antena->x = x;
    nova_Antena->y = y;
    nova_Antena->frequencia = frequencia;
    nova_Antena->visitado = 0;
    nova_Antena->adj = NULL;
    nova_Antena->proximo = NULL;

    return nova_Antena;
}

/**
 * @brief Procura uma antena numa lista ligada através das suas coordenadas.
 *
 * Percorre a lista de antenas a partir da cabeça, comparando as coordenadas
 * de cada antena com os valores fornecidos. Retorna a antena correspondente
 * se for encontrada.
 *
 * @param cabeca apontador para a primeira antena da lista.
 * @param x Coordenada X da antena a procurar.
 * @param y Coordenada Y da antena a procurar.
 * @return apontador para a antena encontrada, ou NULL se não existir.
 */
// Verifica se uma antena com as mesmas coordenadas ja existe
Antena* ProcurarAntenaPorCoordenada(Antena* cabeca, int x, int y) {
    Antena* atual = cabeca;
    while (atual != NULL) {
        if (atual->x == x && atual->y == y) {
            return atual;
        }
        atual = atual->proximo;
    }
    return NULL;
}

// Funcao para criar antena manualmente
/**
 * @brief Insere manualmente uma nova antena no grafo, se não existir.
 *
 * Verifica se já existe uma antena nas coordenadas especificadas. Caso não exista,
 * cria uma nova antena com os dados fornecidos e insere-a de forma ordenada
 * na lista ligada de antenas do grafo.
 *
 * @param g apontador para o grafo onde a antena será inserida.
 * @param x Coordenada X da nova antena.
 * @param y Coordenada Y da nova antena.
 * @param freq Frequência da nova antena.
 * @param resultado apontador para um inteiro onde será armazenado o estado da operação:
 *        -  1: sucesso
 *        - -1: antena já existe
 *        - -2: erro de alocação de memória
 *        -  0: erro geral
 * @return apontador para a nova antena criada, ou NULL em caso de erro.
 */
Antena* inserirAntenaManual(grafo* g, int x, int y, char freq, int* resultado) {
    *resultado = 0;  // status padrao: erro geral

    // Verifica se a antena já existe
    if (ProcurarAntenaPorCoordenada(g->cabeca, x, y)) {
        *resultado = -1; // antena duplicada
        return NULL;
    }

    Antena* nova = CriarAntena(x, y, freq);
    if (!nova) {
        *resultado = -2; // erro de alocacao
        return NULL;
    }

    // Insere ordenadamente e atualiza cabeca do grafo
    g->cabeca = InsereAntenaOrdenada(g->cabeca, nova);

    *resultado = 1; // sucesso
    return nova;
}
/**
 * @brief Insere uma nova antena na lista ligada de forma ordenada por coordenadas.
 *
 * Esta função insere a nova antena na lista mantendo a ordenação por coordenadas X e Y.
 * Se já existir uma antena com as mesmas coordenadas, a nova é descartada.
 *
 * @param cabeca apontador para a cabeça da lista de antenas.
 * @param nova_Antena apontador para a nova antena a ser inserida.
 * @return apontador para a nova cabeça da lista (que pode ter sido atualizada).
 */
// Insere uma nova antena na lista ordenada por coordenadas (x, y)
Antena * InsereAntenaOrdenada(Antena* cabeca, Antena* nova_Antena) {

    if (ProcurarAntenaPorCoordenada(cabeca, nova_Antena->x, nova_Antena->y)!=NULL) {
        free(nova_Antena);
        return 0;
    }

    // Caso especial: lista vazia ou nova antena deve ser a primeira
    if (cabeca == NULL || 
        (nova_Antena->x < cabeca->x) || 
        (nova_Antena->x == cabeca->x && nova_Antena->y < cabeca->y)) {
        // Insere no início
        nova_Antena->proximo = cabeca;
        return nova_Antena;  // nova cabeca da lista
    }

    Antena* atual = cabeca;
    while (atual->proximo != NULL && 
          ((atual->proximo->x < nova_Antena->x) || 
           (atual->proximo->x == nova_Antena->x && atual->proximo->y < nova_Antena->y))) {
        atual = atual->proximo;
    }

    nova_Antena->proximo = atual->proximo;
    atual->proximo = nova_Antena;
    return cabeca;
}

/**
 * @brief Remove uma antena da lista ligada com base nas coordenadas fornecidas.
 *
 * Procura uma antena com as coordenadas (linha, coluna) e remove-a da lista, libertando a memória.
 * Indica o sucesso da operação através do parâmetro booleano.
 *
 * @param cabeca apontador para a cabeça da lista de antenas.
 * @param linha Coordenada X da antena a remover.
 * @param coluna Coordenada Y da antena a remover.
 * @param sucesso apontador para um booleano que indica se a remoção foi bem-sucedida.
 * @return apontador para a nova cabeça da lista (pode ser a mesma ou atualizada).
 */
Antena* removerAntena(Antena* cabeca, int linha, int coluna, bool* sucesso) {
    Antena* atual = cabeca;
    Antena* anterior = NULL;
    *sucesso = false;
    
    while (atual != NULL && !(atual->x == linha && atual->y == coluna)) {
        anterior = atual;
        atual = atual->proximo;
    }
    
    if (atual == NULL) return cabeca;
    
    *sucesso = true;
    
    if (anterior == NULL) {
        Antena* nova_cabeca = atual->proximo;
        free(atual);
        return nova_cabeca;
    } else {
        anterior->proximo = atual->proximo;
        free(atual);
        return cabeca;
    }
}
/**
 * @brief Conta o número total de antenas na lista ligada.
 *
 * Percorre a lista de antenas e devolve o número total de elementos presentes.
 *
 * @param cabeca apontador para a cabeça da lista de antenas.
 * @return Número total de antenas na lista.
 */
int contarAntenas(Antena* cabeca) {
    int count = 0;
    while (cabeca) {
        count++;
        cabeca = cabeca->proximo;
    }
    return count;
}

#pragma endregion 

#pragma region ADJ
/**
 * @brief Cria uma nova adjacência para uma antena.
 *
 * Aloca memória para uma nova estrutura de adjacência e define o seu destino.
 *
 * @param destino apontador para a antena de destino da adjacência.
 * @return apontador para a nova adjacência criada, ou NULL em caso de erro.
 */
Adjacencias* CriaAdjacencia(Antena* destino) {

    Adjacencias* adj = (Adjacencias*)malloc(sizeof(Adjacencias));
    if (!adj) return NULL;
    adj->destino = destino;
    adj->proximo = NULL;
    return adj;
}
/**
 * @brief Adiciona uma adjacência entre duas antenas com a mesma frequência.
 *
 * Cria uma nova ligação entre a antena e o vizinho, caso tenham a mesma frequência.
 *
 * @param antena apontador para a antena de origem.
 * @param vizinho apontador para a antena de destino (vizinha).
 * @return 1 em caso de sucesso, 0 caso as frequências sejam diferentes ou ocorra erro de alocação.
 */
// Adiciona uma adjacencia entre duas antenas da mesma frequencia
int AddAdjacencias(Antena* antena, Antena* vizinho) {
    if (antena->frequencia != vizinho->frequencia) {
        return 0;
    }

    Adjacencias* nova_adj = (Adjacencias*)malloc(sizeof(Adjacencias));
    if (!nova_adj) return 0;

    nova_adj->destino = vizinho;
    nova_adj->proximo = antena->adj;
    antena->adj = nova_adj;

    return 1;
}

/**
 * @brief Cria adjacências entre as antenas do grafo com a mesma frequência.
 *
 * Percorre todas as antenas do grafo e cria ligações bidireccionais entre 
 * aquelas que partilham a mesma frequência. As ligações são adicionadas 
 * à lista de adjacências de cada antena.
 *
 * @param grafo apontador para o grafo que contém as antenas.
 */

void  CriarAdjacencias(grafo* grafo) {
    if (grafo->cabeca == NULL) {
        printf("Grafo vazio. Nada para processar.\n");
        return;
    }

    printf("\n--- Lista de Adjacencias Criadas ---\n");
    
    for (Antena* a = grafo->cabeca; a != NULL; a = a->proximo) {
        printf("\nAntena (%d,%d) [Freq: %c] conectada a:\n", 
               a->x, a->y, a->frequencia);
        
        int conexoes = 0;
        
        for (Antena* b = a->proximo; b != NULL; b = b->proximo) {
            if (a->frequencia == b->frequencia) {
                // Cria as conexoes
                if (AddAdjacencias(a, b)) {
                    AddAdjacencias(b, a);  // Grafo nao direcionado
                    printf("  -> (%d,%d) [Freq: %c]\n", 
                           b->x, b->y, b->frequencia);
                    conexoes++;
                }
            }
        }
        
        if (conexoes == 0) {
            printf("  Nenhuma conexao criada\n");
        }
    }
    
    printf("\n--- Todas adjacencias foram processadas ---\n");
}


/**
 * @brief Liberta a memória associada à lista de linhas do arquivo.
 *
 * Percorre a lista ligada de linhas e liberta a memória de cada nó,
 * incluindo o conteúdo de texto armazenado.
 *
 * @param lista apontador para a cabeça da lista de linhas a libertar.
 */
#pragma endregion


#pragma region CARREGAR E BIN

// Funcao para liberar memoria das linhas do arquivo

void  LibertarLinhasArquivo(LinhaArquivo* lista) {
    while (lista != NULL) {
        LinhaArquivo* temp = lista;
        lista = lista->proximo;
        free(temp->conteudo);
        free(temp);
    }
}


// Carrega as antenas de um arquivo para o grafo
/**
 * @brief Carrega dados de antenas a partir de um ficheiro de texto.
 *
 * Lê o conteúdo de um ficheiro de texto, linha por linha, interpretando
 * os caracteres como antenas com coordenadas e frequência, e insere-as no grafo.
 *
 * @param grafo apontador para o grafo onde as antenas serão inseridas.
 * @param filename Nome do ficheiro de onde os dados serão lidos.
 * @param success apontador para indicar se a operação foi bem-sucedida (true) ou não (false).
 * @param message Mensagem descritiva do resultado da operação (erro ou sucesso).
 * @return Retorna 1 em caso de sucesso, 0 caso contrário.
 */

int CarregarArquivo(grafo* grafo, const char* filename, bool* success, char* message) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        strcpy(message, "Erro ao abrir o arquivo.");
        *success = false;
        return 0;
    }

    LinhaArquivo* lista_linhas = NULL;
    LinhaArquivo* ultima_linha = NULL;
    char* linha = NULL;
    size_t tamanho = 0;
    ssize_t lido;
    int num_linhas = 0;
    int max_colunas = 0;

    while ((lido = getline(&linha, &tamanho, file)) != -1) {
        if (linha[lido - 1] == '\n') {
            linha[lido - 1] = '\0';
            lido--;
        }

        LinhaArquivo* nova_linha = malloc(sizeof(LinhaArquivo));
        if (!nova_linha) {
            strcpy(message, "Erro de alocacao de memoria.");
            *success = false;
            fclose(file);
            free(linha);
            LibertarLinhasArquivo(lista_linhas);
            return 0;
        }

        nova_linha->conteudo = strdup(linha);
        nova_linha->proximo = NULL;

        if (lista_linhas == NULL) {
            lista_linhas = nova_linha;
            ultima_linha = nova_linha;
        } else {
            ultima_linha->proximo = nova_linha;
            ultima_linha = nova_linha;
        }

        int colunas = 0;
        for (int i = 0; linha[i] != '\0'; i++) {
            if (linha[i] != ' ') colunas++;
        }
        if (colunas > max_colunas) max_colunas = colunas;
        num_linhas++;
    }

    free(linha);
    fclose(file);

    if (num_linhas == 0 || max_colunas == 0) {
        strcpy(message, "Arquivo vazio ou formato invalido.");
        *success = false;
        LibertarLinhasArquivo(lista_linhas);
        return 0;
    }

    LinhaArquivo* atual = lista_linhas;
    for (int linha_idx = 0; linha_idx < num_linhas && atual != NULL; linha_idx++) {
        int coluna_idx = 0;
        for (int j = 0; atual->conteudo[j] != '\0'; j++) {
            if (atual->conteudo[j] != ' ') {
                if (atual->conteudo[j] != CHAR_VAZIO) {
                    Antena* nova_antena = CriarAntena(linha_idx, coluna_idx, atual->conteudo[j]);
                    if (InsereAntenaOrdenada(grafo->cabeca, nova_antena)) {
                        // Atualiza o cabeca se necessário
                        if (grafo->cabeca == NULL || 
                            (nova_antena->x < grafo->cabeca->x) || 
                            (nova_antena->x == grafo->cabeca->x && nova_antena->y < grafo->cabeca->y)) {
                            nova_antena->proximo = grafo->cabeca;
                            grafo->cabeca = nova_antena;
                        }
                    } else {
                        // Antena duplicada ignorada
                        free(nova_antena);
                    }
                }
                coluna_idx++;
            }
        }
        atual = atual->proximo;
    }

    LibertarLinhasArquivo(lista_linhas);
    strcpy(message, "Dados importados com sucesso.");
    *success = true;
    return 1;
}

// Funcao SaveGrafoBin com apontadors simples
/**
 * @brief Guarda o grafo num ficheiro binário.
 *
 * Esta função escreve os dados do grafo, incluindo as antenas e as suas adjacências,
 * num ficheiro binário especificado. É utilizado um cabeçalho identificador, seguido do
 * número de antenas, os seus dados, e as ligações de adjacência entre elas.
 *
 * @param fileName Nome do ficheiro binário onde os dados serão guardados.
 * @param graph apontador para o grafo a ser guardado.
 * @return true se o processo for bem-sucedido, false caso contrário.
 */
bool SaveGrafoBin(const char* fileName, grafo* graph) {
    if (!fileName || !graph) return false;

    FILE* file = fopen(fileName, "wb");
    if (!file) return false;

    // Escreve cabecalho
    const char header[] = "ANTG";
    if (fwrite(header, sizeof(char), 4, file) != 4) {
        fclose(file);
        return false;
    }

    // Conta antenas
    int count = contarAntenas(graph->cabeca);
    if (fwrite(&count, sizeof(int), 1, file) != 1) {
        fclose(file);
        return false;
    }

    // Escreve antenas
    Antena* atual = graph->cabeca;
    while (atual) {
        AntenaBin registro = {atual->x, atual->y, atual->frequencia};
        if (fwrite(&registro, sizeof(AntenaBin), 1, file) != 1) {
            fclose(file);
            return false;
        }
        atual = atual->proximo;
    }

    // Escreve adjacencias
    atual = graph->cabeca;
    while (atual) {
        // Conta adjacencias
        int adjCount = 0;
        Adjacencias* adj = atual->adj;
        while (adj) { adjCount++; adj = adj->proximo; }
        
        if (fwrite(&adjCount, sizeof(int), 1, file) != 1) {
            fclose(file);
            return false;
        }

        // Escreve adjacencias
        adj = atual->adj;
        while (adj) {
            // Encontra índice do destino
            int index = 0;
            Antena* temp = graph->cabeca;
            while (temp && temp != adj->destino) {
                index++;
                temp = temp->proximo;
            }
            
            if (!temp || fwrite(&index, sizeof(int), 1, file) != 1) {
                fclose(file);
                return false;
            }
            adj = adj->proximo;
        }
        atual = atual->proximo;
    }

    fclose(file);
    return true;
}

/**
 * @brief Guarda o grafo num ficheiro de texto no formato de matriz.
 *
 * Esta função exporta o conteúdo do grafo para um ficheiro de texto,
 * representando as antenas numa matriz de caracteres, onde posições
 * sem antenas são preenchidas com o caracter definido por `CHAR_VAZIO`.
 *
 * Cada linha do ficheiro representa uma linha da matriz, com as colunas
 * separadas por espaços. As dimensões da matriz são determinadas com base
 * nas coordenadas máximas das antenas presentes no grafo.
 *
 * @param fileName Nome do ficheiro onde o grafo será guardado.
 * @param graph apontador para o grafo a ser exportado.
 * @return 
 * -  1 em caso de sucesso,
 * - -1 se os parâmetros forem inválidos,
 * - -2 se a alocação de memória falhar,
 * - -3 se ocorrer erro ao abrir o ficheiro.
 */
int SaveGrafoMatriz(const char* fileName, grafo* graph) {
    if (!fileName || !graph) {
        return -1;  // Parâmetros inválidos
    }

    // Determinar as dimensoes da matriz
    int max_x = 0, max_y = 0;
    for (Antena* atual = graph->cabeca; atual != NULL; atual = atual->proximo) {
        if (atual->x > max_x) max_x = atual->x;
        if (atual->y > max_y) max_y = atual->y;
    }

    // Ajustar para índices base 0 (adicionar 1 para obter o tamanho)
    int linhas = max_x + 1;
    int colunas = max_y + 1;

    // Criar matriz unidimensional
    char* matriz = (char*)malloc(linhas * colunas * sizeof(char));
    if (!matriz) {
        return -2;  // Falha na alocacao
    }

    // Inicializar matriz com pontos
    for (int i = 0; i < linhas * colunas; i++) {
        matriz[i] = CHAR_VAZIO;
    }

    // Preencher a matriz com as antenas
    for (Antena* atual = graph->cabeca; atual != NULL; atual = atual->proximo) {
        if (atual->x < linhas && atual->y < colunas) {
            matriz[atual->x * colunas + atual->y] = atual->frequencia;
        }
    }

    // Escrever no arquivo
    FILE* file = fopen(fileName, "w");
    if (!file) {
        free(matriz);
        return -3;  // Falha ao abrir arquivo
    }

    for (int i = 0; i < linhas; i++) {
        for (int j = 0; j < colunas; j++) {
            fprintf(file, "%c ", matriz[i * colunas + j]);
        }
        fprintf(file, "\n");
    }

    fclose(file);
    free(matriz);
    return 1;  // Sucesso
}




#pragma endregion 

#pragma region DFS
/**
 * @brief Reinicia o estado de visita de todas as antenas.
 *
 * Esta função percorre a lista de antenas e define o campo `visitado` como 0
 * (não visitado), permitindo reutilizar a estrutura do grafo em novas operações
 * de percurso ou análise.
 *
 * @param cabeca apontador para a primeira antena da lista ligada.
 */

void  ResetVisitas(Antena* cabeca) {
    while (cabeca) {
        cabeca->visitado = 0;
        cabeca = cabeca->proximo;
    }
}

/**
 * @brief Realiza uma travessia em profundidade (DFS) recursiva a partir de uma antena.
 *
 * Marca a antena como visitada e imprime a sua posição e frequência.
 * A travessia continua recursivamente para todas as antenas adjacentes que ainda
 * não tenham sido visitadas.
 *
 * @param antena apontador para a antena de início da travessia.
 * @return true se a travessia foi iniciada com sucesso; false caso a antena seja nula ou já tenha sido visitada.
 */
bool DfsRecursiva(Antena* antena) {
    if (!antena || antena->visitado) return false;
    
    antena->visitado = 1;
    printf("(%d, %d) [Freq: %c]\n", antena->x, antena->y, antena->frequencia);

    Adjacencias* adj = antena->adj;
    while (adj) {
        DfsRecursiva(adj->destino);
        adj = adj->proximo;
    }
    return true;
}

/**
 * @brief Executa uma travessia em profundidade (DFS) a partir de uma antena específica.
 *
 * Procura a antena com as coordenadas fornecidas, reinicia o estado de visita de todas
 * as antenas no grafo e inicia a DFS a partir da antena encontrada.
 * Se a antena não for encontrada, imprime uma mensagem de erro.
 *
 * @param g apontador para o grafo onde se realiza a travessia.
 * @param x Coordenada da linha da antena de início.
 * @param y Coordenada da coluna da antena de início.
 */

bool  ExecutarDFS(grafo* g, int x, int y) {
    Antena* inicio = ProcurarAntenaPorCoordenada(g->cabeca, x, y);
    if (!inicio) {
        //printf("Antena em (%d,%d) nao encontrada!\n", x, y);
        return false;
    }
    
    ResetVisitas(g->cabeca);
    //printf("\nDFS iniciando em (%d,%d):\n", x, y);
    DfsRecursiva(inicio);
    return true;
}

#pragma endregion 


#pragma region BFS

/**
 * @brief Função auxiliar recursiva para realizar a travessia em largura (BFS).
 *
 * Esta função processa os elementos da fila recursivamente, imprimindo as coordenadas
 * das antenas e adicionando os seus vizinhos não visitados à fila.
 *
 * @param fila Array de antenas usado como fila de processamento.
 * @param inicio Índice do início da fila.
 * @param fim Índice do fim atual da fila.
 */
void  BfsRecursivaUtil(Antena* fila, int inicio, int fim) {
    if (inicio >= fim) return;

    Antena* atual = &fila[inicio];
    printf("(%d, %d)\n", atual->x, atual->y);

    Adjacencias* adj = atual->adj;
    while (adj) {
        if (!adj->destino->visitado) {
            adj->destino->visitado = 1;
            fila[fim++] = *adj->destino;
        }
        adj = adj->proximo;
    }

    BfsRecursivaUtil(fila, inicio + 1, fim);
}

/**
 * @brief Inicia a travessia em largura (BFS) de forma recursiva a partir de uma antena.
 *
 * Marca a antena de origem como visitada, inicializa a fila e chama a função recursiva
 * auxiliar para percorrer o grafo.
 *
 * @param origem apontador para a antena de origem da travessia.
 * @param total_antenas Número total de antenas no grafo (usado para alocar a fila).
 */

bool  BfsRecursiva(Antena* origem, int total_antenas) {
    if (!origem) return false;
    
    Antena* fila = malloc(sizeof(Antena) * total_antenas);
    if(!fila) return false;

    int inicio = 0, fim = 0;

    origem->visitado = 1;
    fila[fim++] = *origem;

    BfsRecursivaUtil(fila, inicio, fim);
    free(fila);
    return true;
}

/**
 * @brief Executa a travessia em largura (BFS) a partir de uma antena especificada.
 *
 * Procura a antena nas coordenadas fornecidas, reinicia o estado de visitas de todas
 * as antenas no grafo e inicia a BFS recursiva a partir da antena encontrada.
 *
 * @param g apontador para o grafo.
 * @param x Coordenada da linha da antena de início.
 * @param y Coordenada da coluna da antena de início.
 */
void  ExecutarBFS(grafo* g, int x, int y) {
    Antena* inicio = ProcurarAntenaPorCoordenada(g->cabeca, x, y);
    if (!inicio) {
        printf("Antena em (%d,%d) nao encontrada!\n", x, y);
        return;
    }
    
    ResetVisitas(g->cabeca);
    printf("\nBFS iniciando em (%d,%d):\n", x, y);
    BfsRecursiva(inicio, contarAntenas(g->cabeca));
}

#pragma endregion 

#pragma region TRAVESSIA
/**
 * @brief Mostra todos os caminhos possíveis entre duas antenas utilizando backtracking.
 *
 * Esta função percorre recursivamente todos os caminhos possíveis entre a antena de origem
 * e a antena de destino, imprimindo cada caminho encontrado.
 * Utiliza uma abordagem de backtracking para evitar ciclos.
 *
 * @param atual apontador para a antena atual na travessia.
 * @param destino apontador para a antena de destino.
 * @param caminho Array de antenas que armazena o caminho atual.
 * @param index Índice atual no array de caminho.
 */

void  TodosCaminhos(Antena* atual, Antena* destino, Antena* caminho, int index) {
    if (!atual || atual->visitado) return;

    caminho[index++] = *atual;
    atual->visitado = 1;

    if (atual == destino) {
        for (int i = 0; i < index; i++) {
            printf("(%d,%d)", caminho[i].x, caminho[i].y);
            if (i < index - 1) printf(" -> ");
        }
        printf("\n");
    } else {
        Adjacencias* adj = atual->adj;
        while (adj) {
            TodosCaminhos(adj->destino, destino, caminho, index);
            adj = adj->proximo;
        }
    }

    atual->visitado = 0;
}

#pragma endregion 

#pragma region  Intersecoes

/**
 * @brief Procura e exibe interseções de coordenadas entre antenas de duas frequências diferentes.
 *
 * Esta função percorre todas as antenas do grafo e verifica se existem antenas
 * com frequências distintas (`f1` e `f2`) que ocupem as mesmas coordenadas (x, y).
 * Quando encontra interseções, imprime as coordenadas onde ocorrem.
 *
 * @param grafo apontador para o grafo onde as antenas estão armazenadas.
 * @param f1 Carácter que representa a primeira frequência a ser comparada.
 * @param f2 Carácter que representa a segunda frequência a ser comparada.
 */

void  IntersecoesFrequencias(grafo* grafo, char f1, char f2) {
    for (Antena* a = grafo->cabeca; a != NULL; a = a->proximo) {
        if (a->frequencia != f1) continue;
        for (Antena* b = grafo->cabeca; b != NULL; b = b->proximo) {
            if (b->frequencia != f2) continue;
            if (a->x == b->x && a->y == b->y) {
                printf("Intersecao em (%d, %d)\n", a->x, a->y);
            }
        }
    }
}
#pragma endregion 


#pragma region GRAFOS


/**
 * @brief Liberta toda a memória associada ao grafo e às suas antenas.
 *
 * Esta função percorre todas as antenas do grafo, libertando a memória
 * das estruturas de adjacência associadas e, por fim, as próprias antenas.
 * Também liberta o próprio apontador do grafo.
 *
 * @param g apontador para o grafo a ser libertado.
 * @return int Retorna 1 em caso de sucesso, ou -1 se o apontador do grafo for nulo.
 */
int LiberarGrafo(grafo* g) {
    if (!g) return -1;

    Antena* atual = g->cabeca;
    while (atual) {
        Antena* temp = atual;
        atual = atual->proximo;

        Adjacencias* adj = temp->adj;
        while (adj) {
            Adjacencias* adj_temp = adj;
            adj = adj->proximo;
            free(adj_temp);
        }

        free(temp);
    }
    free(g);
    return 1;
}

/**
 * @brief Cria e inicializa um novo grafo vazio.
 *
 * Aloca memória para a estrutura do grafo e inicializa o apontador
 * da cabeça da lista de antenas como NULL.
 *
 * @return grafo* apontador para o novo grafo criado ou NULL em caso de falha na alocação.
 */
grafo* CriarGrafo() {
    grafo* novo = (grafo*)malloc(sizeof(grafo));
    if (novo) {
        novo->cabeca = NULL;  // Inicializa a lista de antenas como vazia
    }
    return novo;
}



/**
 * @brief Lista todas as antenas e as suas conexões no grafo.
 *
 * Esta função percorre o grafo, imprimindo as coordenadas e frequências
 * de cada antena, bem como as antenas adjacentes (conectadas) a cada uma.
 * Caso o grafo esteja vazio, uma mensagem informativa será exibida.
 *
 * @param g apontador para a estrutura do grafo a ser listado.
 */

void  ListarGrafo(grafo* g) {
    if (g == NULL || g->cabeca == NULL) {
        printf("Grafo vazio!\n");
        return;
    }

    printf("\n===== LISTA DE ANTENAS E CONEXÕES =====\n");

    // Percorre todas as antenas no grafo
    for (Antena* atual = g->cabeca; atual != NULL; atual = atual->proximo) {
        printf("\nAntena (%d, %d) [Frequencia: %c]: ", atual->x, atual->y, atual->frequencia);

        // Percorre a lista de adjacencias da antena atual
        Adjacencias* adj = atual->adj;
        if (adj == NULL) {
            printf("sem conexoes.");
        } else {
            while (adj != NULL) {
                printf("-> (%d, %d) [Freq: %c] ", adj->destino->x, adj->destino->y, adj->destino->frequencia);
                adj = adj->proximo;
            }
        }
    }

    printf("\n\n========== FIM DA LISTA ==========\n");
}
#pragma endregion 


#pragma region MAIN
/**
 * @brief Função principal do programa de gestão de antenas e conexões.
 *
 * Esta função apresenta um menu interativo ao utilizador, permitindo-lhe:
 * - Carregar antenas a partir de um ficheiro;
 * - Criar conexões (adjacências) entre as antenas;
 * - Executar procurar em profundidade (DFS) e em largura (BFS);
 * - Identificar interseções entre antenas de frequências distintas;
 * - Visualizar todos os caminhos possíveis entre duas antenas;
 * - Criar antenas manualmente;
 * - Remover antenas existentes;
 * - Listar o grafo atual;
 * - Guardar o grafo em ficheiro binário ou como matriz num ficheiro de texto.
 *
 * O programa continua a executar até que o utilizador escolha sair.
 *
 * @return Retorna 0 no fim da execução.
 */
int main() {
    
    grafo grafo = {NULL};
    bool sucesso = false;
    char mensagem[100];
    char arquivo[256];
    int opcao;
    int total_antenas = 0;

    do {
        printf("\n--- Menu ---\n");
        printf("1. Carregar arquivo de antenas\n");
        printf("2. Criar adjacencias\n");
        printf("3. Executar DFS\n");
        printf("4. Executar BFS\n");
        printf("5. Encontrar intersecoes entre frequencias\n");
        printf("6. Mostrar todos os caminhos entre duas antenas\n");
        printf("7. criar antena manualmente\n");
        printf("8. Sair\n");
        printf("9. Remover antena\n");
        printf("10. Listar grafo\n");
        printf("11. Guardar grafo em arquivo binario\n");
        printf("12. guardar arquivo texto\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);

        
        switch (opcao) {
            case 1:
                printf("Informe o nome do arquivo: ");
                scanf("%s", arquivo);
                LiberarGrafo(&grafo); // Limpa grafo anterior, se houver
                if (CarregarArquivo(&grafo, arquivo, &sucesso, mensagem)) {
                    printf("%s\n", mensagem);
                    total_antenas = contarAntenas(grafo.cabeca);
                } else {
                    printf("Erro: %s\n", mensagem);
                }
                break;

            case 2:
                if (grafo.cabeca == NULL) {
                    printf("Carregue um arquivo primeiro.\n");
                } else {
                    CriarAdjacencias(&grafo);
                    printf("Adjacencias criadas.\n");
                }
                break;

            case 3: // DFS
                if (grafo.cabeca == NULL) {
                    printf("Carregue um arquivo primeiro.\n");
                } else {
                int x, y;
                printf("Informe as coordenadas de inicio (x y): ");
                    scanf("%d %d", &x, &y);
                            ExecutarDFS(&grafo, x, y);
                }
                break;

            case 4: // BFS
                if (grafo.cabeca == NULL) {
                    printf("Carregue um arquivo primeiro.\n");
                } else {
                    int x, y;
                    printf("Informe as coordenadas de inicio (x y): ");
                    scanf("%d %d", &x, &y);
                    ExecutarBFS(&grafo, x, y);
                }
                break;

            case 5:
                if (grafo.cabeca == NULL) {
                    printf("Carregue um arquivo primeiro.\n");
                } else {
                    char f1, f2;
                    printf("Informe a primeira frequencia: ");
                    scanf(" %c", &f1);
                    printf("Informe a segunda frequencia: ");
                    scanf(" %c", &f2);
                    IntersecoesFrequencias(&grafo, f1, f2);
                    printf("\n");
                }
                break;

            case 6:
                if (grafo.cabeca == NULL) {
                    printf("Carregue um arquivo primeiro.\n");
                } else {
                    int x1, y1, x2, y2;
                    printf("Informe as coordenadas da antena origem (x y): ");
                    scanf("%d %d", &x1, &y1);
                    printf("Informe as coordenadas da antena destino (x y): ");
                    scanf("%d %d", &x2, &y2);

                    Antena* origem = ProcurarAntenaPorCoordenada(grafo.cabeca, x1, y1);
                    Antena* destino = ProcurarAntenaPorCoordenada(grafo.cabeca, x2, y2);

                    if (!origem || !destino) {
                        printf("Antena origem ou destino nao encontrada.\n");
                    } else {
                        Antena* caminho = malloc(sizeof(Antena*) * total_antenas);
                        if (!caminho) {
                            printf("Erro de alocacao de memoria.\n");
                            break;
                        }
                        ResetVisitas(grafo.cabeca);
                        printf("Todos os caminhos entre (%d,%d) e (%d,%d):\n", x1, y1, x2, y2);
                        TodosCaminhos(origem, destino, caminho, 0);
                        free(caminho);
                    }
                }
                break;
            case 7: { 
                // Criar antena manualmente
                int x, y;
                char freq;
                int status;

                printf("Digite as coordenadas (x y) da nova antena: ");
                scanf("%d %d", &x, &y);

                printf("Digite a frequencia da antena (um caractere): ");
                scanf(" %c", &freq);

                Antena* nova = inserirAntenaManual(&grafo, x, y, freq, &status);

                if (status == 1) {
                    printf("Antena criada com sucesso em (%d, %d) com frequencia '%c'.\n", 
                        nova->x, nova->y, nova->frequencia);
                    total_antenas = contarAntenas(grafo.cabeca); // Atualiza contagem
                } else if (status == -1) {
                    printf("Erro: Já existe uma antena nas coordenadas (%d, %d).\n", x, y);
                } else if (status == -2) {
                    printf("Erro: Falha ao alocar memória para a antena.\n");
                } else {
                    printf("Erro desconhecido.\n");
                }
                break;
            }
            case 8:
                printf("Saindo...\n");
                LiberarGrafo(&grafo);
                break;
            case 9:
                if (grafo.cabeca == NULL) {
                    printf("Grafo vazio. Nada para remover.\n");
                } else {
                    int x, y;
                    bool sucesso;
                    printf("Informe as coordenadas da antena a remover (x y): ");
                    scanf("%d %d", &x, &y);
                    grafo.cabeca = removerAntena(grafo.cabeca, x, y, &sucesso);
                    if (sucesso) {
                        printf("Antena em (%d,%d) removida com sucesso.\n", x, y);
                        total_antenas = contarAntenas(grafo.cabeca);  // Atualiza contagem
                    } else {
                        printf("Antena em (%d,%d) nao encontrada.\n", x, y);
                    }
                }
                break;
            case 10:
                ListarGrafo(&grafo);
                break;
            case 11:
                if (grafo.cabeca == NULL) {
                    printf("Grafo vazio. Nada para salvar.\n");
                } else {
                    char nomeArquivo[256];
                    printf("Informe o nome do arquivo para salvar (ex: grafo.bin): ");
                    scanf("%s", nomeArquivo);
        
                    if (SaveGrafoBin(nomeArquivo, &grafo)) {
                        printf("Grafo salvo com sucesso no arquivo %s\n", nomeArquivo);
                    } else {
                    printf("Erro ao salvar o grafo no arquivo %s\n", nomeArquivo);
                    }
                }
                break;
            case 12: {
                int resultado;
                if (grafo.cabeca == NULL) {
                    printf("Erro: Nao ha antenas para salvar (grafo vazio)\n");
                    break;
                    } 
                    printf("Informe o nome do arquivo para salvar: ");
                    scanf("%s", arquivo);
                    resultado = SaveGrafoMatriz(arquivo, &grafo);
                    if (resultado == 1) {
                        printf("Matriz salva com sucesso!\n");
                    } else {
                        printf("Erro ao salvar matriz (Código: %d)\n", resultado);
                    }
                    break;
                
                            
                }
            default:
                printf("Opcao invalida.\n");
        }
    } while (opcao != 8);

    return 0;
}

#pragma endregion 