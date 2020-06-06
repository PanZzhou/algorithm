#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdlib>
#include <ctime>
#include <vector>
using namespace std;
typedef struct node{
    int scene;    //相机看到的场景编号
    struct node* next;
}node,*Node;

typedef struct list{
	int scene_num;//当前相机看到的场景总数
	struct node* next;
}list,*List;

List arr=nullptr;
int* res=nullptr;  //最后的结果，有m个元素，res[i]为1时，表示将这个相机放入集合
int* dot=nullptr;  //每个观察点被当前的res集合中的摄像机拍摄到的次数,有的dot[i]初始值为0
int* cam_done=nullptr; //当前camera看到的所有观察点中，其min_dot_of_camera已经最小了（为1）
int* verify=nullptr;
#define FACTOR 100
string str="C:\\Users\\FlameZ\\Desktop\\computer\\SetCoveringRealData\\SC01.txt";
// string str="C:\\Users\\FlameZ\\Desktop\\computer\\SetCoveringTestData\\test01.txt";
string my="result-";
ifstream in(str.c_str());

void init_data(int &,int &);
void free_data(int);
void insert_list(list &, int);
void insert(Node &,int);
void delete_list(list &);
void delete_node(Node &);
int min_dot_of_camera(int);
int max_of_all_camera(int);
void deal_data(int,int);
void random_camera(int &, int &, int);
void remove_camera(int);
int random(int);
void print_res(int,int);
int next_camera(int, int);