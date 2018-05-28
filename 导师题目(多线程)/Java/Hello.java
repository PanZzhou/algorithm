package tea;
class Hello implements Runnable{
	@Override
	public void run() {
		// TODO Auto-generated method stub
		System.out.println(Thread.currentThread().getName()+"：Hello world");
	}
	public static void main(String[] args){
		for(int i=0;i<4;i++)
			new Thread(new Hello(),"线程"+i).start();
	}
}