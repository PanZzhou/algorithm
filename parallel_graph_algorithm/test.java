package my_altorithm;
class test{
	private static int size = 2;
	public static void main(String[] args){
		read_graph_from_txt txt = new read_graph_from_txt();
		int[][] graph = new int[size][size];
		graph = txt.read("C:\\Users\\Administrator\\Desktop\\成果\\graph_matrix_1.txt");
//		for(int[] a:graph){
//			for(int b:a)
//				System.out.print(b+" ");
//			System.out.println();
//		}
	}
}