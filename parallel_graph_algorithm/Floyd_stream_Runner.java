package my_altorithm;
class Floyd_stream_Runner implements Runnable{
	public static int array_size;   //数组大小
	public static int cut_count = 2; //横向或者纵向切割数量，线程数目2次幂
	public static final String s = "C:\\Users\\Administrator\\Desktop\\成果\\graph_matrix_1.txt";
	//一系列数组用于同步和计算；
	public static int[][] arr;  //存储邻接矩阵
	
	private static int start;  //列开始位置
	private static int end; //列结束位置
	private static int row = 0;
	private static int col = 0;
	private static int more;  //余数
	private static int row_index; 
	private static int col_index;
	private static int thread_index;
	
	private int row_start;  //行开始位置
	private int row_end;  //行结束位置
	private int col_start;  //列开始位置
	int col_end; //列结束位置
	private int index;  //线程顺序
	private int row_id; //行号  从零开始
	private int col_id; //列号 从零开始
	
	//存储迭代需要的信息
    int[][] col_hub; //存储列的信息
	int[][] row_hub; //存储行的信息
	
	int k;  //迭代次数
	
	Floyd_stream_Runner(){
		get_index();
		get_col_row_id();
		col_hub = new int[array_size][getRow_end()-getRow_start()+1];
		row_hub = new int[array_size][getCol_end()-getCol_start()+1];
		set_hub(col_hub);
		set_hub(row_hub);
		k = 0;
		System.out.println(index+"::"+getRow_start()+" "+getRow_end()+" "+getCol_start()+" "+col_end+" "+row_id+" "+col_id);
	}
	@Override
	public void run() {
		while(k<array_size){
			System.out.println("我是线程"+index+" "+k+"次运行");
			//若k行在线程内
			if(k>=getRow_start()&&k<=getRow_end()){
				//拷贝信息到自己的区域
				Floyd_stream.copy_arr_to_row_hub(index,index);
				if(row_id-1>=0)
					Floyd_stream.copy_arr_to_row_hub(index,index-1);
				if(row_id+1<cut_count)
					Floyd_stream.copy_arr_to_row_hub(index,index+1);
			}
			//若k列在线程内
			if(k>=getCol_start()&&k<=col_end){
				Floyd_stream.copy_arr_to_col_hub(index,index);
				if(col_id-1>=0)
					Floyd_stream.copy_arr_to_col_hub(index,index-cut_count);
				if(col_id+1<cut_count)
					Floyd_stream.copy_arr_to_col_hub(index,index+cut_count);
			}
			//不满足则等待
			while(row_hub[k][0]==-1||col_hub[k][0]==-1)
				Thread.yield();
			//条件满足，更新线程负责的区域
			for(int i=getRow_start(),m=0;i<=getRow_end();i++,m++){
				for(int j=getCol_start(),n=0;j<=getCol_end();j++,n++){
					if(i==k||j==k)
						continue;
					if(arr[i][j]>row_hub[k][n]+col_hub[k][m])
						arr[i][j] = row_hub[k][n]+col_hub[k][m];
				}
			}
			//把收到的数据发送给下一个线程
			//若k行小于范围内，向下一个线程拷贝数据
			if(k<row_start&&row_id+1<cut_count)
				Floyd_stream.copy_row_hub(index,index+1);
			//若k行大于范围，向上面一个线程拷贝数据；
			if(k>row_end&&row_id-1>=0)
				Floyd_stream.copy_row_hub(index, index-1);
			//若k列小于范围内，向右一个线程拷贝数据
			if(k<col_start&&col_id+1<cut_count)
				Floyd_stream.copy_col_hub(index,index+cut_count);
			//若k列大于范围内，向左一个线程拷贝数据
			if(k>col_end&&col_id-1>=0)
				Floyd_stream.copy_col_hub(index,index-cut_count);
			//设置自己的状态为未接收数据
			row_hub[k][0] = -1;
			col_hub[k][0] = -1;
			k++;
		}
	}
	
	//把数组hub设为元素为-1的数组
	private void set_hub(int[][] hub){
		for(int a=0;a<hub.length;a++){
			for(int b=0;b<hub[0].length;b++)
				hub[a][b] = -1;
		}
	}
	public static void init(){
		read_graph_from_txt txt = new read_graph_from_txt();
		arr = txt.read(s);
		array_size = arr[0].length;
		more = array_size%cut_count;
		row_index = 0;
		col_index = 0;
		thread_index = 0;
	}
	
	//获取每个线程的作用范围,若放在构造函数里的话，可不用同步语句，因main中构造函数按顺序调用；
	private void get_index(){
		    index = thread_index++;
			//确定行的起始位置
			if(col!=0){
				setCol_start(start);
				col_end = end;
			}
			setRow_start(row);
			if(more-row_index++>0)
				setRow_end((row + array_size/cut_count)>=array_size?(array_size-1):(row + array_size/cut_count));
			else
				setRow_end((row -1 + array_size/cut_count)>=array_size?(array_size-1):(row -1 + array_size/cut_count));
			if(getRow_end()>=array_size-1||col==0){
				if(getRow_end()>=array_size-1){
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
					setCol_start(start);
					col_end = end;
				}
				col = end + 1;
			}
			if(getRow_end()<array_size-1)
				row = getRow_end() + 1;
//		}
//		System.out.println(index+"::"+row_start+" "+row_end+" "+col_start+" "+col_end);
	}
	
	private void get_col_row_id(){
		int temp_1 = (index+1)%cut_count;
		int temp_2 = (index+1)/cut_count;
		if(temp_1==0){
			col_id = temp_2 - 1;
			row_id = cut_count - 1;
		}else{
			col_id = temp_2;
			row_id = temp_1 - 1;
		}
	}
	public int getCol_start() {
		return col_start;
	}
	public int getCol_end() {
		return col_end;
	}
	private void setCol_start(int start2) {
		this.col_start = start2;
	}
	public int getRow_start() {
		return row_start;
	}
	public void setRow_start(int row_start) {
		this.row_start = row_start;
	}
	public int getRow_end() {
		return row_end;
	}
	public void setRow_end(int row_end) {
		this.row_end = row_end;
	}

}