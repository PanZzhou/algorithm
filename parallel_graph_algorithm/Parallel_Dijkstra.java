//单源最短路径dijkstra算法并行形式
package my_altorithm;

import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

class Parallel_Dijkstra {
    private int tag = 0;  //用于标记每个线程从哪里开始及放置buffer[k],其中worth[buffer[k]]为worth数组中最小的值
    private int value;   //用于设置worth[tag]=0前worth[tag]的值
    private Object obj = new Object();  //用于加锁
    private Object obj1 = new Object();  //用于加锁
    
    private static int p_count = 3;  //线程个数
    private int task_count;  //正在执行线程个数
    private int[] buffer; 
    private static int size;  //数组大小
    private boolean is_statute[]; //线程i是否规约完毕
    private int p_count_except_0;  //标记所有线程是否查找完毕，完毕时值为零；
    
    private int source; //开始计算的源点
    private static int N=10000;//定义在类里且加上static，则此变量属于整个类，任何一个实例改变它，则对其他实例可见（已改变）
    private static String s = "C:\\Users\\Administrator\\Desktop\\成果\\graph_matrix_1.txt";
	private static int[][] arr;
	private int[] worth;//记录已标记节点组成的集合到其他点的最小距离
	private int more;

	Parallel_Dijkstra(){
    	init();
    }
	public static void init_static_items(){
		read_graph_from_txt txt = new read_graph_from_txt();
		arr = txt.read(s);
		size = arr[0].length;
	}
	public static int get_size(){
		return size;
	}
	public void init(){
		p_count_except_0 = p_count;
		worth = new int[size];
		buffer = new int[p_count];
		is_statute = new boolean[p_count];
		more = size%p_count;
		task_count = 0;
		source = 0;
	}
	public static void set_p_count(int count){
		p_count = count;
	}
	public void set_source(int source){
		this.source = source;
	}
	
	//计算线程。
	public class computeTask implements Runnable{
		private int start,end;//每个线程都有start和end，用于标记范围；
		private int index; //标记线程是第几个
		computeTask(){
			get_index();
		}

		@Override
		public void run() {
			// TODO Auto-generated method stub
			int i = start;
			while(i<=end){
				if(i==source)
					worth[i] = 0;
				else 
					worth[i] = arr[source][i];
				i++;
			}
			while(true){
				i = find_min_worth();
//				等待规约
				synchronized(obj1){
					p_count_except_0 = p_count_except_0 - 1;
					//如果返回的点已经被访问过，跳出循环
					if(worth[i]==0){
						--task_count;
						buffer[index] = -1;
						if(p_count_except_0==0)
							obj1.notify();
						break;
					}
					if(p_count_except_0==0)
						obj1.notify();
				}
				//给对应的buffer赋值数值最小的坐标
				buffer[index] = i;
				synchronized(obj){
					while(!is_statute[index]){
						try {
							obj.wait();
						} catch (InterruptedException e) {
							e.printStackTrace();
						}
					}
					//线程设置is_statute[i]为false
					is_statute[index] = false;
				}
				//线程更新worth数组的对应位置
				for(int k = start;k<=end;k++){
					if(k!=tag&&worth[k]!=0&&worth[k]>value+arr[tag][k])
						worth[k] = arr[tag][k]+value;
				}
			}
		}
		//获取每个线程的作用范围
		private void get_index(){
			synchronized(obj){
				index = task_count++;
				start = tag;
				if(more-->0)
					end = (tag + size/p_count)>=size?(size-1):(tag + size/p_count);
				else
					end = (tag -1 + size/p_count)>=size?(size-1):(tag -1 + size/p_count);
				tag = end + 1;
			}
		}
		
		private int find_min_worth(){
			int i = start,j=start;
			int flag = N;
			int k = start;
			boolean has_no_zero = false;//是否存在除零以外的数
			while(i <= end){
				if(worth[i]!=0&&worth[i]<flag){
					flag = worth[i];
					j = i;
				}
				if(!has_no_zero&&worth[i]!=0){
					has_no_zero = true;
					if(worth[i]==N)
						k = i;
				}
				i++;
			}
			if(has_no_zero&&worth[j]!=0&&worth[j]!=N)
				return j;
			return k;
		}
	}
	
	public class myDaemon implements Runnable{
		myDaemon(){}
		@Override
		public void run() {
//			开始执行位置
//			long startTime = System.currentTimeMillis();
			while(task_count!=0){
				//线程0进行规约及广播操作
				synchronized(obj1){
					while(p_count_except_0!=0){
						try {
							obj1.wait();
						} catch (InterruptedException e) {
							e.printStackTrace();
						}
					}
					p_count_except_0 = task_count;
				}
				//把符合条件的最小的权值的下标存入tag中
				get_tag();
				value = worth[tag];
				if(value!=0)
					System.out.println("节点"+source+"-->"+"节点"+tag+"  权值为"+value);
				worth[tag] = 0;//表示此顶点已经访问；
				synchronized(obj){
					for(int i=0; i<p_count; i++)
						is_statute[i] = true;
					obj.notifyAll();
				}
			}
			//执行完位置
//	        long endTime = System.currentTimeMillis();
//			System.out.println("Parallel time is "+
//					(endTime-startTime)+" milliseconds");
		}
		
		private void get_tag(){
			boolean first_choose = true;
			for(int k=0;k<p_count;k++){
				if(buffer[k]!=-1&&worth[buffer[k]]!=0){
					if(first_choose){
						tag = buffer[k];
						first_choose = false;
						continue;
					}
					if(worth[tag]>worth[buffer[k]])
						tag = buffer[k];
				}
			}
		}
	}
	
	
	public static void main(String[] args) throws InterruptedException{
		Parallel_Dijkstra.init_static_items();
		Parallel_Dijkstra d = new Parallel_Dijkstra();
		d.set_source(0);
		Thread myDaemon = new Thread(d.new myDaemon());
        myDaemon.setDaemon(true);
		ExecutorService exec = Executors.newCachedThreadPool();
		for(int k=0;k<p_count;k++)
			exec.execute(d.new computeTask());
        myDaemon.start();
        exec.shutdown();
	}
}