#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

float distancia(int u);
float distanciaentrenodes(int u, int t);

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
    int examined;
    float dist; // heuristc h
    float gscore; // Distancia percorrida ate o node
    float fscore; // gscore + dist
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
                string1 = malloc(strlen(record)); /* hello + 0-terminator */
                strcpy(string1, record);
                string1[2]='.';
                x=atof(string1);
                v[id].x=x;
                //printf("%s %f", string1, x);
                free(string1);
            }
            else if(n==2)
            {
                char *string2;
                string2 = malloc(strlen(record)); /* hello + 0-terminator */
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
        v[id].visited=0;
        v[id].examined=0;// TODOS RECEBEM 0 PARA INDICAR QUE NÃO FORAM VISITADOS
        v[id].gscore=100000.0;
        v[id].fscore=100000.0;
        aux->prox=NULL;
    }
    //int a;
    //pont a;
    //for(a=1;a<735;a++) printf("%d %f %f %f\n",a, v[a].dist, v[a].x, v[a].y);
    //for(a=head[713];a!=NULL;a=a->prox) printf("%d ", a->id);
    //greedy(203);
    astar(203);
    return 0 ;
}

/*void insertAstar(int id)  //INSERE O ELEMENTO NUMA FILA DE PRIORIDADES
{
    printf("inserir id %d\n", id);
    v[id].visited=1;
    pont in=malloc (sizeof (node));
    in->id=id;
    if(pq==NULL)
    {
        pq=in;
        in->prox=NULL;
        pqlast=in;
    }
    else
    {
        pqlast->prox=in;
        pqlast=in;
        in->prox=NULL;
    }
    if(id==222 || id==225){
        pont y=pq;
        float cu;
        while(y!=NULL){
            cu=v[y->id].dist+v[y->id].gscore;
            printf("Teste: FP %d com peso %f + %f = %f \n", y->id,v[y->id].dist,v[y->id].gscore,cu);
            y=y->prox;
        }
    }
    //printf("o ultimo eh %d\n", pqlast->id);
}
*/
void insert(int id)  //INSERE O ELEMENTO NUMA FILA DE PRIORIDADES
{
    v[id].visited=1;
    pont in=malloc (sizeof (node));
    in->id=id;
    pont aux=pq;
    if(pq==NULL){
        pq=in;
        in->prox=NULL;
    }
    else{
        while(aux->prox!=NULL) aux=aux->prox;
        aux->prox=in;
        in->prox=NULL;
    }

}

float distancia(int u)
{
    float dist=0;
    while(pai[u]!=u)
    {
        //printf("%d--",u);
        dist=dist+sqrt((v[u].x-v[pai[u]].x)*(v[u].x-v[pai[u]].x)+(v[u].y-v[pai[u]].y)*(v[u].y-v[pai[u]].y));
        u=pai[u];
    }
    //printf("\n--%f--\n",dist);
    return dist;
}

float distanciaentrenodes(int u, int t)
{
    float ret;
    ret=sqrt((v[u].x-v[t].x)*(v[u].x-v[t].x)+(v[u].y-v[t].y)*(v[u].y-v[t].y));
    return ret;
}

int deletemin() //DELETA O ELEMENTO DA FILA DE PRIORIDADES
{
    pont aux=pq;
    pont menor = aux;
    while(aux!=NULL){
        if(v[menor->id].fscore>v[aux->id].fscore)
            menor=aux;
        aux=aux->prox;
    }
    if(pq==menor) pq=menor->prox;
    else {
        aux=pq;
        while(aux->prox!=menor) aux=aux->prox;
        aux->prox=menor->prox;
    }
    int menos=menor->id;
    free(menor);
    return menos;
}

void astar(int start)
{
    pq=NULL;
    v[start].gscore=0;
    v[start].fscore=v[start].dist;
    insert(start);
    while(pq!=NULL)
    {
        int u=deletemin();
        v[u].examined=1;
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
                if(v[aux->id].examined==0)
                {
                    if(v[aux->id].visited==0)
                    {
                        pai[aux->id]=u;
                        insert(aux->id);
                    }
                    float tentativa = v[u].gscore+distanciaentrenodes(u,aux->id);
                    if(tentativa < v[aux->id].gscore)
                    {
                        pai[aux->id]=u;
                        v[aux->id].gscore=tentativa;
                        v[aux->id].fscore=v[aux->id].gscore+v[aux->id].dist;
                    }
                }
                aux=aux->prox;
            }

        }
    }
}

void fim(int u)
{
    int aux=u;
    while(pai[u]!=u)
    {
        printf("--%d",u);
        //dist=dist+sqrt((v[u].x-v[pai[u]].x)*(v[u].x-v[pai[u]].x)+(v[u].y-v[pai[u]].y)*(v[u].y-v[pai[u]].y));
        u=pai[u];
    }
    printf("\nDistancia Final: %f\n",v[aux].fscore);
}

