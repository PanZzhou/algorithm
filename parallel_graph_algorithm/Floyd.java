package my_altorithm;
class Floyd{
	private int[][] arr;
	private final String s = "C:\\Users\\Administrator\\Desktop\\成果\\graph_matrix_1.txt";
	private int size;
	Floyd(){
		read_graph_from_txt txt = new read_graph_from_txt();
		arr = txt.read(s);
		size = arr[0].length;
	}
	public void runFloyd(){
		for(int k=0; k<size; k++){
			for(int i=0; i<size; i++)
				for(int j=0; j<size; j++){
					if(i==j){
						arr[i][j] = 0;
						continue;
					}
					if(arr[i][j]>arr[i][k]+arr[k][j])
						arr[i][j] = arr[i][k] + arr[k][j];
				}
		}
		print_arr();
	}
	public void print_arr(){
		for(int i=0; i<size; i++){
			for(int j=0; j<size; j++)
				if(i!=j)
					System.out.println("节点"+i+" --> 节点"+j+" 的tsp为："+arr[i][j]);
		}
	}
	public static void main(String[] args){
		Floyd f = new Floyd();
		f.runFloyd();
	}
}