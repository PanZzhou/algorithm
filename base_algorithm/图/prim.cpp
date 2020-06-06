#include <cstdio>
#include "graph.h"
using namespace std;

void init_lowcost_adjvex(int *lowcost, int *adjvex, int graph[][7], int vex){
	for(int i=0; i<vexnum; i++){
		lowcost[i]=graph[vex][i];
		adjvex[i]=vex;
	}
}

//O(n2)
void prim(int graph[][7], int vex){
	int lowcost[vexnum];//lowcost[i]为零表示i节点已被加入生成树中
	int adjvex[vexnum];//记录前驱节点下表
	init_lowcost_adjvex(lowcost, adjvex, graph, vex);
	for(int i=1; i<vexnum; i++){
		int min = MAX, k;
		for(int j=0; j < vexnum; j++){
			if(lowcost[j] != 0 && lowcost[j] < min){
				min = lowcost[j];
				k = j;
			}
		}
		printf("%c ---- %c\n", adjvex[k]+65, k+65);
		lowcost[k] = 0;
		for(int j=0; j<vexnum; j++){
			if(lowcost[j]!=0 && lowcost[j]>graph[k][j]){
				lowcost[j] = graph[k][j];
				adjvex[j] = k;
			}
		}
	}
}

int main(){
	prim(graph, 3);
}