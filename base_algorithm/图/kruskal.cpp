#include <cstdio>
#include <iostream>
#include "graph.h" //图的结构存储
using namespace std;

//边数组结构体
typedef struct{
	char a, b; //边的两个顶点
	int weight;//边权值
}Edge;

//初始化边数组,初始化为有序数组
void init_edges(Edge* edges){
	int a = 0, b;
	for(int i = 0; i < vexnum - 1; i++){
		for(int j = i + 1; j < vexnum; j++){
			if(graph[i][j] < MAX){//边存在，则将其加入数组并排序(插入排序)
				b = a - 1;
				while(b >= -1){
					if(b == -1 || graph[i][j]>=edges[b].weight){
						edges[b+1].weight = graph[i][j];
				        edges[b+1].a = i + 65; //邻接矩阵下标转换成字母（0-A...）
				        edges[b+1].b = j + 65; ////邻接矩阵下标转换成字母（0-A...）
				        ++a;
				        break;
					}else{
						edges[b+1] = edges[b];
						b--;
					}
				}
			}
		}
	}
	//输出排序后的边数组
	for(int i = 0; i<arcnum; i++){
		printf("weight: %d  arc: %c ------- %c\n", edges[i].weight, edges[i].a, edges[i].b);
	}
}
void init_parents(int *p){
	for(int i = 0; i < vexnum; i++) p[i] = -1;
}

//并查集，查找tag节点的最小生成树的根节点
int find_parents(int* p, int tag){
	while(p[tag] >= 0) tag = p[tag];
	return tag;
}

//克鲁斯卡尔算法,O(eloge):e为边数
void kruskal(int graph[][7]){
	int c = 0, m, n;
	int parents[vexnum];//A节点的数组下标是0，以此类推
    Edge edges[arcnum];//初始化后为根据边权重排好序的数组
	init_parents(parents);
	init_edges(edges);
	while(c < arcnum){
		m = find_parents(parents, edges[c].a - 65);
		n = find_parents(parents, edges[c].b - 65);
		if(m != n ){
			parents[n] = m;
			printf("arc: %c ------ %c is choosed!!!\n",edges[c].a,edges[c].b);
		}
		c++;
	}
	//输出parents数组
	for(int i = 0; i<vexnum; i++){
		printf("%d\n", parents[i]);
	}
}

int main(){
	kruskal(graph);
}