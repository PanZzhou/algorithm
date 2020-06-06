//图的结构图如同目录下“无向图.png”所示

//图的邻接矩阵表示法
int MAX = 147483647;//要在数组中使用，所以不能用宏定义的方法
int graph[7][7]={{0, 6, MAX,8, 14, MAX, MAX},
               {6, 0, 8, 3, MAX, MAX, MAX},
               {MAX, 8, 0, 4, MAX, MAX, MAX},
               {8, 3, 4, 0, 4, 12, MAX},
               {14, MAX, MAX, 4, 0, 9, 8},
               {MAX, MAX, MAX, 12, 9, 0, 3},
               {MAX, MAX, MAX, MAX, 8, 3, 0}};

int arcnum = 11;
int vexnum = 7;

// int num = (sizeof(graph[0])/sizeof(int)); //顶点数量

// typedef struct ArcCell{
// 	int weight;
// }ArcCell, AdjMatrix[num][num];

// typedef struct{
// 	int arcnum, vexnum;//弧数和顶点数
// 	AdjMatrix arcs;   //邻接矩阵
// }MGraph;