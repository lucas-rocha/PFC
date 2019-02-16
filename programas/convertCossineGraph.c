#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*comando de execucao: ./convertCosineGraph  arqCossine.txt nome do
formato que se quer gerar o grafo: net ou lgl num colunas do arq arqCossine.txt
arqCoocur.txt e uma matriz quadrada que para cada entrada i,j contem o valor de cosseno entre os documentos i e j.
*/


int main(int argc, char** argv){
	char nomeArq[1000];
	FILE *f =fopen(argv[1],"rt");
	if (!f) {
    fprintf(stderr, "arquivo de cossenos nao pode ser aberto\n");
		exit(1);
	}

	char arqGraphName[300];
	strcpy(arqGraphName,argv[1]);
	char* p=strstr(arqGraphName,".txt");
	if (strcmp(argv[2], "net")==0) {
    strcpy(p,".net");
	}
	else{
		if (strcmp(argv[2], "dot")==0) {
			strcpy(p,".dot");
		}
	}

	printf("Gerando arquivo: %s\n", arqGraphName);
	FILE *f2 =fopen(arqGraphName, "wt");
	if (!f2) {
    fprintf(stderr, "arquivo nao pode ser aberto\n");
		exit(1);
	}
	int numDocs= atoi(argv[3]);
	double mat[numDocs];
	//Gerando os vertices do grafo - no formato net os vertices devem comecar
	//com indice 1, entao o primeiro vertice tera indice 1 e rotulo "1"
	//Ler matriz de Coocorrencia
	int i,j;

	if(strcmp(argv[2], "net")==0){
		fprintf(f2,"*Vertices %d\n", numDocs);
		for (i=1; i<=numDocs; i++) {
    	fprintf(f2, "%d \"%d\"\n", i,i);
		}
		double cos;
		int numArcs=0;
		//contando número de arestas
		for (i=0; i<numDocs; i++) {
			for (j=0; j<numDocs; j++) {
				fscanf(f,"%lf", &mat[j]);
				if (j>i && mat[j]>0.0) {
					numArcs++;
				}
			}
		}
		rewind(f);
		fprintf(f2, "*Arcs %d\n", numArcs); //Imprimindo as arestas do grafo
		for (i=0; i<numDocs; i++) {
			for (j=0; j<numDocs; j++) {
				fscanf(f,"%lf", &mat[j]);
				if (j>i && mat[j]!=0.0) {
					fprintf(f2,"%d %d %.6f\n", i+1, j+1, mat[j]);
				}
			}
		}
	}
	fclose(f2);
	return(0);
}