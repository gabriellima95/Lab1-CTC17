#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

float distancia(int u);

typedef struct node node;
typedef node* pont;
struct node
{
    int id;
    pont prox;
};

typedef struct visit visit;
struct visit
{
    int visited;
    float dist;
    float x;
    float y;
};

float xm=34.367;
float ym=57.5;
pont head[735];
visit v[735];
pont pq, pqlast;
int pai[735];
int main()
{
    // ESSE COMEÇO É SOMENTE O TRATAMENTO DE DADOS PARA SEREM GUARDADOS NUMA LISTA DE ADJACÊNCIA
    char buffer[1024] ;
    char *record,*line;
    int id;
    float x,y;
    FILE *fstream = fopen("Uruguay.csv","r");
    if(fstream == NULL)
    {
        printf("\n file opening failed ");
        return -1 ;
    }
    while((line=fgets(buffer,sizeof(buffer),fstream))!=NULL)
    {
        record = strtok(line,";");
        int n = 0;
        pont aux;
        while(record != NULL)
        {
            if(n==0) id = atoi(record);
            else if(n==1)
            {
                char *string1;
                string1 = malloc(strlen(record));
                strcpy(string1, record);
                string1[2]='.';
                x=atof(string1);
                v[id].x=x;
                free(string1);
            }
            else if(n==2)
            {
                char *string2;
                string2 = malloc(strlen(record));
                strcpy(string2, record);
                string2[2]='.';
                y=atof(string2);
                v[id].y=y;
                free(string2);
            }
            else if(n==3)
            {
                if(atoi(record)!=0)
                {
                    head[id]=malloc (sizeof (node));
                    aux=head[id];
                    aux->id=atoi(record);
                }
            }
            else
            {
                if(atoi(record)!=0)
                {
                    aux->prox=malloc (sizeof (node));
                    aux=aux->prox;
                    aux->id=atoi(record);
                }
            }
            record = strtok(NULL, ";");
            n++;
        }
        v[id].dist = sqrt((xm-x)*(xm-x)+(ym-y)*(ym-y)); // CALCULOS DA DISTANCIA ATÉ A CIDADE DESTINO
        v[id].visited=0;// TODOS RECEBEM 0 PARA INDICAR QUE NÃO FORAM VISITADOS
        aux->prox=NULL;
    }
    pq=NULL;
    greedy(203);
    return 0 ;
}

void insert(int id)  //INSERE O ELEMENTO NUMA FILA DE PRIORIDADES
{
    pont  in=malloc (sizeof (node));
    in->id=id;
    if(pq==NULL)
    {
        pq=malloc (sizeof (node));
        pq->id=id;
        pq->prox=NULL;
    }
    else
    {
        pont p=pq;
        pont q=pq;
        while(p!=NULL && v[p->id].dist<v[id].dist)
        {
            q=p;
            p=p->prox;
        }
        if(p==q) pq=in;
        else q->prox=in;
        in->prox=p;
    }
}


float distancia(int u)
{
    float dist=0;
    while(pai[u]!=u)
    {
        dist=dist+sqrt((v[u].x-v[pai[u]].x)*(v[u].x-v[pai[u]].x)+(v[u].y-v[pai[u]].y)*(v[u].y-v[pai[u]].y));
        u=pai[u];
    }
    return dist;
}

void deletemin() //DELETA O ELEMENTO DA FILA DE PRIORIDADES
{
    v[pq->id].visited=1;
    pont aux=pq;
    pq=pq->prox;
    free(aux);
}

void greedy(int start)
{
    pai[start]=start;
    insert(start);
    while(pq!=NULL)
    {
        int u=pq->id;
        deletemin();
        if(u==600)
        {
            fim(u);
            break;
        }
        else
        {
            pont aux=head[u];
            while(aux!=NULL)
            {
                if(v[aux->id].visited==0)
                {
                    v[aux->id].visited=1;
                    pai[aux->id]=u; // SALVA O CAMINHO PARA CHEGAR ATÉ A CIDADE DESTINO
                    insert(aux->id);
                }
                aux=aux->prox;
            }
        }
    }
}


void fim(int u)
{
    float distfinal=distancia(u);
    printf("\nDistancia: %f",distfinal);
    printf("\nCidades percorridas:\n");
    while(pai[u]!=u)
    {
        printf("--%d",u);
        //dist=dist+sqrt((v[u].x-v[pai[u]].x)*(v[u].x-v[pai[u]].x)+(v[u].y-v[pai[u]].y)*(v[u].y-v[pai[u]].y));
        u=pai[u];
    }
    printf("--%d",u);
}
