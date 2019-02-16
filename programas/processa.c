#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef struct t{
	int nodeId;
	char nodeName[200];
}Tterm;

int main(int argc, char** argv){
	char nomeArq[1000];
	FILE *f =fopen(argv[2],"rt");
	if (!f) {
    fprintf(stderr, "arquivo nao pode ser aberto\n");
		exit(1);
	}
	FILE *f2 = fopen(argv[1], "rt");
	if (!f2) {
    fprintf(stderr, "arquivo nao pode ser aberto\n");
		exit(1);
	}
	char arqGraphName[300];
	strcpy(arqGraphName,argv[2]);
	char* p=strstr(arqGraphName,".txt");
	strcpy(p,".net");
	printf("%s\n", arqGraphName);
	FILE *f3 =fopen(arqGraphName, "wt");
	if (!f3) {
    fprintf(stderr, "arquivo nao pode ser aberto\n");
		exit(1);
	}
	int numTerms= atoi(argv[3]);
	Tterm vetTerms[numTerms];
	char name[500];
	int lin;
	//Ler os nomes dos termos do arquivo jason

	while (fscanf(f2, "%d" "%s",&lin, name)!=EOF) {
		//printf("%d %s\n", lin, name);
		vetTerms[lin].nodeId=0; //inicia todos nodeId com zero
    strcpy(vetTerms[lin].nodeName,name);
		printf("%d %s\n", lin, vetTerms[lin].nodeName);
	}
	printf("LIN: %d\n",lin);
	short mat[numTerms][numTerms];
	//Ler matriz
	int i,j;
	for (i=0; i<numTerms; i++) {
		for (j=0; j<numTerms; j++) {
			fscanf(f,"%hd", &mat[i][j]);
		}
	}
	int numVertices=0;
	int Id=1;
	for (i=0; i<numTerms; i++) {
		for (j=0; j<numTerms; j++) {
			if (mat[i][j]>2 && i!=j) {
				numVertices++;
				vetTerms[i].nodeId=Id++;
				break;// the node is connected to at least one edge with weight greater than 2
			}
		}
	}
	/*gerando os vertices no formato .net aceito pelo Gephy*/
	fprintf(f3,"*Vertices %d\n", numVertices);
	for(i=0;i<numTerms;i++){
		if (vetTerms[i].nodeId) {
			fprintf(f3,"%d \"%s\"\n",vetTerms[i].nodeId, vetTerms[i].nodeName);
		}

	}

	//printf("\nGraph:\n");
	fprintf(f3,"*arcs\n");
	for (i=0; i<numTerms; i++) {
		for (j=i+1; j<numTerms; j++) {
			if (mat[i][j]>2) {
				fprintf(f3,"%d %d %d\n",vetTerms[i].nodeId,vetTerms[j].nodeId, mat[i][j]);

			}
		}
	}
}