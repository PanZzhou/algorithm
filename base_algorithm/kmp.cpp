#include <cstdio>
using namespace std;
//next[j]可以表示为与j位置不匹配的元素下标
void get_next1(int *next, char *mattch, int len){
	if(len<1) return;
	next[0] = -1;//guard守卫
	int i = 0, j = -1;//j指向的位置可以表示前缀后缀最大重合长度
	while(i<len){
		if(j==-1||mattch[i]==mattch[j]){
			++i;//向前推进一个位置来确定下一个位置的值
			++j;//匹配的元素个数实际比j大1，故加1;同时可以进入指向下一个元素从而进入下一个循环
			next[i] = j;//j的值实时确定匹配元素个数
		}
		else j=next[j];//定位到第一个不匹配的元素上(匹配元素个数即为第一个不匹配元素的下标)
	}
}

int index_kmp(char *pattern, int pat_len, char *mattch, int mat_len, int *next){
	int i = 0, j = 0;
	while(i<pat_len&&j<mat_len){
		if(j==-1||mattch[j]==pattern[i]){
			++i;
			++j;
		}else j=next[j];
	}
	if(j>=mat_len)
		return i - mat_len;
	else
		return -1;
}

int main(){
	char pattern[] = {'a','b','c','a','c','b','c','d','a','b','c','a','b','c','d','e'};
	char mattch[] = {'a','b','c','a','b','c','d'};
	int pat_len = sizeof(pattern)/sizeof(char);
	int mat_len = sizeof(mattch)/sizeof(char);
	int next[mat_len];
	get_next1(next, mattch, mat_len);
	int pos = index_kmp(pattern,pat_len, mattch, mat_len, next);
	for(int i=0; i<mat_len; i++)
		printf("%d ", next[i]);
	printf("\n%d\n", pos);
}