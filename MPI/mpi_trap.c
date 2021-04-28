#include <mpi.h>
#include <stdio.h>
#include <math.h>
double trap(double x){
	return x;
}

int main(int argc,char *argv[]){
	int myid, numprocs;
	double low=0,high=5;  //求解范围
	int area_num = 10000;  //分段的个数
	double res = 0, local = 0;  //结果
	double area_len, self_low, self_high;
	int self_area_num;
	MPI_Init(&argc,&argv);
	MPI_Comm_rank(MPI_COMM_WORLD,&myid);
	MPI_Comm_size(MPI_COMM_WORLD,&numprocs);
	
	area_len = (high-low)/area_num;  //每个分段的长度
	self_area_num = ceil(area_num/numprocs); //每个进程分到的分段个数，某个个线程可能小于这个值；向上取整
	self_low = self_area_num*area_len*myid+low;
	self_high = self_low+self_area_num*area_len;
	self_high = self_high>high?high:self_high;//最后一个进程可能会超过范围。
	
	printf("进程%d的范围是%f到%f\n",myid,self_low,self_high);
	//计算过程
	local = (trap(self_low)+trap(self_high))/2;
	double x;
	for(x=self_low+area_len;x<self_high;x=x+area_len){
		local+=trap(x);
	}
	local*=area_len;
	//通信过程
	if(myid!=0){
		//非0进程发送本地结果到0进程进行统计
		MPI_Send(&local,1,MPI_DOUBLE,0,0,MPI_COMM_WORLD);
	}else{
		//0进程统计结果
		res+=local;  //先加上本地的结果
		int i;
		for(i=1;i<numprocs;i++){
			MPI_Recv(&local,1,MPI_DOUBLE,i,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
			res+=local;
		}
		printf("结果为%f\n",res);
	}
	

	MPI_Finalize();
	return 0;
}
