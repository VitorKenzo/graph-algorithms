/*Vitor Kenzo F. Pellegatti RA:771066 05/09/2022*/
#include <stdio.h>
#include <stdlib.h>

//maior custo possivel de uma aresta eh 10^4 pela descricao do problema
#define INF 10001

//grafo da atividade de EDA https://github.com/lizier/atividade08-VitorKenzo
//----------------------implementacao do grafo por lista de adjacencia-------------------//
struct node { 
	 int n; //vizinho
	 int w; //peso
	 struct node *next; 
};
typedef struct node Node;

struct graph {
	 int V; 
	 int A; 
	 Node **adj; 
};
typedef struct graph Graph;

void inicializar_grafo(Graph *G, unsigned nvertices){
	 G->V = nvertices;
	 G->A = 0;
	 //alocando uma lista de listas do tamanho do numero de vertices do grafo
	 G->adj = (Node**)malloc(sizeof(Node*) * G->V);
	 //inicializando todas as listas como nulas
	 //uma vez que o grafo inicia sem nenhum arco
	 for(unsigned v = 0; v < G->V; v++){
			G->adj[v] = NULL;
	 }
}

void destroi_grafo(Graph *G){
	 //para cada vertice do grafo
	 for(unsigned v = 0; v < G->V; v ++){
			//iremos desalocar todos os nos das listas
			while(G->adj[v]){
				 Node* l = G->adj[v];
				 G->adj[v] = G->adj[v]->next;
				 free(l);
			}
	 }
	 //desalocando a lista de listas
	 free(G->adj);
	 //resetando o grafo
	 G->V = G->A = 0;
}

void adiciona_aresta(Graph *G, unsigned u, unsigned v, unsigned w){
		
	//adicionando a direcao u->v
	//aloca-se um novo no
	Node* lv = (Node*)malloc(sizeof(Node));
	//coloca no elemento o vizinho respectivo
	lv->n = u;
	//coloca no elemento o peso respectivo
	lv->w = w;
	//coloca na lista de adjacencia de v
	lv->next = G->adj[v];
	G->adj[v] = lv;

	//adicionando a entrada de v->u
	//aloca-se um novo no
	Node* lu = (Node*)malloc(sizeof(Node));
	//coloca no elemento o vizinho respectivo
	lu->n = v;
	//coloca no elemento o peso respectivo
	lu->w = w;
	//coloca na lista de adjacencia de v
	lu->next = G->adj[u];
	G->adj[u] = lu;

	//aumenta o numero de arestas do grafo
	G->A++;

}

//-------------------------------------------------------------------------------------//

int main(){

    //entradas do problema
    int n, m, k;

    //ENTRADA
    scanf("%d %d %d", &n, &m, &k);

    //alocando o grafo
    //vamos alocar o grafo com n+1 para conseguir resolver o problema
    //nosso no criado sera o no n que sera conectado a todas as estufas
    //as estufas sao de 0 ate k-1
	Graph *G = (Graph*)malloc(sizeof(Graph));
	inicializar_grafo(G,n+1);

    //ENTRADA DAS ARESTAS
	int u, v, w;
	for(int i = 0; i < m; i ++){
		scanf("%d %d %d", &u, &v, &w);
		adiciona_aresta(G,u,v,w);
	}

    //adicionando o no falso que sera a raiz do dijkstra
    for(int i = 0; i < k; i ++){
        adiciona_aresta(G,n,i,0);
    }

    //variaveis para o algoritmo de Dijkstra
	int dist[n+1];
	int S[n+1];
	int pred[n+1];

    //inicializacao das variaveis para o Dijkstra
    //no inicial falso sera o primeiro
    dist[n] = 0;
    for(int i = 0; i < n + 1; i ++){
		//o conjunto S indica vertices a incluir na arvore, sendo 1 aqueles que ainda precisam ser incluidos e 0 ja incluidos
		S[i] = 1;
		pred[i] = -1;
		if(i != n){
			dist[i] = INF;
		}
	}

    //Algoritmo de Dijkstra
    //Como estamos apenas usando um vetor simples para o peso, nosas complexidade aqui como foi discutida em aula eh de O(n^2)
	int to_be_included = n+1;
	int current_n = 0;
    int current_w = INF;
	Node* it_adj;
	while(to_be_included != 0){

        current_w = INF;

		//encontrando o menor peso
		for(int i = 0; i < n + 1; i ++){
			if(S[i] == 1 && dist[i] < current_w){
				current_n = i;
                current_w = dist[i];
			}
		}
		//retirando de Q o vertice com maior peso
		S[current_n] = 0;

		//atualizando peso dos vertices vizinhos caso tenham arestas com pesos maiores sendo encontradas
		it_adj = G->adj[current_n];
		while(it_adj != NULL){
			
			if(S[it_adj->n] == 1 && dist[it_adj->n] > (dist[current_n] +  it_adj->w)){

				dist[it_adj->n] = dist[current_n] +  it_adj->w;
				pred[it_adj->n] = current_n;

            //caso especial onde a distancia eh a mesma e temos que pegar o no de menor numero
			}else if(S[it_adj->n] == 1 && dist[it_adj->n] == (dist[current_n] +  it_adj->w)){

                //encontrando a estufa mais proxima caso ela nao seja uma estufa
                int estufa_temp = current_n;
                while(estufa_temp >= k){
                    estufa_temp = pred[estufa_temp];
                }

                //sempre pegando a estufa de menor numero possivel
                if(estufa_temp < pred[it_adj->n]){
                    //printf("entro! %d\n",estufa_temp);
                    pred[it_adj->n] = estufa_temp;
                }
            }

			it_adj = it_adj->next;
		}

		to_be_included --;
	}

    //o vetor de resposta possui um tamanho igual a quantidade de centros
    int res[n - k];
    int j = 0;
    int qnt_centros = 0;
    for(int i = k; i < n; i ++){
        res[j] = pred[i]; 
        if(res[j] < k){
            qnt_centros ++;
        }
        j ++;
    }
    
    //depois de colocar todos os predecessores no vetor resposta precisamos checar se temos estufas em todos os lugares
    int temp;
    while(qnt_centros < n - k ){
        for(int i = 0; i < n - k; i ++){
            if(res[i] >= k){
                temp = res[i];
                res[i] = pred[temp];
                if(res[i] < k){
                    qnt_centros ++;
                }
            }
        }
    }

    //SAIDA
    for(int i = 0; i < n - k - 1; i ++){
        printf("%d ",res[i]);
    }
    printf("%d\n", res[n - k - 1]);

    //desalocando a memoria dinamicado grafo e sua referencia
	destroi_grafo(G);
	free(G);

    return 0;
}