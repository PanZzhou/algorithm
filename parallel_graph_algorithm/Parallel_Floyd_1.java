//全部顶点对间的最短路径，使用二维映射Floyd算法实现。
package my_altorithm;

import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

class Parallel_Floyd_1{
	private Object obj = new Object();//用于加锁同步
	private Object obj1 = new Object();//用于加锁同步
	private int array_size;   //数组大小
	private static int cut_count = 2; //横向或者纵向切割数量，线程数目2次幂
	private final String s = "C:\\Users\\Administrator\\Desktop\\成果\\graph_matrix_1.txt";
	private int start;  //列开始位置
	private int end; //列结束位置
	private int row = 0;
	private int col = 0;
	private int more;  //余数
	private int row_index; 
	private int col_index;
	private int thread_index;
	private int k;  //循环次数
	private int copied_num;
	private int dealed_num;
	private boolean[] copy;
	private boolean[] deal;
	
	private static int[][] arr;  //存储邻接矩阵
	private int[] col_arr; //存储k列的数组
	private int[] row_arr; //存储k行的数组
	Parallel_Floyd_1(){
		init();
	}
	
	private class Runner implements Runnable{
		private int row_start;  //行开始位置
		private int row_end;  //行结束位置
		private int col_start;  //列开始位置
		private int col_end; //列结束位置
		private int index;  //线程顺序
		Runner(){
			get_index();
		}
		private void set_contition(boolean[] arr,boolean a){
			for(int b=0;b<arr.length;b++)
				arr[b] = a;
		}
		@Override
		public void run() {
			while(k<array_size){
				//若第k行位于本线程映射的区域，则复制这部分数据到row_arr数组中
				if(k>=row_start&&k<=row_end){
					for(int a=col_start;a<=col_end;a++)
						row_arr[a] = arr[k][a];
					synchronized(obj){
						copied_num++;
						if(copied_num==2*cut_count){
							copied_num = 0;
							set_contition(copy,true);
							obj.notifyAll();
						}
					}
				}
				//若第k列位于本线程映射的区域，则复制这部分数据到row_arr数组中
				if(k>=col_start&&k<=col_end){
					for(int a=row_start;a<=row_end;a++)
						col_arr[a] = arr[a][k];
					synchronized(obj){
						copied_num++;
						if(copied_num==2*cut_count){
							copied_num = 0;
							set_contition(copy,true);
							obj.notifyAll();
						}
					}
				}
				synchronized(obj){
					while(!copy[index]){
						try {
							obj.wait();
						} catch (InterruptedException e) {
							e.printStackTrace();
						}
					}
					copy[index] = false;
				}
				//处理本区域的数据
				for(int a=row_start;a<=row_end;a++){
					for(int b=col_start;b<=col_end;b++){
						if((a!=k||b!=k)&&arr[a][b]>col_arr[a]+row_arr[b])
							arr[a][b] = col_arr[a] + row_arr[b];
						if(a==b)
							arr[a][b] = 0;
					}
				}
//				System.out.println("deal finished");
				synchronized(obj1){
					dealed_num++;
					if(dealed_num==cut_count*cut_count){
						dealed_num = 0;
						k++;
						if(k==array_size)
							print_arr();
						set_contition(deal,true);
						obj1.notifyAll();
					}
				}
				synchronized(obj1){
					while(!deal[index]){
						try {
							obj1.wait();
						} catch (InterruptedException e) {
							// TODO Auto-generated catch block
							e.printStackTrace();
						}
					}
					deal[index] = false;
				}
			}
		}
		
		private void print_arr() {
			// TODO Auto-generated method stub
			for(int a=0;a<array_size;a++){
				for(int b=0;b<array_size;b++)
					System.out.println("节点"+a+"--> 节点"+b+" 的SP为 "+arr[a][b]);
			}
		}
		//获取每个线程的作用范围,若放在构造函数里的话，可不用同步语句，因main中构造函数按顺序调用；
		private void get_index(){
			    index = thread_index++;
//			    System.out.println(index);
//			synchronized(obj){    
				//确定行的起始位置
				if(col!=0){
					col_start = start;
					col_end = end;
				}
				row_start = row;
				if(more-row_index++>0)
					row_end = (row + array_size/cut_count)>=array_size?(array_size-1):(row + array_size/cut_count);
				else
					row_end = (row -1 + array_size/cut_count)>=array_size?(array_size-1):(row -1 + array_size/cut_count);
				if(row_end>=array_size-1||col==0){
					if(row_end>=array_size-1){
						row = 0;
						row_index = 0;
					}
					start = col;
					if(more-col_index++>0)
						end = (col + array_size/cut_count)>=array_size?(array_size-1):(col + array_size/cut_count);
					else
						end = (col -1 + array_size/cut_count)>=array_size?(array_size-1):(col -1 + array_size/cut_count);
				    //确定列的起始位置
					if(col==0){
						col_start = start;
						col_end = end;
					}
					col = end + 1;
				}
				if(row_end<array_size-1)
					row = row_end + 1;
//			}
//			System.out.println(index+"::"+row_start+" "+row_end+" "+col_start+" "+col_end);
		}
		
	}
	private void init(){
		read_graph_from_txt txt = new read_graph_from_txt();
		arr = txt.read(s);
		array_size = arr[0].length;
		col_arr = new int[array_size];
		row_arr = new int[array_size];
		copy = new boolean[cut_count*cut_count];
		deal = new boolean[cut_count*cut_count];
		more = array_size%cut_count;
		row_index = 0;
		col_index = 0;
		k = 0;
		thread_index = 0;
		dealed_num = 0;
		copied_num = 0;
	}
	public static void main(String[] args) throws InterruptedException{
		Parallel_Floyd_1 F = new Parallel_Floyd_1();
		long startTime = System.currentTimeMillis();
		ExecutorService exec = Executors.newCachedThreadPool();
		for(int k=0;k<cut_count*cut_count;k++)
			exec.execute(F.new Runner());
        exec.shutdown();
        while(!exec.isTerminated()){}
        long endTime = System.currentTimeMillis();
        System.out.println("Parallel time is "+
				(endTime-startTime)+" milliseconds");
	}
}