/*Vitor Kenzo F. Pellegatti RA:771066 01/10/2022*/
#include <stdio.h>
#include <stdlib.h>

#define MAX_NODE_SIZE 500
#define MAX_EDGE_SIZE 10000
#define INF 2000000000

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
void BFS(int qnt_vertices, int vizinhos[qnt_vertices][qnt_vertices], int raiz, int* pred, int* dist, int* cor){
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
		for(int i = 0; i < qnt_vertices; i ++){
			//fazendo com que a busca em largura nao siga caminhos indevidos com peso zerado ou inexistentes
			if(vizinhos[u][i] < 1){
				continue;
			}
			if(cor[i] == 0){
				dist[i] = dist[u] + 1;
				pred[i] = u;
				//cinza
				cor[i] = 1;
				push(&q,i);
			}
		}
		//preto
		cor[u] = 2;
	}
}

/**********************************************************************************/

int main(){

	//ENTRADA
	int n, m;
	scanf("%d %d", &n, &m);

	//vamos usar uma matriz de adjacencias para facilitar a mudanca dos pesos
	//das arestas na rede residual. Considera-se que o arco nao existe quano eh -1
	int adjacencias[n][n];
	for(int i = 0; i < n; i ++){
        for(int j = 0; j < n; j ++){
            adjacencias[i][j] = -1;
        }
    }

	int u, v, w;
	for(int i = 0; i < m; i ++){
		scanf("%d %d %d", &u, &v, &w);
		//Criando a rede residual
		if(adjacencias[u][v] == -1){
			adjacencias[u][v] = w;
			adjacencias[v][u] = 0;
		}else{
			adjacencias[u][v] += w;
		}
	}
	
	//criando vetores auxiliares para a BFS
	int pred[n];
	int dist[n];
	/*Branco = 0; Cinza = 1; preto = 2*/
	int cor[n];

	//garantindo que a condicao do while seja valida para executar pelo menos uma vez
	pred[n-1] = 0;

	//variaveis auxiliares
	int path[n];
	int min;
	int min_valor = 0;

	while(pred[n-1] != -1){
		min = INF;
		//sabemos que 0 eh s e n-1 eh t
		BFS(n,adjacencias,0,pred,dist,cor);

		//significa que nao existe mais como aumentar o fluxo
		if(pred[n-1] == -1){
			break;
		}

		//vamos encontrar o caminho
		int j = 0;
		if(pred[n-1] != -1){
			path[j] = n-1;
			j ++;	
			int i = pred[n-1];
			while(i != -1){
				path[j] = i;
				i = pred[i]; 
				j ++;
			}	
		}

		//encontrando o arco com valor minimo do caminho para passar como fluxo
		for(int i = 0; i < dist[n-1]; i ++){
			if(adjacencias[path[i+1]][path[i]] < min)
				min = adjacencias[path[i+1]][path[i]];
		}

		//somatoria para o valor final
		min_valor += min;

		//reajustando a rede residual com o fluxo novo
		for(int i = 0; i < dist[n-1]; i ++){
			adjacencias[path[i+1]][path[i]] -= min;
			adjacencias[path[i]][path[i+1]] += min;
		}
	}

	//SAIDA
	printf("%d\n", min_valor);
	
	return 0;
}
