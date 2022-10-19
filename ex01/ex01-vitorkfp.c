/*Vitor Kenzo F. Pellegatti RA:771066 04/08/2022*/
#include <stdio.h>
#include <stdlib.h>

#define MAX_NODE_SIZE 600
#define MAX_EDGE_SIZE 10000
#define INF 2000000000

/**********************API-da-lista*************************/
//vamos usar para representar as listas de adjacencias
struct node{
	int item;
	struct node *next;
};

typedef struct node Node;

struct lista{
    Node* cabeca;
};
typedef struct lista Lista;

void init(Lista* lista){
	lista->cabeca = NULL;
}

int isEmpty(Lista lista){
	return(lista.cabeca == NULL);
}

void insert(Lista* lista, int n){
	if(isEmpty((*lista))){
		Node* novo = (Node*)malloc(sizeof(Node));
		novo->item = n;
		novo->next = NULL;
		lista->cabeca = novo;
	}else{
		Node* novo = (Node*)malloc(sizeof(Node));
		novo->item = n;
		novo->next = lista->cabeca;
		lista->cabeca = novo;
	}
}

void destroy(Lista* lista){
	while(lista->cabeca != NULL){
		Node* rem = lista->cabeca;
		lista->cabeca = lista->cabeca->next;
		free(rem);
	}
}

/**********************API-da-fila**************************/
/*!
 * Tipo do elemento armazenado na lista 
 */

typedef struct {
	int v[MAX_NODE_SIZE];
	int front, back, size;
} queue;

void initialize(queue *q){
	q->back = q->front = q->size = 0;
}

int empty(queue *q){
	return (q->size == 0);
}

int full(queue *q){
	return(q->size == MAX_NODE_SIZE);
}

void push(queue *q, int data){
	if(!full(q)){
		q->v[q->back] = data;
		q->back ++;
		q->back %= MAX_NODE_SIZE;
		q->size ++;
	}
}
int pop(queue *q){
	if(!empty(q)){
		int item = q->v[q->front];
		q->front ++;
		q->front %= MAX_NODE_SIZE;
		q->size --;
		return item;
	}
	//erro
	return -1;
}
unsigned size(queue *q){
	return q->size;
}
/*****************************Funcoes-auxiliares***************************/

//BUSCA EM LARGURA
void BFS(Lista* vizinhos, int raiz, int qnt_vertices, int* pred, int* dist, int* cor){
	//inicializando fila vazia
	queue q;
	initialize(&q);

	for(int i = 1; i < qnt_vertices; i ++){
		dist[i] = INF;
		//branco
		cor[i] = 0;
		//predecessor nulo
		pred[i] = -1;
	}

	dist[raiz] = 0;
	//cinza
	cor[raiz] = 1;
	//predecessor nulo
	pred[raiz] = -1;
	
	push(&q,raiz);

	int u;
	while(!empty(&q)){
		u = pop(&q);
		for(Node* i = vizinhos[u].cabeca; i != NULL; i = i->next){
			if(cor[i->item] == 0){
				dist[i->item] = dist[u] + 1;
				pred[i->item] = u;
				//cinza
				cor[i->item] = 1;
				push(&q,i->item);
			}
		}
		//preto
		cor[u] = 2;
	}
}
/**********************************************************************************/

int main(){

	//entrada
	int n, m;
	scanf("%d %d", &n, &m);

	//criando e inicializando a lista de adjacencias
	Lista adjacencias[n];
	for(int i = 0; i < n; i ++){
		init(&adjacencias[i]);
	}

	//populando a lista de adjacencias
	int u, v;
	for(int i = 0; i < m; i ++){
		scanf("%d %d", &u, &v);
		//nao tem orientacao no grafo desse exercicio
		insert(&adjacencias[u],v);
		insert(&adjacencias[v],u);
	}

	//criando vetores auxiliares para a BFS
	int pred[n];
	int dist[n];
	/*Branco = 0; Cinza = 1; preto = 2*/
	int cor[n];

	//sempre comecamos a busca pelo 0 que eh o Erdos
	BFS(adjacencias,0,n,pred,dist,cor);

	//depois da busca vamos diretamente checar o vetor de distancias
	int max = 0;
	for(int i = 0; i < n; i ++){
		if(dist[i] > max){
			max = dist[i];
		}
	}

	//resposta
	if(max == INF){
		printf("infinito\n");
	}else{
		printf("%d\n",max);
	}

	/*PRINTS DE DEBBUG 
	printf("IMPRIMINDO LISTA DE ADJACENCIAS\n");
	for(int i = 0; i < n; i ++){
		printf("%d:",i);
		for(Node* j = adjacencias[i].cabeca; j != NULL; j = j->next){
			printf(" %d ->",j->item);
		}
		printf("\n");
	}

	for(int i = 0; i < n; i ++){
		printf("%d ",pred[i]);
	}
	printf("\n");
	for(int i = 0; i < n; i ++){
		printf("%d ",dist[i]);
	}
	printf("\n");
	for(int i = 0; i < n; i ++){
		printf("%d ",cor[i]);
	}
	printf("\n");*/

	//desalocando as listas de adjacencias
	for(int i = 0; i < n; i ++){
		destroy(&adjacencias[i]);
	}

	return 0;
}

