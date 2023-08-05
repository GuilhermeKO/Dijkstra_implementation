#include <stdio.h>
#include <stdlib.h>

struct GRAFO{
struct VERTICE *head;
};

struct VERTICE{
struct VERTICE *next;
struct ARESTA *aresta;
int chave;
};

struct ARESTA{
struct ARESTA *next;
struct ARESTA *tail;
struct VERTICE *adj;
int peso;
};

struct NODE{	
int conteudo;
int prioridade;
struct NODE *next;
struct NODE *prev;
};

struct FILA{
struct NODE *head;
struct NODE *tail;
};

typedef struct ARESTA ARESTA;
typedef struct VERTICE VERTICE;
typedef struct GRAFO GRAFO;
typedef struct NODE NODE;
typedef struct FILA FILA;


GRAFO *cria_grafo();
VERTICE *busca_vertice(GRAFO *g, int chave);
void cria_vertice(GRAFO *g, int chave);
void cria_aresta(GRAFO *g, int chave1, int chave2, int peso);
void imprimi_grafo(GRAFO *g);
void djikstra(GRAFO *g, FILA *f, int vertice_atual, int v, int *d, int *p, int cont);

FILA *cria_fila();
NODE *busca_fila(FILA *f, int num, int prioridade);
void enqueue(FILA *f, int num, int peso);
void dequeue(FILA *f);
void imprimi_fila(FILA *f);
void imprimi_estruturas(int v, int *d, int *p);

#define inf 1000000;

int main(){

	GRAFO *g = cria_grafo();

	int vertice, aresta, vertice_inicio, a, b, c;
	scanf("%d %d %d", &vertice, &aresta, &vertice_inicio);


	for(int i = 1; i < vertice+1; ++i)
		cria_vertice(g, i);

	while(scanf("%d %d %d", &a, &b, &c) != EOF)
		cria_aresta(g, a, b, c);


	int d[vertice+1];
	int p[vertice+1];

	for(int i = 1; i < vertice+1; ++i){
		d[i] = inf;
		p[i] = -1;
	}

	FILA *Q = cria_fila();

	enqueue(Q, vertice_inicio, 0);
	d[vertice_inicio] = 0;
	p[vertice_inicio] = vertice_inicio;


	//imprimi_grafo(g);
	printf("ITERAÇÃO 0:\n");
	imprimi_estruturas(vertice, d, p);
    printf("\n");
    imprimi_fila(Q);
    printf("\n");

	djikstra(g, Q, vertice_inicio, vertice, d, p, 0);

	
	return 0;
	
}

GRAFO *cria_grafo(){
	GRAFO *new_graph = (GRAFO*) malloc(sizeof(GRAFO));
	new_graph->head = NULL;

	return new_graph;
}

VERTICE *busca_vertice(GRAFO *g, int chave){
	VERTICE *aux = g->head;
	
	while(aux != NULL && aux->chave != chave)
		aux = aux->next;
	
	return aux;
}

void cria_vertice(GRAFO *g, int chave){
	if(g == NULL)
		return;

	VERTICE *new_vertice = (VERTICE*) malloc(sizeof(VERTICE));
	new_vertice->chave = chave;
	new_vertice->next = g->head;

	g->head = new_vertice;

	return;
}

void cria_aresta(GRAFO *g, int chave1, int chave2, int peso){
	if(g == NULL)
		return;

	VERTICE *v1 = busca_vertice(g, chave1);
	if(v1 == NULL)
		return;

	VERTICE *v2 = busca_vertice(g, chave2);
	if(v2 == NULL)
		return;

	ARESTA *new_aresta  = (ARESTA*) malloc(sizeof(ARESTA));

	new_aresta->adj = v2;
	new_aresta->next = NULL;
	new_aresta->peso = peso;

	if(v1->aresta == NULL){
		v1->aresta = new_aresta;
		v1->aresta->tail = new_aresta;		
	} else {
		v1->aresta->tail->next = new_aresta;
		v1->aresta->tail = new_aresta;
	}

	return;

}

void imprimi_grafo(GRAFO *g){
    if (g == NULL)
    	return;
    
    VERTICE *v_aux = g->head;

    while(v_aux != NULL){
        printf("%d ->", v_aux->chave);

        ARESTA *a_aux = v_aux->aresta;
        
        while(a_aux != NULL){
            printf(" %d(%d)", a_aux->adj->chave, a_aux->peso);
            a_aux = a_aux->next;
        }
        puts("");
 		
 		v_aux = v_aux->next;

    	free(a_aux);
    }
    free(v_aux);
    return;
}

void imprimi_estruturas(int v, int *d, int *p){
	printf(" v   d   p\n");
	for(int i = 1; i < v+1; ++i){
		if(d[i] == 1000000)
			printf("%2d: inf %3d\n", i, p[i]);
		else
			printf("%2d: %2d %3d\n", i, d[i], p[i]);
	}
} 

FILA *cria_fila(){
	FILA *new_fila = (FILA*) malloc(sizeof(FILA));
	new_fila->head = NULL;
	new_fila->tail = NULL;

	return new_fila;
}

void enqueue(FILA *f, int num, int peso) {

	NODE *new_node = (NODE*) malloc(sizeof(NODE));
	new_node-> conteudo = num;
	new_node->prioridade = peso;
	new_node->next = NULL;
	new_node->prev = NULL;

	if(f->head == NULL){
		f->head = new_node;	

	} else if(f->head->prioridade > peso){
		if(f->head->conteudo == num){
			new_node->next = f->head->next;
			dequeue(f);
			f->head = new_node;
		} else {
			new_node->next = f->head;
			f->head->prev = new_node;
			f->head = new_node;
		}
	
	} else {
		NODE *aux = f->head;
		while (aux->next != NULL && aux->next->prioridade < peso) {
			aux = aux->next; 

			if(aux->conteudo == num)
				return;
        }

        new_node->next = aux->next;
        aux->next = new_node; 
	}
	
	return;
}

void dequeue(FILA *f){
	if (f->head == f->tail && f->head != NULL){
		free(f->head);
		f->head = NULL;
		f->tail = NULL;
	} else {
		NODE *aux = f->head->next;
		free(f->head);
		f->head = aux;
	} 
	return;
}

void imprimi_fila(FILA *f){
	printf("FILA: ");
	NODE *aux = f->head;
	while (aux != NULL){
		printf("%d(%d) -> ", aux->conteudo, aux->prioridade);
		aux = aux->next;
	}
	printf("NULL\n\n");
	free(aux);
	return;
}

NODE *busca_fila(FILA *f, int num, int prioridade){
	NODE *aux = f->head;
	while(aux != NULL){
		if(aux->conteudo == num){
			if(aux->prioridade > prioridade){
				aux->prioridade = prioridade;
				return aux;
			}
		}
		aux = aux->next;
	}
	return NULL;
}

void djikstra(GRAFO *g, FILA *f, int vertice_atual, int v, int *d, int *p, int cont){
	VERTICE *v_aux = busca_vertice(g, vertice_atual); 	
	++cont;

	ARESTA *a_aux = v_aux->aresta;
        
 
    dequeue(f);

    while(a_aux != NULL){

	   	if(a_aux->peso + d[vertice_atual] < d[a_aux->adj->chave]){
	   		p[a_aux->adj->chave] = vertice_atual;
	   		d[a_aux->adj->chave] = a_aux->peso + d[p[a_aux->adj->chave]]; 
		   	
		   	NODE *aux = busca_fila(f, a_aux->adj->chave, a_aux->peso + d[p[a_aux->adj->chave]]);
		   	if(aux == NULL){
		    	enqueue(f, a_aux->adj->chave, a_aux->peso + d[p[a_aux->adj->chave]]);
		   	}
	   	}
    	
	    a_aux = a_aux->next;
    }
    

    printf("ITERAÇÃO %d:\n", cont);
	imprimi_estruturas(v, d, p);
    printf("\n");
    imprimi_fila(f);
    printf("\n");


    if(f->head == NULL){
    	return;
    }

   	djikstra(g, f, f->head->conteudo, v, d, p, cont);

   	return;
}