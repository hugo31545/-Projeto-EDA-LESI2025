/**
 * @file trabalho.h
 * @brief Ficheiro de cabeçalho com estruturas e funções para manipulação de grafos de antenas
 * @author Hugo Gomes
 * @version 1.0
 * @date 2025-05-18
 * Este ficheiro define estruturas para antenas, listas de adjacências e grafos, 
 * bem como várias funções para criar, manipular e analisar redes de antenas.
 */

#ifndef TRABALHO_H
#define TRABALHO_H

#include <stdio.h>
 #include <stdlib.h>
 #include <string.h>
 #include <stdbool.h>
 #include <time.h>
 #include <stddef.h>
  
 
 #ifdef _MSC_VER
 typedef long ssize_t; 
 #endif

/**
 * @def CHAR_VAZIO
 * @brief Constante que representa um carácter vazio na matriz
 */
#define CHAR_VAZIO '.'

/**
 * @struct Adjacencias
 * @brief Estrutura que representa adjacências entre antenas da mesma frequência
 */
// Estrutura de adjacencia entre antenas de mesma frequencia
typedef struct Adjacencias {
    struct Antena* destino;       /**< apontador para a antena de destino */
    struct Adjacencias* proximo;  /**< apontador para a próxima adjacência na lista */
} Adjacencias;

/**
 * @struct Antena
 * @brief Estrutura que representa uma antena
 */

 typedef struct Antena {
    int x;                  /**< Coordenada x (linha) */
    int y;                  /**< Coordenada y (coluna) */
    char frequencia;        /**< Frequência da antena */
    int visitado;          /**< Flag de visita (0 = não visitado, 1 = visitado) */
    struct Antena* proximo; /**< apontador para a próxima antena na lista ligada */
    Adjacencias* adj;       /**< Lista de adjacências */
} Antena;

/**
 * @struct LinhaArquivo
 * @brief Estrutura para leitura de linhas de ficheiro (lista ligada)
 */
typedef struct LinhaArquivo {
    char* conteudo;             /**< Conteúdo da linha */
    struct LinhaArquivo* proximo; /**< apontador para a próxima linha */
} LinhaArquivo;

/**
 * @struct grafo
 * @brief Estrutura do grafo contendo a lista de antenas
 */
typedef struct {
    Antena* cabeca; /**< apontador para a cabeça da lista de antenas */
} grafo;

/**
 * @struct AntenaBin
 * @brief Estrutura auxiliar para binário
 */
typedef struct {
    int x;           /**< Coordenada x */
    int y;           /**< Coordenada y */
    char frequencia; /**< Frequência */
} AntenaBin;


/* FUNÇÕES BÁSICAS DE ANTENA */

/**
 * @brief Cria uma nova antena
 * @param x Coordenada x
 * @param y Coordenada y
 * @param frequencia Carácter de frequência
 * @return apontador para a nova antena criada
 */
Antena* CriarAntena(int x, int y, char frequencia);

/**
 * @brief Procura uma antena pelas suas coordenadas
 * @param cabeca Cabeça da lista de antenas
 * @param x Coordenada x a procurar
 * @param y Coordenada y a procurar
 * @return apontador para a antena encontrada ou NULL se não existir
 */
Antena* ProcurarAntenaPorCoordenada(Antena* cabeca, int x, int y);

/**
 * @brief Insere uma antena por ordem (por coordenadas)
 * @param cabeca Cabeça da lista de antenas
 * @param nova_Antena Nova antena a inserir
 * @return Cabeça atualizada da lista de antenas
 */
Antena* InsereAntenaOrdenada(Antena* cabeca, Antena* nova_Antena);

/**
 * @brief Insere manualmente uma antena no grafo
 * @param g apontador para o grafo
 * @param x Coordenada x
 * @param y Coordenada y
 * @param freq Carácter de frequência
 * @param resultado Flag de resultado (saída)
 * @return apontador para a antena inserida
 */
Antena* inserirAntenaManual(grafo* g, int x, int y, char freq, int* resultado);

/**
 * @brief Remove uma antena do grafo
 * @param cabeca Cabeça da lista de antenas
 * @param linha Coordenada x da antena a remover
 * @param coluna Coordenada y da antena a remover
 * @param sucesso Flag de sucesso (saída)
 * @return Cabeça atualizada da lista de antenas
 */
Antena* removerAntena(Antena* cabeca, int linha, int coluna, bool* sucesso);

/**
 * @brief Conta o número de antenas no grafo
 * @param cabeca Cabeça da lista de antenas
 * @return Número de antenas
 */
int contarAntenas(Antena* cabeca);

/* FUNÇÕES DE ADJACÊNCIA */

/**
 * @brief Cria uma nova adjacência
 * @param destino Antena de destino
 * @return apontador para a nova adjacência
 */
Adjacencias* CriaAdjacencia(Antena* destino);

/**
 * @brief Adiciona adjacências entre duas antenas
 * @param antena Antena fonte
 * @param vizinho Antena vizinha a adicionar
 * @return 1 se bem-sucedido, 0 caso contrário
 */
int AddAdjacencias(Antena* antena, Antena* vizinho);

/**
 * @brief Cria adjacências para todas as antenas no grafo
 * @param grafo apontador para o grafo
 */
void CriarAdjacencias(grafo* grafo);

/* FUNÇÕES DE GRAFO */

/**
 * @brief Cria um novo grafo
 * @return apontador para o novo grafo criado
 */
grafo* CriarGrafo();

/**
 * @brief Liberta toda a memória usada pelo grafo
 * @param g apontador para o grafo
 * @return 0 se bem-sucedido
 */
int LiberarGrafo(grafo* g);

/**
 * @brief Lista todas as antenas no grafo
 * @param g apontador para o grafo
 */
void ListarGrafo(grafo* g);


/* FUNÇÕES DE MANIPULAÇÃO DE FICHEIROS */

/**
 * @brief Carrega um grafo a partir de ficheiro
 * @param grafo apontador para o grafo a preencher
 * @param filename Nome do ficheiro a carregar
 * @param success Flag de sucesso (saída)
 * @param message Mensagem de erro (saída)
 * @return 0 se bem-sucedido, código de erro caso contrário
 */
int CarregarArquivo(grafo* grafo, const char* filename, bool* success, char* message);

/**
 * @brief Guarda o grafo num ficheiro binário
 * @param fileName Nome do ficheiro de saída
 * @param graph Grafo a guardar
 * @return true se bem-sucedido, false caso contrário
 */
bool SaveGrafoBin(const char* fileName, grafo* graph);

/**
 * @brief Guarda o grafo num ficheiro de matriz
 * @param fileName Nome do ficheiro de saída
 * @param graph Grafo a guardar
 * @return 0 se bem-sucedido, código de erro caso contrário
 */
int SaveGrafoMatriz(const char* fileName, grafo* graph);

/* FUNÇÕES DE PROCURA/ALGORITMOS */

/**
 * @brief Reinicia as flags de visita para todas as antenas
 * @param cabeca Cabeça da lista de antenas
 */
void ResetVisitas(Antena* cabeca);

/**
 * @brief Executa pesquisa em profundidade recursiva
 * @param antena Antena inicial
 * @return true se a pesquisa completar, false caso contrário
 */
bool DfsRecursiva(Antena* antena);

/**
 * @brief Função utilitária para DFS 
 * @param g grafo 
 * @param X coordenada 
 * @param Y coordenada
 */
bool  ExecutarDFS(grafo* g, int x, int y);

/**
 * @brief Função utilitária para BFS recursiva
 * @param fila Fila de antenas
 * @param inicio Índice de início da fila
 * @param fim Índice de fim da fila
 */
void BfsRecursivaUtil(Antena* fila, int inicio, int fim);

/**
 * @brief Executa pesquisa em largura recursiva
 * @param origem Antena inicial
 * @param total_antenas Número total de antenas
 */
bool  BfsRecursiva(Antena* origem, int total_antenas);
/**
 * @brief Função utilitária para BFS 
 * @param g grafo 
 * @param X coordenada 
 * @param Y coordenada
 */
void  ExecutarBFS(grafo* g, int x, int y);

/**
 * @brief Encontra todos os caminhos entre duas antenas
 * @param atual Antena atual no caminho
 * @param destino Antena de destino
 * @param caminho Caminho atual a ser explorado
 * @param index Índice atual no caminho
 */
void TodosCaminhos(Antena* atual, Antena* destino, Antena* caminho, int index);

/**
 * @brief Encontra interseções entre duas frequências
 * @param grafo apontador para o grafo
 * @param f1 Primeira frequência
 * @param f2 Segunda frequência
 */
void IntersecoesFrequencias(grafo* grafo, char f1, char f2);

/* FUNÇÕES AUXILIARES */

/**
 * @brief Liberta a memória usada pela lista de linhas de ficheiro
 * @param lista Cabeça da lista de linhas
 */
void LibertarLinhasArquivo(LinhaArquivo* lista);

#endif
