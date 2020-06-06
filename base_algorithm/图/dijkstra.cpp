#include <cstdio>
#include "graph.h" //图的结构存储
using namespace std;

void init_dist(int *dist, int vex){
	for(int i = 0; i < vexnum; i++)
		dist[i] = graph[vex][i];
}

void init_path(int* path, int vex){
	for(int i = 0; i < vexnum; i++){
		if(i != vex && graph[vex][i] < MAX)
			path[i] = vex; //初始状态中，和源点相连的节点的前驱节点置为源点
		else{
			path[i] = -1;
		}
	}
}

void init_s(int* s, int vex){
	for(int i = 0; i< vexnum; i++){
		i==vex?s[i]=1:s[i]=-1;
	}
}

int find_min(int* dist, int* s){
	int flag = -1, i = 0;
	while(i < vexnum){
		if(s[i] != 1 && (flag == -1 || dist[i] < dist[flag])){
			flag = i;
		}
		i++;
	}
	return flag;
}

void print_result(int *dist, int *path, int vex){
	int flag;
	for(int i=0; i<vexnum; i++){
		if(vex == i) continue;
		flag = i;
		while(path[flag]!=-1){
			printf("%c -- ", flag+65);
			flag = path[flag];
		}
		printf("%c,dist is %d\n", vex+65, dist[i]);
	}
}

//O(n2)
void dijkstra(int graph[][7], int vex){
	int dist[vexnum]; //每个点距离参数vex节点的距离
	int s[vexnum]; //集合s，值为非负即表示加入了s集合，从而标记已访问
	int path[vexnum]; //记录每个点到源点的前驱节点
	init_dist(dist, vex);
	init_s(s, vex);
	init_path(path, vex);
	int a = 0, min_flag;
	while(a<vexnum){
		min_flag = find_min(dist, s);
		printf("tag: %d\n", min_flag);
		for(int k = 0; k < vexnum; k++){
			if(k != min_flag && s[k] != 1 && (dist[k] > dist[min_flag] + graph[min_flag][k])){
				// printf("distance: %d %d %d\n", dist[k], dist[min_flag], graph[min_flag][k]);
				dist[k] = dist[min_flag] + graph[min_flag][k];
				path[k] = min_flag;
			}
		}
		if(min_flag >= 0)
			s[min_flag] = 1;
		a++;
	}
	print_result(dist, path, vex);
}
int main(){
	dijkstra(graph, 4);
}