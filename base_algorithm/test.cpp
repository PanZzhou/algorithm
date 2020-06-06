#include<iostream>
#include<cstdlib>
 
using namespace std;
 
//交换a,b的值，注意如果漏掉&，结果不正确
void swap(int &a, int &b)
{
	int temp = a;
	a = b;
	b = temp;
}
 
//分区  若a[j]<基数，则将a[j]的值和a[++i]交换
int partition(int a[], int left, int right)
{
	int i = left;
	for (int j = left; j < right; j++)
	{
		if (a[j] < a[left])
		{
			swap(a[++i], a[j]);
		}
 
	}
	swap(a[i], a[left]);
	return i;
}
 
//快速排序  用随机法选取任意一个数（数组内）作为基数，并将该数与a[left]互换
//递归方式，对分区数组再进行划分
void quickSort(int a[], int left, int right)
{
	if (left < right)
	{
		int i = left + rand() % (right - left);
		swap(a[i], a[left]);
		int mid = partition(a, left, right);
		quickSort(a, left, mid);
		quickSort(a, mid + 1, right);
	}
 
}
 
int main()
{
	int a[] = { 2,1,4,5,3,8,7,9,0,6 };
 
	quickSort(a, 0, 10);
 
	for (int i = 0; i < 10; i++)
	{
		cout << a[i] << "";
 
	}
	cout << endl;
	return 0;
}