#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define INDEX_SIZE_INCREMENT 500


/*comando de execucao: ./convertDocCoocurGraph.c  matrixTermXDoc.txt nome do
formato que se quer gerar o grafo: net ou doc.
*/

//Definindo o tipo do no da lista invertida de termos
typedef struct t{
	int doc;
	int freq;
	struct t* prox;
} TTermInvListNode;

//Definindo o tipo de no da lista invertida de documentos que ira armazenar
//o numero de termos em comum entre cada par de documentos.

typedef struct d{
	int doc;
	int numCoocur;
	struct d* prox;
}TDocInvListNode;

//Tipo do indice de termos
typedef struct ti{
	int indexSize;
	TTermInvListNode** termListVec;
}TTermIndex;

//Tipo do indice de documentos
typedef struct di{
	int indexSize;
	TDocInvListNode**  docListVec;
}TDocIndex;

//Criando os indices de termos e de documentos
TTermIndex termIndex;
TDocIndex docIndex;

//function for preparing the Term idex to store the index
void prepareTermIndex();

//function for preparing the Doc idex to store the index
void prepareDocIndex(int numDocs);

void incrementDocPair(int doc1,int doc2, int* numEdges);
void readMatrix(FILE *f, int* numTerms, int* numDocs);
void printTermIndex();
int genDocIndex(int numTerms);
void generatingDotFormat(char* graphName, int numDocs);
void generatingNetFormat(char* graphName, int numDocs, int numEdges);

int main(int argc, char** argv){
	char graphName[100]; char* c;
	strcpy(graphName, argv[1]);
	c=strstr(graphName,".txt"); strcpy(c,"");
	int numTerms, numDocs;
	FILE *f =fopen(argv[1],"rt");
	if (!f) {
    fprintf(stderr, "matriz termoXdoc  nao pode aberto\n");
		exit(1);
	}
	//Criandos os indices de termos e de documentos
	termIndex.indexSize=0; termIndex.termListVec=NULL;
	docIndex.indexSize=0;	docIndex.docListVec=NULL;

	readMatrix(f, &numTerms, &numDocs);
	//printf("NumTerms: %d\n", numTerms);
	//printf("NumDocs: %d\n", numDocs);
	prepareDocIndex(numDocs);
	int numEdges=genDocIndex(numTerms);
	if (strcmp(argv[2],"dot")==0) {
    generatingDotFormat(graphName,numDocs);
		return(0);
	}
	if (strcmp(argv[2],"net")==0) {
    generatingNetFormat(graphName,numDocs,numEdges);
		return(0);
	}
	printf("Invalid parameter for the format of the output graph\n");
	return(0);
}

void prepareTermIndex(){
	TTermInvListNode** aux;
	int i=termIndex.indexSize;
	termIndex.indexSize+=INDEX_SIZE_INCREMENT;
	aux=(TTermInvListNode**)realloc(termIndex.termListVec,(sizeof(TTermInvListNode*)*termIndex.indexSize));

	if (aux) {
		termIndex.termListVec=aux;
    int j;
		for (j=i; j< termIndex.indexSize;j++) {
			termIndex.termListVec[j]=NULL;
		}
	}
	else{
			fprintf(stderr,"There is not suffcient memory. Aborting...\n");
			exit(1);
	}

}
void prepareDocIndex(int numDocs){
	int i;
	docIndex.indexSize=numDocs;
	docIndex.docListVec=(TDocInvListNode**) malloc(sizeof(TDocInvListNode**)*numDocs);
	if (!docIndex.docListVec) {
    fprintf(stderr,"There is not suffcient memory. Aborting...\n");
		exit(1);
	}
	for (i=0; i<docIndex.indexSize; i++) {
    docIndex.docListVec[i]=NULL;
	}
}
void readMatrix(FILE *f, int* numTerms, int* numDocs){
	int c,i=0,t=0,d=0,num,vez=0;
	char snum[50];
	c=fgetc(f);
	//printf("term: %d --> ",t);
	while (!feof(f)){
		while (isdigit(c)) {
			snum[i++]=c;
			c=fgetc(f);
		}
		snum[i]='\0';
		i=0;
		num=atoi(snum);
		snum[0]='\0';
		if (t>=termIndex.indexSize) {
			prepareTermIndex();
		}
		if (num) {//insert document d in the list of term t
			TTermInvListNode* aux=(TTermInvListNode*)malloc(sizeof(TTermInvListNode));
			if (aux) {
				aux->doc=d;
				aux->freq=num;
				aux->prox=termIndex.termListVec[t];
				termIndex.termListVec[t]=aux;
			}
			else{
				fprintf(stderr,"Error: there is not sufficient memory. Aborting...\n");
				exit(1);
			}
		}
		d++;

		while (isspace(c)) {
			if (c=='\n') {
				t++;
				if(!vez){
					*numDocs=d;vez++;
				}
				d=0;
			}
			c=fgetc(f);
		}
	}
	fclose(f);
	*numTerms=t;
}

void printTermIndex(int indexSize){
	int i; TTermInvListNode* aux;
	for (i=0; i<indexSize; i++) {
    aux=termIndex.termListVec[i];
		printf("Term: %d: ",i);
		while (aux) {
			printf("(doc: %d, freq: %d) ", aux->doc, aux->freq);
			aux=aux->prox;
		}
		printf("\n");
	}

}
void incrementDocPair(int doc1, int doc2, int *numEdges){
	TDocInvListNode *q, *p = docIndex.docListVec[doc1];
	if (!p) {
    p=(TDocInvListNode*)malloc(sizeof(TDocInvListNode));
		if(!p){
			fprintf(stderr,"Error: there is not sufficient memory. Aborting...\n");
			exit(1);
		}
		p->doc=doc2;
		p->numCoocur=1;
		p->prox=NULL;
		docIndex.docListVec[doc1]=p;
		(*numEdges)++;
	}
	else{// search for doc2 in the ajacent list of doc1
		while (p->doc!=doc2 && p->prox!=NULL) {
			p=p->prox;
		}
		if (p->doc==doc2) {
			p->numCoocur++;
		}
		else{//insert doc2 in the adjacent list of doc1
			p->prox=(TDocInvListNode*)malloc(sizeof(TDocInvListNode));
			q=p->prox;
			if (!q) {
				fprintf(stderr,"Error: there is not sufficient memory. Aborting...\n");
				exit(1);
			}
			q->doc=doc2;
			q->numCoocur=1;
			q->prox=NULL;
			(*numEdges)++;
		}
	}
}
int genDocIndex(int numTerms){
	int i,numEdges=0;
	TTermInvListNode *p,*q;
	for (i=0; i<numTerms; i++) {
		for (p=termIndex.termListVec[i]; p;  p=p->prox) {
			for (q=p->prox; q; q=q->prox) {
				incrementDocPair(q->doc,p->doc,&numEdges);
			}
		}
	}
	return(numEdges);
}



void generatingDotFormat(char* graphName, int numDocs ){ //Graphviz format
	int i;
	printf("graph %s {\n", graphName);
	printf("nodesep=8\n");
	printf("size=\"7.5,9\"\n");
	printf("ranksep=\"1.2,equally\"\n");
	printf("page=\"8.5,11\"\n");
	//Imprimindo os vertices*
	printf("node [shape=plaintext]\n");
	for(i=0;i<numDocs;i++){
		printf("%d \[label=\"%d\" fontsize=7]\n",i+1, i+1);
	}

	//Imprimindo as arestas
		TDocInvListNode* p;
		for (i=0; i<numDocs; i++) {
			if(docIndex.docListVec[i]){
				p=docIndex.docListVec[i];
				while (p) {
					printf("%d -- %d \[label=\"%d\" fontsize=7 weight=\"%d\"]\n",i+1,p->doc+1,p->numCoocur,p->numCoocur);
					p=p->prox;
				}
			}
		}
		printf("}\n");
}

void generatingNetFormat(char* graphName, int numDocs, int numEdges ){ //Net format
	int i;
	printf("*Vertices %d\n",numDocs);

	//Imprimindo os vertices*
	for(i=0;i<numDocs;i++){
		printf("%d \"%d\"\n",i+1, i+1);
	}

	//Imprimindo as arestas
	printf("*Arcs %d\n",numEdges);
	TDocInvListNode* p;
	for (i=0; i<numDocs; i++) {
		if(docIndex.docListVec[i]){
			p=docIndex.docListVec[i];
			while (p) {
				printf("%d  %d %d \n",i+1,p->doc+1,p->numCoocur);
				p=p->prox;
			}
		}
	}
}


