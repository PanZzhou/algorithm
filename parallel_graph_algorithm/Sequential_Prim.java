//最小生成树Prim算法串行形式
package my_altorithm;
class Sequential_Prim{
    private  int N = 10000;//定义在类里且加上static，则此变量属于整个类，任何一个实例改变它，则对其他实例可见
    private String s = "C:\\Users\\Administrator\\Desktop\\成果\\graph_matrix_1.txt";
	private int[][] arr;
	private int[] worth;//记录已标记节点组成的集合到其他点的最小距离
	private int[] mst;//记录到每个节点的最小代价的源节点
	//初始化worth数组
	Sequential_Prim(){
		init();
	}
	//把数组temp1拷贝到数组temp2中
	public void copy(int[] temp1,int[] temp2){
		int size = temp1.length;
		while(--size>=0){
			temp2[size] = temp1[size];
		}
	}
	public void Prim(){
		copy(arr[0],worth);
		worth[0] = 0;
		int j = 0,sum = 0;
		while(no_done()){
		    j = find_min_worth();
		    System.out.println("节点"+mst[j]+"-->"+"节点"+j+"  权值为"+worth[j]);
		    sum += worth[j];
		    update_worth(j);
		}
		System.out.println("\n总的权值 sum = "+sum);
	}
	private boolean no_done(){
		for(int i=0;i<worth.length;i++){
			if(worth[i]!=0)
				return true;
		}
		return false;
	}
	private int find_min_worth(){
		int i = 0,j=0;
		int flag = N;
		while(i<worth.length){
			if(worth[i]!=0&&worth[i]<flag){
				flag = worth[i];
				j = i;
			}
			i++;
		}
		return j;
	}
	private void update_worth(int j){
		for(int i=0;i<arr[0].length;i++){
			if(i!=j&&worth[i]!=0&&arr[j][i]<worth[i]){
				worth[i] = arr[j][i];
				mst[i] = j;
			}
		}
		worth[j] = 0;
	}
	private void init(){
		read_graph_from_txt txt = new read_graph_from_txt();
		arr = txt.read(s);
		int size = arr[0].length;
		worth = new int[size];
		mst = new int[size];
	}

	public static void main(String[] args){
		System.out.println("\n\nSequential prim algorithm:\n\n");
		Sequential_Prim p = new Sequential_Prim();
		long startTime = System.currentTimeMillis();
		p.Prim();
		long endTime = System.currentTimeMillis();
		System.out.println("Sequential time is "+
				(endTime-startTime)+" milliseconds");
	}
}
