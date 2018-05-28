package my_altorithm;
//import java.util.LinkedList;
//import java.util.Queue;

public class BFS_DFS {
 /*
  * verNum:存储节点的长度
  * visited:布尔数组，标记当前节点是否已经访问过
  * ver:存储节点的关键字
  * edge:二维数组，存储节点之间边的信息的列表
  */
 static int verNum;
 static boolean []visited;
 static int [][]edge;
 private int count = 0;
 private static String s = "C:\\Users\\Administrator\\Desktop\\成果\\graph_matrix_1.txt";
 private static int N = 10000;
BFS_DFS(){
	read_graph_from_txt txt = new read_graph_from_txt();
	edge = txt.read(s);
	verNum = edge[0].length;
	count = 0;
}
 /*
  * 图的深度优先遍历操作
  */
 void dfsTraverse(){
  //将所有节点标记为未访问
  visited=new boolean[verNum];
  for(int i=0;i<verNum;i++){
   if(visited[i]==false){
	   count++;
	   dfs(i);
   }else
	   System.out.println();
  }
 }

 /*
  * 图的深度优先递归算法
  */
 void dfs(int i){
  visited[i]=true;               //访问节点
  System.out.print("节点 "+i+"->");
  for(int j=0;j<verNum;j++){        //遍历边信息列表，进行递归
   if(visited[j]==false&&edge[i][j]!=N){
    dfs(j);
   }
  }
 }

 /*
  * 图的广度优先遍历算法
  */
// void bfsTraverse(){
//  visited=new boolean[verNum];         //标记所有节点均未被访问
//  Queue<Integer>queue=new LinkedList<Integer>(); //存储节点的队列
//  for(int i=0;i<verNum;i++){           //访问节点
//   if(visited[i]==false){
//    visited[i]=true;
//    System.out.print(ver[i]+" ");
//    queue.add(i);                //顶点入队列
//    while(!queue.isEmpty()){          //队列不为空时进行遍历操作
//     int j=queue.poll();             //顶点出队列，作为下一遍历中边信息列表的行，保证每个边信息列表都得到遍历
//     for(int k=0;k<verNum;k++){
//      if(edge[j][k]==1&&visited[k]==false){
//       visited[k]=true;
//       System.out.print(ver[k]+" ");
//       queue.add(k);
//      }
//     }
//    }
//   }
//  }
// }

 /*
  * 将边的信息列表构造成一个完整的边信息列表
  */
// void addEdge(int i,int j){
//  if(i==j)
//   return;
//  edge[i][j]=1;
//  edge[j][i]=1;
// }

 /*
  * 求该图的连通分量(测试图为不连通图，因此求出该图的最大连通分量)
  */
 public void con(){
	 System.out.println(count);
 }

 public static void main(String[] args) {
  // TODO Auto-generated method stub
  BFS_DFS b=new BFS_DFS();
  System.out.println("图的深度遍历操作:");
  b.dfsTraverse();
  System.out.println();
//  System.out.println("图的广度遍历操作:");
//  b.bfsTraverse();
//  System.out.println();
  System.out.println("连通分量:");
  b.con();
 }
}