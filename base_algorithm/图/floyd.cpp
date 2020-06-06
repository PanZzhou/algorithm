#include <cstdio>
#include "graph.h"
using namespace std;

void print_result(int path[][7]){
	printf("  A B C D E F G\n");
	for(int i=0;i<vexnum;i++){
		printf("%c ", i+65);
		for(int j=0;j<vexnum;j++){
			if(path[i][j]<=0)
				printf("# ");
			else
				printf("%c ", path[i][j]+65);
		}
		printf("\n");
	}
}

//O(n3)
void floyd(int graph[][7]){
	int A[vexnum][vexnum];
	int path[7][7];
	for(int i=0; i<vexnum; i++){
		for(int j=0; j<vexnum; j++){
			A[i][j] = graph[i][j];
			path[i][j] = -1;
		}
	}
	for(int k=0; k<vexnum; k++){
		for(int i=0; i<vexnum; i++){
			for(int j=0; j<vexnum; j++){
				if(A[i][j]>A[i][k] + A[k][j]){
					A[i][j] = A[i][k] + A[k][j];
					path[i][j]=k;
				}
			}
		}
	}
	print_result(path);
}

int main() {
	floyd(graph);
}