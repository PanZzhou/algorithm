//全部顶点对间的最短路径，使用串行Dijkstra算法，源划分，每个顶点分一个线程执行单源最短路径；
package my_altorithm;

import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

class allvertex_shortestPathwith_Sequential_Dijkstra{
	
	private int tag;  //用于确定每个线程的作用范围
	private int size;  //数组大小
	private int more;   //平均分配顶点时多出来的顶点数，把其分配给前more个线程，每个线程得到多余一个顶点。
	private int p_count = 4;  //创建的线程数量
	allvertex_shortestPathwith_Sequential_Dijkstra(){
		Sequential_Dijkstra.init_arr();
		size = Sequential_Dijkstra.getSize();
		tag = 0;
		more = size%p_count;
	}
	
	public int get_p_count(){
		return p_count;
	}
	
	class run_Sequential_Dijkstra implements Runnable{
		private Sequential_Dijkstra myDijkstra;
		private int start;
		private int end;
		run_Sequential_Dijkstra(){
			get_index();
			myDijkstra = new Sequential_Dijkstra();
		}
		@Override
		public void run() {
			// TODO Auto-generated method stub
			for(int i=start;i<=end;i++){
				myDijkstra.setSource(i);
				myDijkstra.Dijkstra();
			}
		}
		//获取每个线程的作用范围
		private void get_index(){
			start = tag;
			if(more-->0)
				end = (tag + size/p_count)>=size?(size-1):(tag + size/p_count);
			else
				end = (tag -1 + size/p_count)>=size?(size-1):(tag -1 + size/p_count);
			tag = end + 1;
		}
	}
	public static void main(String[] args){
		allvertex_shortestPathwith_Sequential_Dijkstra all = new allvertex_shortestPathwith_Sequential_Dijkstra();
		long startTime = System.currentTimeMillis();
		ExecutorService exec = Executors.newCachedThreadPool();
		for(int k=0;k<all.get_p_count();k++)
			exec.execute(all.new run_Sequential_Dijkstra());
        exec.shutdown();
        while(!exec.isTerminated()){}
        long endTime = System.currentTimeMillis();
        System.out.println("Parallel time is "+
				(endTime-startTime)+" milliseconds");
	}
}