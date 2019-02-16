#include<stdio.h>
#include<stdlib.h>
#include<string.h>


int main(int argc, char**argv){
	/* This program reads the text file in argv[1] which contains the output of the
	 the infomap clustering tool and the the text file in argv[2] which contains a tweet collection and generates a text file with tweets gruped into
	cluters found by infomap. The file in argv[1] is not exactly the output of
	infomap but it must be pre-processed so that it has the following format:
	cluster:num float-value "object-label" object-id. Example: 1:1 0.0186775 "17" 17.
	The resulting file is output in the standard output device.
*/
	char nameArq1[1000];
	char nameArq2[1000];
	if (argc!=3) {
    fprintf(stderr, "Correct usage: ./separaDocsPorCluster  Infomap-output-file tweetFile\n");
		exit(1);
	}
	FILE *f1, *f2;
	//create a dynamic array which store for each document its correponding cluster number
  int* vetDoc=(int*)malloc(20000*sizeof(int));
	int vetDocSize=20000;
	if (!vetDoc) {
    fprintf(stderr,"There isn't sufficient memory. Aborting...\n");
		exit(1);
	}
	int i;

	//Opening the cluster file and the tweet file
	f1=fopen(argv[1], "rt");
	f2= fopen(argv[2], "rt");
	if (!f1) {
    fprintf(stderr,"File %s could not be opened\n", argv[1]);
		exit(1);
	}
	if (!f2) {
    fprintf(stderr,"File %s could not be opened\n", argv[2]);
		exit(1);
	}
	int clu, stuff1, doc;
	float stuff2;

	//Reading the cluster file
	fscanf(f1,"%d:%*s %f %*s %d",&clu, &stuff2, &doc);
	while (!feof(f1)) {
		if(doc >=vetDocSize){
			vetDocSize+=2000;
			int *p=(int*)realloc(vetDoc,vetDocSize*sizeof(int));
			if (!p) {
				fprintf(stderr,"There isn't sufficient memory. Aborting...\n");
				exit(1);
			}
			vetDoc=p;
		}
		vetDoc[doc]=clu;
    fscanf(f1,"%d:%*s %f %*s %d",&clu,  &stuff2, &doc);
	}

	fclose(f1);
	//Reading the tweet file
	char tweet[500];
	fscanf(f2,"%[^\n]",tweet); fgetc(f2);
	int contTweet=0;
	FILE* f3;
	char fileName[1000];
	char aux[10];
	while (!feof(f2)) {
		strcpy(fileName, "Cluster-");
		contTweet++;
		clu=vetDoc[contTweet];
		sprintf(aux,"%d",clu);
		strcat(fileName,aux);
		f3=fopen(fileName,"at");
		if (!f3) {
			fprintf(f3,"File %s could not be opened\n", fileName);
			exit(1);
		}
		//verifying if the file has just being created
		fseek (f3, 0, SEEK_END);   // non-portable
    int size=ftell (f3);
		if (size==0) {
			fprintf(f3,"\n*******************\n");
			fprintf(f3, "Cluster %d:\n", clu);
			fprintf(f3,"*******************\n\n");
		}
		fprintf(f3,"%s\n", tweet);
		fclose(f3);
		fileName[0]='\0';
		fscanf(f2,"%[^\n]",tweet); fgetc(f2);
	}
	system("cat Cluster-*");
	system("rm Cluster-*");
	return(0);
}