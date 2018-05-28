package tea;

import java.util.Random;
import java.util.concurrent.locks.Condition;
import java.util.concurrent.locks.Lock;
import java.util.concurrent.locks.ReentrantLock;

class producer_consumer{
	private int buffer;
	private int loop = 100;
	private Lock lock;
	private Condition is_odd;
	private Condition is_even;
	private Condition done;
	private boolean put_odd;
	private boolean put_even;
	private boolean get;
	private boolean finishwithodd;
	private boolean finishwitheven;
	producer_consumer(){
		lock = new ReentrantLock();
		is_odd = lock.newCondition();
		is_even = lock.newCondition();
		done = lock.newCondition();
		put_odd = false;
		put_even = false;
		get = true;
		finishwithodd = false;
		finishwitheven = false;
	}
	private class producer implements Runnable{
		private Random random;
		producer(){
			random = new Random();
		}
		@Override
		public void run() {
			// TODO Auto-generated method stub
			int i=0;
			while(i++<loop){
				lock.lock();
				try {
					while(!get)
						done.await();
					get = false;
					buffer = random.nextInt(100)+1;
					System.out.println("producer puts: "+buffer);
					if(buffer%2==0){
						put_even = true;
						is_even.signal();
						if(i==loop){
							finishwitheven = true;
							put_odd = true;
							is_odd.signal();
						}
					}
					else{
						put_odd = true;
						is_odd.signal();
						if(i==loop){
							finishwithodd = true;
							put_even = true;
							is_even.signal();
						}
					}
				} catch (InterruptedException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}finally{
					lock.unlock();
				}
			}
		}
	}
	private class consumer_odd implements Runnable{
		@Override
		public void run() {
			// TODO Auto-generated method stub
			while(true){
				lock.lock();
				try {
					while(!put_odd)
						is_odd.await();
					if(finishwitheven) break;
					put_odd = false;
					System.out.println("consumer_odd get: "+buffer);
					if(finishwithodd) break;
					get = true;
					done.signal();
				} catch (InterruptedException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}finally{
					lock.unlock();
				}
			}
		}
		
	}
	private class consumer_even implements Runnable{
		@Override
		public void run() {
			// TODO Auto-generated method stub
			while(true){
				lock.lock();
				try {
					while(!put_even)
						is_even.await();
					if(finishwithodd) break;
					put_even = false;
					System.out.println("consumer_even get: "+buffer);
					if(finishwitheven) break;
					get = true;
					done.signal();
				} catch (InterruptedException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}finally{
					lock.unlock();
				}
			}
		}
		
	}
	public static void main(String[] args){
		producer_consumer p_c = new producer_consumer();
		new Thread(p_c.new producer()).start();
		new Thread(p_c.new consumer_odd()).start();
		new Thread(p_c.new consumer_even()).start();
	}
}