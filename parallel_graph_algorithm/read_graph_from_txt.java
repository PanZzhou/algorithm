package my_altorithm;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;

class read_graph_from_txt{
	private int[][] graph;
	private final int size = 53;
	private int ratio = 53;
	read_graph_from_txt(){
		// 初始化一个用于存储txt数据的数组
	    graph = new int[size][size];
	}
	public int[][] read(String s){
	    int index = 0;
	    int i = 0,j = 0;
	    String[][] rows = new String[size][size];
	    BufferedReader br = null;
	    try {
	       // 读文件了. 路径就是那个txt文件路径
	       br  = new BufferedReader(new FileReader(new File(s)));
	        String str = null;
	        // 按行读取
	        while((str=br.readLine())!=null){
	        // 可能两个数字之间的空格数不固定,可以是n个.
	            rows[index] = str.split("	");
	            index++;
	        }
	//将String数组转换成int数组
	        for (String[] strings : rows) {
	            for (String string : strings) {
	            	graph[i][j] = (int) Double.parseDouble(string);
	                j++;
	                if(j>=size)
	                	j = 0;
	            }
	            i++;
	            if(i>=size)
	            	i = 0;
	        }
	        if(ratio == 1){
	        	 System.out.println("节点个数："+graph[0].length+"\n\n");
	        	 return graph;
	        }
	        int[][] graph1 = new int[ratio*size][ratio*size];
	        for(int m=0,a=0;m<ratio*size;m++){
	        	for(int n=0,b=0;n<ratio*size;n++){
	        		graph1[m][n] = graph[a][b];
	        		b++;
	        		if(b>=size) 
	        			b = 0;
	        	}
	        	a++;
	        	if(a>=size)
	        		a = 0;
	        }
	        System.out.println("节点个数："+graph1[0].length+"\n\n");
	        return graph1;
	                                             
	    } catch (FileNotFoundException e) {
	        // TODO Auto-generated catch block
	        e.printStackTrace();
	    } catch (IOException e) {
	        // TODO Auto-generated catch block
	        e.printStackTrace();
	    }
        return graph;
	}

//	public static void main(String[] args){
//		read_graph_from_txt txt = new read_graph_from_txt();
//		txt.read();
//	}
}