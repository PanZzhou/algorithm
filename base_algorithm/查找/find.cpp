#include <cstdio>
using namespace std;

int binary_search(int *seqlist, int key, int length){
	int low = 0, high = length - 1, mid;
	while(low<=high){//<=非<
		mid = (high + low)/2;//是加号非减号
		printf("%d   %d\n", mid, seqlist[mid]);
		if(seqlist[mid]==key)
			return mid;
		else if(seqlist[mid]>key)
			high=mid-1;//需要减一
		else
			low=mid+1;//需要加一
	}
	return -1;
}

int main(){
	int a[]={1,9,10,111};
	int tag = binary_search(a, 111, sizeof(a)/sizeof(int));
	printf("%d\n", tag);
}