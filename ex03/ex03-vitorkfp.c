/*Vitor Kenzo F. Pellegatti RA:771066 04/09/2022*/
#include <stdio.h>
#include <stdlib.h>

//aqui infinito pode ser -1, porque estamos procurando uma arvore geradora maxima e nao minima
// e funciona para a inicializacao do Prim uma vez que 0 sera o maior valor presente
#define INF -1

//grafo da atividade de EDA https://github.com/lizier/atividade08-VitorKenzo
//(obrigado vitor do passado, podia ter achado antes vitor do passado presente...) com pequenas alteracoes para arestas com peso
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

//para representar o resultado
struct aresta{
	int u;
	int v;
};
typedef struct aresta Aresta;

//-------------------------------------------------------------------------------------//

int main() {

	//grafo com n vertices e m arestas
	int n, m;

	//ENTRADA
	scanf("%d %d", &n, &m);

	//alocando o grafo
	Graph *G = (Graph*)malloc(sizeof(Graph));
	inicializar_grafo(G,n);

	//ENTRADA DAS ARESTAS
	int u, v, w;
	for(int i = 0; i < m; i ++){
		scanf("%d %d %d", &u, &v, &w);
		adiciona_aresta(G,u,v,w);
	}

	//variaveis para o algoritmo de Prim
	int peso[n];
	int Q[n];
	int pred[n];
	Aresta res[n-1];

	//inicializando as variaveis
	//No nosso caso, a raiz do algoritmo de Prim sempre sera o vertice 0
	//peso[r] = 0
	peso[0] = 0;
	for(int i = 0; i < n; i ++){
		//o conjunto Q indica vertices a incluir na arvore, sendo 1 aqueles que ainda precisam ser incluidos e 0 ja incluidos
		Q[i] = 1;
		pred[i] = -1;
		if(i != 0){
			peso[i] = INF;
		}
	}

	//Algoritmo de Prim 
	//Como estamos apenas usando um vetor simples para o peso, nosas complexidade aqui como foi discutida em aula eh de O(n^2)
	int to_be_included = n;
	int res_i = 0;
	int current_n = 0;
	Node* it_adj;
	while(to_be_included != 0){

		current_n = 0;

		//encontrando o menor peso
		for(int i = 0; i < n; i ++){
			if(Q[i] == 1 && peso[i] > peso[current_n]){
				current_n = i;
			}
		}
		//retirando de Q o vertice com maior peso
		Q[current_n] = 0;

		//incluindo no vetor de resposta na ordem lexicografica certa
		if(to_be_included != n){
			if(current_n < pred[current_n]){
				res[res_i].u = current_n;
				res[res_i].v = pred[current_n];
			}else{
				res[res_i].u = pred[current_n];
				res[res_i].v = current_n;
			}
			res_i ++;
		}

		//atualizando peso dos vertices vizinhos caso tenham arestas com pesos maiores sendo encontradas
		it_adj = G->adj[current_n];
		while(it_adj != NULL){
			
			if(Q[it_adj->n] == 1 && peso[it_adj->n] < it_adj->w){
				peso[it_adj->n] = it_adj->w;
				pred[it_adj->n] = current_n;
			}

			it_adj = it_adj->next;
		}

		to_be_included --;
	}


	//Ordenando o resultado de maneira lexicografica
	//estamos usando um bubble sort simples, ja que a complexidade do proprio Prim nesse caso tambem esta de O(n^2)
	//desculpa professora, sei que nao eh a solucao mais elegante e minima mas ao mesmo tempo nao afeta a complexidade
	Aresta temp;
	for(int i = 0; i < n-1; i ++){
		for(int j = 0; j < n - i - 2; j ++){
			if(res[j].u > res[j+1].u){
				temp = res[j];
				res[j] = res[j+1];
				res[j+1] = temp;
			}else if(res[j].u == res[j+1].u && res[j].v > res[j+1].v){
				temp = res[j];
				res[j] = res[j+1];
				res[j+1] = temp;
			}
		}
	}

	//SAIDA
	for(int i = 0; i < n-1; i ++){
		printf("%d %d\n",res[i].u, res[i].v);
	}

	//desalocando a memoria dinamicado grafo e sua referencia
	destroi_grafo(G);
	free(G);

	return 0;
}
