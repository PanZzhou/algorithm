#include <stdio.h>
#include <stdlib.h>
#include <string.h>

using namespace std;
//#########################################插入类排序##################################(s[0]不放元素)
//直接插入排序,最好(有序时)：O(n), 最坏或平均：O(n2),稳定
void insert_sort(int *s, int len){
	int i,j;
	for(i=2;i<len;i++){
		if(s[i]<s[i-1]){
			s[0] = s[i];//设置哨兵，0号位置不存元素
			for(j=i-1;s[0]<s[j];j--)
				s[j+1]=s[j];
			s[j+1]=s[0]; //j+1
		}
	}
}

//折半插入排序，O(n2),稳定(比直接插入排序只是减少了元素比较次数)
void half_insert_sort(int *s, int len){
	int i,j,low,high,mid;
	for(i=2;i<len;i++){
		if(s[i]<s[i-1]){
			s[0]=s[i];
			high=i-1;
			low=1;
			//折半查找待插入位置，等于s[0]的情况归为小于s[0]
			while(low<=high){
				mid=(high+low)/2;
				if(s[mid]>s[0])
					high=mid-1;
				else low=mid+1;
			}
			for(j=i-1;j>=high+1;j--)
				s[j]=s[j-1];
			s[high+1]=s[0];//插入操作
		}
	}
}
//希尔排序，平均：O(n1.3)，最坏：O(n2),不稳定，不适用于链式存储
void shell_sort(int *s, int len){
	int i,j,dk;
	//初始步长为数组长度一般（向下取整），最短步长为1，每次减少一次步长
	for(dk=len/2;dk>=1;dk=dk/2){
		for(i=dk+1;i<len;i++){
			if(s[i]<s[i-dk]){//s[i-dk]而非s[i-dk+1],因i初始值为dk+1,最左端的元素是从下标1开始
				s[0]=s[i];//s[0]不存放元素
				for(j=i-dk;j>=0&&s[j]>s[0];j-=dk)
					s[j+dk]=s[j];
				s[j+dk]=s[0];
			}
		}
	}
}
//#########################################选择类排序##################################
//选择排序，O(n2),不稳定
void select_sort(int *s, int len){
	int i,j,temp, min;
	for(i=0;i<len-1;i++){
		min=i;
		for(j=i+1;j<len;j++)
			if(s[j]<s[min])
				min=j;
		if(min!=i){
			temp=s[i];
			s[i]=s[min];
			s[min]=temp;
		}
	}
}

//堆排序,最好平均最差都为O(nlogn), 建堆为O(n), 不稳定
void adjust_down(int *s, int k, int len){
	int temp=s[k-1];//k为第k个元素，k-1为下标
	for(int i=k*2;i<=len;i*=2){
		if(i<len&&s[i-1]<s[i]) ++i;
		if(temp>=s[i-1]) break;
		else{
			s[k-1]=s[i-1];
			k=i;
		}
	}
	s[k-1]=temp;
}

//建堆
void build_heap(int *s, int len){
	for(int i=len/2;i>0;i--)
		adjust_down(s,i,len);
}
//调堆
void heap_sort(int *s, int len){
	int temp;
	build_heap(s, len);
	for(int i=len;i>1;i--){//轮流取堆顶后调堆
		temp=s[0];
		s[0]=s[i-1];
		s[i-1]=temp;
		adjust_down(s,1,i-1);
	}
}

//#########################################交换类排序##################################
//冒泡排序，最好(已排好序):O(n),最坏(平均)：O(n2),稳定
void bubble_sort(int *s, int len){
	int temp,i,j;
	bool flag=false;
	for(i=0;i<len-1;i++){
		flag=false;
		for(j=len-1;j>i;j--){
			if(s[j]<s[j-1]){
				temp=s[j];
				s[j]=s[j-1];
				s[j-1]=temp;
				flag=true;
			}
		}
		if(!flag)//一遍遍历后发现有序，直接返回
			return;
	}
}

//快速排序,最好(无序时)：O(nlogn), 最坏(有序时):O(n2),不稳定；      空间复杂度:最好O(logn)，最坏:O(n)
void quick_sort(int *s, int low, int high){
	if(low<high){
		int pivot = s[low],i=low,j=high;
		while(low<high){
			while(low<high&&s[high]>=pivot) high--;
			s[low]=s[high];
			while(low<high&&s[low]<=pivot) low++;
			s[high]=s[low];
		}
		s[low]=pivot;
		quick_sort(s,i,low-1);
		quick_sort(s,low+1,j);
	}
}
//#########################################其他类排序##################################
//归并范围小于某一阈值时使用插入排序
void merge_insert_sort(int *s, int low, int high){
	int i,j,temp;
	for(i=low+1;i<=high;i++){
		if(s[i]<s[i-1]){
			temp = s[i];
			for(j=i-1;j>=low&&temp<s[j];j--)
				s[j+1]=s[j];
			s[j+1]=temp;
		}
	}
}
//归并排序, 时间：O(nlogn)，空间：O(n),稳定
void merge(int *s, int low, int mid, int high, int *t){
	int i,j,k;
	// for(i=low;i<=high;i++) t[i]=s[i];//优化前要复制数组
	for(i=low, j=mid+1, k=low;i<=mid&&j<=high;k++){
		if(t[i]<=t[j])
			s[k]=t[i++];
		else
			s[k]=t[j++];
	}
	while(i<=mid) s[k++]=t[i++];
	while(j<=high) s[k++]=t[j++];
}
//两个优化点：数组拷贝和小于某一值时使用插入排序
void merge_sort(int *s, int low, int high, int *t){
	if(low+2>=high){//优化后，长度小于3个元素时使用插入排序(一般为7个)
		merge_insert_sort(s, low, high);
		return;
	}else if(low<high){
		int mid=(low+high)>>1;
		// merge_sort(s, low, mid, t); //优化前，s与t的位置互换从而不用数组拷贝
		// merge_sort(s, mid+1, high, t); //优化前，s与t的位置互换从而不用数组拷贝
		merge_sort(t, low, mid, s); //优化后
		merge_sort(t, mid+1, high, s); //优化后
		merge(s, low, mid, high, t);
	}
}
void merge_sort(int *s, int len){
	int *t=(int*)malloc(sizeof(int)*len);
	memcpy(t,s,len*sizeof(int));
	merge_sort(s, 0, len-1,t);
	free(t);
	t=nullptr;
}

//总结
//不稳定：选择，堆，希尔，快排
//有序数列：快的选择：插入，冒泡；  不好的选择：快排(O(n2))
//无序数列：快排
//空间：快排:O(n)或O(logn), 归并：O(n), 其他为O(1)
int main(){
	int s[]={2,5,3,5,3,8,67,6,5,39,34,23,22,11,38,77,56,24,88,36,32,1,4,23,53,32,23,44,11,2};
	int len=sizeof(s)/sizeof(int);
	printf("%d\n", len);
	// quick_sort(s,0,len-1);
	// heap_sort(s,len);
	merge_sort(s, len);
	for (int i = 0; i < len; ++i)
	{
		printf("%d ", s[i]);
	}
}