#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MINCOOCUR 5
#define MINWEIGHT 2

/*comando de execucao: ./converteGraph.c arq.jason arqCoocur.txt nome do
formato que se quer gerar o grafo: net ou lgl num colunas do arq arqCoocur.txt
arq.jason é um arquivo txt com pares termid string do termo (ex. 0 "oi");
arqCoocur.txt e uma matriz quadrada que para cada entrada i,j contem o numero de documentos em que os termos de termid i e j coocorreram
*/

typedef struct t{
	int nodeId;
	char nodeName[200];
}Tterm;

int main(int argc, char** argv){
	char nomeArq[1000];
	FILE *f =fopen(argv[2],"rt");
	if (!f) {
    fprintf(stderr, "arquivo de coocorrencias nao pode aberto\n");
		exit(1);
	}
	FILE *f2 = fopen(argv[1], "rt");
	if (!f2) {
    fprintf(stderr, "arquivo jason nao pode ser aberto\n");
		exit(1);
	}
	char arqGraphName[300];
	strcpy(arqGraphName,argv[2]);
	char* p=strstr(arqGraphName,".txt");
	if (strcmp(argv[3], "net")==0) {
    strcpy(p,".net");
	}
	else{
		if (strcmp(argv[3], "dot")==0) {
			strcpy(p,".dot");
		}
	}

	printf("Gerando arquivo: %s\n", arqGraphName);
	FILE *f3 =fopen(arqGraphName, "wt");
	if (!f3) {
    fprintf(stderr, "arquivo nao pode ser aberto\n");
		exit(1);
	}
	int numTerms= atoi(argv[4]);
	Tterm vetTerms[numTerms];
	char name[500];
	int lin;

	//Ler os nomes dos termos do arquivo jason
	while (fscanf(f2, "%d" "%s",&lin, name)!=EOF) {
		//printf("%d %s\n", lin, name);
		vetTerms[lin].nodeId=0; //inicia todos nodeId com zero
    strcpy(vetTerms[lin].nodeName,name);
		//printf("%d %s\n", lin, vetTerms[lin].nodeName);
	}
	short mat[numTerms][numTerms];
	//Ler matriz de Coocorrencia
	int i,j;
	for (i=0; i<numTerms; i++) {
		for (j=0; j<numTerms; j++) {
			fscanf(f,"%hd", &mat[i][j]);
		}
	}

	int Id=1;
	for (i=0; i<numTerms; i++) {
		for (j=0; j<numTerms; j++) {
			if( (mat[j][i]>=MINWEIGHT) && (i != j) && (mat[i][i]>=MINCOOCUR) ) {
				vetTerms[i].nodeId=Id++;
				break;// the node is connected to at least one edge with weight greater than MINCOOCUR
			}
		}
	}

	if(strcmp(argv[3], "dot")==0){//Gravando no formato do Graphviz
		/*gerando os vertices no formato .dot aceito pelo Graphviz*/
		printf("Numero de termos: %d \n", numTerms);
		p=strstr(arqGraphName,".dot"); strcpy(p,"");
		fprintf(f3,"graph %s {\n", arqGraphName);
		fprintf(f3,"nodesep=8\n");
		fprintf(f3, "size=\"7.5,9\"\n");
		fprintf(f3, "ranksep=\"1.2,equally\"\n");
		fprintf(f3,"page=\"8.5,11\"\n");

		/*Imprimindo os vertices*/
		fprintf(f3,"node [shape=plaintext]\n");
		int aux=numTerms+1;
		for(i=0;i<numTerms;i++){
			if (vetTerms[i].nodeId || (vetTerms[i].nodeId==0 && mat[i][i]>=MINCOOCUR)) {
				if(vetTerms[i].nodeId!=0)
					fprintf(f3,"%d \[label=\"%s\" fontsize=7] /*vertice occurence = %d*/ \n",vetTerms[i].nodeId, vetTerms[i].nodeName,mat[i][i]);
				else
					fprintf(f3,"%d \[label=\"%s\" fontsize=7] /*vertice occurence = %d*/ \n",aux++, vetTerms[i].nodeName, mat[i][i]);
			}
		}
		/*Imprimindo as arestas*/

		for (i=0; i<numTerms; i++) {
			for (j=i+1; j<numTerms; j++) {
				if (mat[i][j]>=MINWEIGHT &&vetTerms[i].nodeId && vetTerms[j].nodeId) {
					fprintf(f3,"%d -- %d \[label=\"%d\" fontsize=7 weight=\"%d\"]\n",vetTerms[i].nodeId,vetTerms[j].nodeId, 	mat[i][j], mat[i][j]);
				}
			}
		}
		fprintf(f3, "}\n");
		fclose(f3);
		return(0);
	}

	if(strcmp(argv[3], "net")==0){ //Gravando no formato net
		//Contando o numero de vertices
		int numVertices=0;
		for (i=0; i<numTerms; i++) {
			if (vetTerms[i].nodeId || (vetTerms[i].nodeId==0 && mat[i][i]>=MINCOOCUR)) {
				numVertices++;
			}
		}
		fprintf(f3,"*Vertices %d\n", numVertices);
		int aux=numTerms+1;
		for(i=0;i<numTerms;i++){
			if (vetTerms[i].nodeId || (vetTerms[i].nodeId==0 && mat[i][i]>=MINCOOCUR)) {
				if(vetTerms[i].nodeId!=0)
					fprintf(f3,"%d \"%s\"\n",vetTerms[i].nodeId, vetTerms[i].nodeName);
				else
					fprintf(f3,"%d \"%s\"\n",aux++, vetTerms[i].nodeName);
			}
		}
		/*Imprimindo as arestas*/
		//Computando o numero de arestas
		int numArestas=0;
		for (i=0; i<numTerms; i++) {
				for (j=i+1; j<numTerms; j++) {
					if (mat[i][j]>=MINWEIGHT &&vetTerms[i].nodeId && vetTerms[j].nodeId) {
						numArestas++;
					}
				}
		}
		fprintf(f3, "*Edges %d\n",numArestas);

		for (i=0; i<numTerms; i++) {
			for (j=i+1; j<numTerms; j++) {
				if (mat[i][j]>=MINWEIGHT &&vetTerms[i].nodeId && vetTerms[j].nodeId) {
					fprintf(f3,"%d %d %d\n",vetTerms[i].nodeId,vetTerms[j].nodeId, mat[i][j]);
				}
			}
		}
		fclose(f3);
		return(0);
	}
}

