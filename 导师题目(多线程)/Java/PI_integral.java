package tea;

class PI_integral implements Runnable{
    private long start;
    private long end;
    private long rank;
    private static int Thread_num = 10;
    private static long fragment = 800000000;
    private static double sum = 0.0;
    PI_integral(int temp){
    	rank = temp;
    	long range = fragment/Thread_num;
    	start = rank*range;
    	end = start+range;
    }
	@Override
	public void run() {
		// TODO Auto-generated method stub
		double delta = 1.0/fragment;
		double my_sum = 0.0;
		for(long i=start;i<end;i++){
			double x = (double)i/fragment;
			my_sum += delta/(1.0+x*x); 
		}
		my_sum*=4;
		synchronized(getClass()){
			sum += my_sum;
		}
	}
	public static void main(String[] args) throws InterruptedException{
		Thread[] t = new Thread[Thread_num];
		for(int i=0;i<Thread_num;i++){
			t[i] = new Thread(new PI_integral(i));
			t[i].start();
		}
		for(int i=0;i<Thread_num;i++)
			t[i].join();
		System.out.println(sum);
	}
}