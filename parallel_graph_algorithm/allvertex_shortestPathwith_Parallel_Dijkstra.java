//全部顶点对间的最短路径，使用源并行形式的Dijkstra算法实现（每个顶点分配一组线程）。
package my_altorithm;

import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

import my_altorithm.Parallel_Dijkstra.computeTask;
import my_altorithm.Parallel_Dijkstra.myDaemon;

class allvertex_shortestPathwith_Parallel_Dijkstra{
	private int task_of_each_point = 4; //每个点分配的线程数
	private int point_num;
	allvertex_shortestPathwith_Parallel_Dijkstra(){
		Parallel_Dijkstra.init_static_items();
		Parallel_Dijkstra.set_p_count(task_of_each_point);
		point_num = Parallel_Dijkstra.get_size();
	}
	public void get_result(){
		long startTime = System.currentTimeMillis();
		ExecutorService exec = Executors.newCachedThreadPool();
		while(--point_num>=0){
			Parallel_Dijkstra p = new Parallel_Dijkstra();
			p.set_source(point_num);
			Thread myDaemon = new Thread(p.new myDaemon());
	        myDaemon.setDaemon(true);
			for(int i=0;i<task_of_each_point;i++)
				exec.execute(p.new computeTask());
			myDaemon.start();
		}
		 exec.shutdown();
		 while(!exec.isTerminated()){}
	        long endTime = System.currentTimeMillis();
	        System.out.println("Parallel time is "+
					(endTime-startTime)+" milliseconds");
	}
	
	public static void main(String[] args){
		new allvertex_shortestPathwith_Parallel_Dijkstra().get_result();;
	}
}