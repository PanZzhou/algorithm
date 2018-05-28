//全部顶点对间的最短路径，使用流水线Floyd算法实现。
//不用内部类了

package my_altorithm;

class Floyd_stream{
	private int task_count;
	private static Floyd_stream_Runner[] f;
	private Thread t[];
	Floyd_stream(){
		Floyd_stream_Runner.init();
		task_count = Floyd_stream_Runner.cut_count*Floyd_stream_Runner.cut_count;
		f = new Floyd_stream_Runner[task_count];
		t = new Thread[task_count];
		for(int a=0;a<task_count;a++)
			f[a] = new Floyd_stream_Runner();
	}
	public void Floyd_stream_start(){
		for(int a=0;a<task_count;a++){
			t[a] = new Thread(f[a]);
			t[a].start();
		}
	}
	public void Floyd_stream_join() throws InterruptedException{
		for(int a=0;a<task_count;a++)
			t[a].join();
	}
	public static void copy_col_hub(int index1,int index2){
		while(f[index2].col_hub[f[index1].k][0]!=-1)
			Thread.yield();
		for(int a=0;a<f[index1].col_hub[0].length;a++)
			f[index2].col_hub[f[index1].k][a] = f[index1].col_hub[f[index1].k][a];
	}
	public static void copy_row_hub(int index1,int index2){
		while(f[index2].row_hub[f[index1].k][0]!=-1)
			Thread.yield();
		for(int a=0;a<f[index1].row_hub[0].length;a++)
			f[index2].row_hub[f[index1].k][a] = f[index1].row_hub[f[index1].k][a];
	}
	public static void copy_arr_to_row_hub(int index1,int index2){
		for(int a=f[index1].getCol_start(),i=0;a<=f[index1].getCol_end();a++,i++)
			f[index2].row_hub[f[index1].k][i] = Floyd_stream_Runner.arr[f[index1].k][a];
	}
	public static void copy_arr_to_col_hub(int index1,int index2){
		for(int a=f[index1].getRow_start(),i=0;a<=f[index1].getRow_end();a++,i++)
			f[index2].col_hub[f[index1].k][i] = Floyd_stream_Runner.arr[a][f[index1].k];
	}
	public void print_arr(){
		for(int i=0;i<Floyd_stream_Runner.array_size;i++)
			for(int j=0;j<Floyd_stream_Runner.array_size;j++){
				if(i!=j)
					System.out.println("节点"+i+"-->节点"+j+"的SP为"+Floyd_stream_Runner.arr[i][j]);
				else
					System.out.println("节点"+i+"-->节点"+j+"的SP为"+0);
			}
	}
	public static void main(String[] args) throws InterruptedException{
		Floyd_stream F = new Floyd_stream();
		long startTime = System.currentTimeMillis();
		F.Floyd_stream_start();
		F.Floyd_stream_join();
		 long endTime = System.currentTimeMillis();
	        System.out.println("Parallel time is "+
					(endTime-startTime)+" milliseconds");
//		for(int i=0;i<Floyd_stream_Runner.array_size;i++){
//			for(int j=0;j<f[3].getRow_end()-f[3].getRow_start()+1;j++)
//				System.out.print(f[3].col_hub[i][j]);
//			System.out.println();
//		}
//		for(int i=0;i<Floyd_stream_Runner.array_size;i++){
//			for(int j=0;j<f[0].getCol_end()-f[0].getCol_start()+1;j++)
//				System.out.print(f[0].row_hub[i][j]);
//			System.out.println();
//		}
//		F.print_arr();
	}
}