/*Vitor Kenzo F. Pellegatti RA:771066 09/08/2022*/
#include <stdio.h>
#include <stdlib.h>

#define MAX_NODE_SIZE 500
#define MAX_EDGE_SIZE 1000
#define INF 2000000000

/*
    n -> numero de vertices do grafo
    v -> vertice v descoberto que sera prontamente explorado
    tempo -> variavel tempo do algoritmo
    adjacencias[][] -> copia da matriz de adjacencias
    cor[] -> vetor de cor do algoritmo
    pred[] -> vetor pred do algoritmo
    d[] -> vetor de descoberta do algoritmo
    f[] -> vetor de termino de exploracao do algoritmo
    ordem[] -> tera a ordenacao topologica da busca
    ord_i -> indice para o vetor ordem
*/
void DFS_aux(int n, int v, int *tempo, int adjacencias[n][n], int * cor, int *pred, int *d, int *f, int *ordem, int *ord_i){
    
    cor[v] = 1;
    d[v] = (*tempo);
    (*tempo) ++;

    for(int i = 0; i < n; i ++){
        if(adjacencias[v][i] == 1){
            if(cor[i] == 0){
                pred[i] = v;
                DFS_aux(n, i, tempo, adjacencias, cor, pred, d, f, ordem, ord_i);
            }
        }
    }

    //preto
    cor[v] = 2;
    f[v] = (*tempo);
    (*tempo) ++;

    //preenchendo a ordem topologica
    ordem[(*ord_i)] = v;
    (*ord_i) --;

}

/*
    n -> numero de vertices do grafo
    adjacencias[][] -> copia da matriz de adjacencias
    cor[] -> vetor de cor do algoritmo
    pred[] -> vetor pred do algoritmo
    d[] -> vetor de descoberta do algoritmo
    f[] -> vetor de termino de exploracao do algoritmo
    raiz -> raiz da primeira busca
    ordem[] -> tera a ordenacao topologica da busca
*/
void DFS(int n, int adjacencias[n][n], int *cor, int *pred, int *d, int *f, int raiz, int *ordem){
    
    for(int i = 0; i < n; i ++){
        cor[i] = 0;
        pred[i] = -1;
    }

    int tempo = 0;
    int ord_i = n-1;

    int j = raiz;
    for(int i = 0; i < n; i ++){
        if(cor[j] == 0){
            DFS_aux(n, j, &tempo, adjacencias, cor, pred, d, f, ordem, &ord_i);
            
            j ++;

            if(j == n){
                j = 0;
            }
        }
    }
}


int main(){

	//entrada
	int n, m;
	scanf("%d %d", &n, &m);

    //para este exercicio vamos usar uma matriz de adjacencias
    //principalmente para ter o acesso aleatorio
    //vamos manter o grau de entrada em um vetor para podermos escolher a fonte posteriormente
    int adjacencias[n][n];
    int grau_entrada[n];
    for(int i = 0; i < n; i ++){
        for(int j = 0; j < n; j ++){
            adjacencias[i][j] = 0;
        }
        grau_entrada[i] = 0;
    }

    int u, v;
    for(int i = 0; i < m; i ++){
        scanf("%d %d", &u, &v);
        //vamos preencher tal que o arco eh u -> v
        adjacencias[u][v] = 1;
        //aumentando o grau de entrada de v
        grau_entrada[v] ++;
    }

    //temos a certeza de que pelo menos uma fonte o grafo tem
    int raiz = 0;
    int num_raiz = 0;
    for(int i = 0; i < n; i ++){
        if(grau_entrada[i] == 0){
            raiz = i;
            num_raiz ++;
        }
    }

    //variaveis para descobrir se o caminho eh hamiltoniano
    int hamiltoniano = 0;
    int ordem[n];

    //caso tenha mais de uma raiz, a ordenacao topologica ja pode ser diferente logo de comeco
    //portanto ela nao eh unica logo nao hamiltoniana
    if(num_raiz > 1){
        printf("Nao possui.\n");
    }else{
        //variaveis auxiliares para a busca
        int cor[n];
        int pred[n];
        int d[n];
        int f[n];

        //fazendo uma busca em profundidade com O(n^2) ja que estamos usando matriz de adjacencia
        DFS(n, adjacencias, cor, pred, d, f, raiz, ordem);

        hamiltoniano = 1;
        //vamos checar se existe um arco direto de cada vertice na ordem topologica encontrada
        //sendo esse o caso, tem-se o caminho hamiltoniano, caso contrario, podemos trocar vertices na ordem
        //topologica fazendo o caminho deixar de ser hamiltoniano  
        for(int i = 0; i < n-1; i ++){
            if(adjacencias[(ordem[i])][(ordem[i+1])] == 0){
                hamiltoniano = 0;
            }
        }

        //saida
        if(hamiltoniano){
            for(int i = 0; i < n; i ++){
                printf("%d ",ordem[i]);
            }
            printf("\n");
        }else{
            printf("Nao possui.\n");
        }
    }

    return 0;

}
