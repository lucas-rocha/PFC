#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define INDEX_SIZE_INCREMENT 500

/* This program reads a text file where each line corresponds to a word and each column represents a document. Each entry in the file 
	is the tf (term frequency) of the term in the document. The program
	computes for each term the its df (document frequency - number of odocuments where the term occurs.) and computes for each pair of documents the dot product between the two documents.  The dot product. Next the program generate a graph where vertices are documents and there is an edge between two documents if the dot product between the documents is greater than zero. the weight of each edge is the dot-product value between the two documents in the pair. the df of each term is normalized by the greatest df value among all terms.
	*/

/* How to execute the progrmam: ./idfInternProductGraph.c  matrixTermXDoc.txt name of the format to generat the output grapph:  net (Pajek format)  or dot (Graphviz format).
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
	float dotProd;
	struct d* prox;
}TDocInvListNode;

//Tipo do indice de termos
typedef struct ti{
	int indexSize;//number of cells allocated for the doc termListVec in memmory. May be greater than the number of terms.
	TTermInvListNode** termListVec;
}TTermIndex;

//Tipo do indice de documentos
typedef struct di{
	int indexSize; //number of cell allocated for the doc docListVec in memmory. May be greater than the number of documents.
	TDocInvListNode**  docListVec;
}TDocIndex;

//Criando os indices de termos e de documentos
TTermIndex termIndex;
TDocIndex docIndex;

//function for preparing the Term idex to store the index
void prepareTermIndex();

//function for preparing the Doc idex to store the index
void prepareDocIndex(int numDocs);

void incrementDocPair(int doc1, int doc2, float df ,int *numEdges);
void readMatrix(FILE *f, int* numTerms, int* numDocs);
void printTermIndex();
int computeDotProduct(int numTerms, float *df);
void generatingDotFormat(char* graphName, int numDocs);
void generatingNetFormat(char* graphName, int numDocs, int numEdges);
void computeDf(float *df, int numTerms);

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
	//creat a dynamic array to store the df of terms:
	float *df= (float*)calloc(numTerms,sizeof(float));
	if(!df){
		fprintf(stderr, "There is not suffcient memory to run the program. Aborting ...\n");
		exit(1);
	}
	//Compute the df of each term
	computeDf(df, numTerms);
	//Initialize the adjacent lists of documents
	prepareDocIndex(numDocs);

	//Compute the dot product for each pair of documents using the document index to store the dot products.
	int numEdges=computeDotProduct(numTerms, df);

	//Generate the graph in the format indicated by argv[2]

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
void incrementDocPair(int doc1, int doc2, float df, int *numEdges){
	TDocInvListNode *q, *p = docIndex.docListVec[doc1];
	if (!p) {
    p=(TDocInvListNode*)malloc(sizeof(TDocInvListNode));
		if(!p){
			fprintf(stderr,"Error: there is not sufficient memory. Aborting...\n");
			exit(1);
		}
		p->doc=doc2;
		p->dotProd=df*df;
		p->prox=NULL;
		docIndex.docListVec[doc1]=p;
		(*numEdges)++;
	}
	else{// search for doc2 in the ajacent list of doc1
		while (p->doc!=doc2 && p->prox!=NULL) {
			p=p->prox;
		}
		if (p->doc==doc2) {
			p->dotProd+=df*df;
		}
		else{//insert doc2  at the end of the adjacent list of doc1
			p->prox=(TDocInvListNode*)malloc(sizeof(TDocInvListNode));
			q=p->prox;
			if (!q) {
				fprintf(stderr,"Error: there is not sufficient memory. Aborting...\n");
				exit(1);
			}
			q->doc=doc2;
			q->dotProd=df*df;
			q->prox=NULL;
			(*numEdges)++;
		}
	}
}
int computeDotProduct(int numTerms, float *df){
	int i,numEdges=0;
	TTermInvListNode *p,*q;
	for (i=0; i<numTerms; i++) {
		for (p=termIndex.termListVec[i]; p;  p=p->prox) {
			for (q=p->prox; q; q=q->prox) {
				incrementDocPair(q->doc,p->doc, df[i], &numEdges);
			}
		}
	}
	return(numEdges);
}

void computeDf(float *df, int numTerms){
	int i; TTermInvListNode* aux;
	int imax;
	float max=-1.0;
	for (i=0; i<numTerms; i++) {
    aux=termIndex.termListVec[i];
		while (aux) {
			df[i]+=1.0;
			aux=aux->prox;
		}
		if (df[i]>max) {
			max=df[i];
		}
	}
	for (i=0; i<numTerms; i++) {
    df[i]=df[i]/max;
	}

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
					printf("%d -- %d \[label=\"%.1f\" fontsize=7 weight=\"%.6f\"]\n",i+1,p->doc+1,p->dotProd,p->dotProd);
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
				printf("%d  %d %.6f \n",i+1,p->doc+1,p->dotProd);
				p=p->prox;
			}
		}
	}
}

