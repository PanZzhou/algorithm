package tea;

import java.util.Random;

class PI_monte implements Runnable{
    private static int num = 0;
    private static int Thread_num = 10;
    private static int loop = 1000000;
    private Random random;
    PI_monte(){
    	random = new Random();
    }
	@Override
	public void run() {
		// TODO Auto-generated method stub
		float floatnumber_1;
		float floatnumber_2;
		int k = 0;
		while(k++<loop){
			floatnumber_1 = random.nextFloat();
			floatnumber_2 = random.nextFloat();
			if(hit(floatnumber_1,floatnumber_2)){
				synchronized(getClass()){
					num++;
				}
			}
		}
	}
    private boolean hit(float x,float y){
    	if(x*x+y*y<=1)
    		return true;
    	return false;
    }
	public static void main(String[] args) throws InterruptedException{
		Thread[] t = new Thread[Thread_num];
		for(int i=0;i<Thread_num;i++){
			t[i] = new Thread(new PI_monte());
			t[i].start();
		}
		for(int i=0;i<Thread_num;i++)
			t[i].join();
		System.out.println(num*4.0/(Thread_num*loop));
	}
}