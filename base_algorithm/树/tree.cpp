#include <iostream>
using namespace std;
typedef struct Node{
    int data;
    struct Node* lchild;
    struct Node* rchild;
}Node,*TreeNode;
int insert(TreeNode &r, int value, int f);
int main(){
    int N;
    while(cin>>N){
        int temp;
        TreeNode root=nullptr;
        for(int i=0;i<N;i++){
            cin>>temp;
            cout<<insert(root, temp, -1)<<endl;
        }
    }
    return 0;
}
int insert(TreeNode &r, int value, int f){
    if(nullptr==r){
        r=(TreeNode)malloc(sizeof(Node));
        r->lchild=nullptr;
        r->rchild=nullptr;
        r->data=value;
        return f;
    }else if(value<r->data)
        return insert(r->lchild, value, zr->data);
    else 
        return insert(r->rchild, value, r->data);
}