#include <iostream>
#include <stack>
#include <queue>
using namespace std;
typedef int TElemType;
typedef struct BiNode{
	TElemType data;
	struct BiNode *lchild, *rchild;
	int ltag, rtag;//用于二叉树线索化成线索二叉树
}BiNode, *BiTree;

//二叉树结构初始化数组，先序顺序初始化，为零表示无节点
int Tree[] = {1,2,4,0,0,0,3,5,0,0,0};

/****非递归实现****/
// 先序
void PreTraverseN(BiTree p){
	stack<BiNode*> s;
	BiNode *cur = p;
	while(cur || !s.empty()){
		if(cur){
			printf("%d\n", cur->data);
			s.push(cur);
			cur = cur->lchild;
		}else{
			cur = s.top();
			s.pop();
			cur = cur->rchild;
		}
	}
}
// 中序
void InTraverseN(BiTree p){
	stack<BiNode*> s;
	BiNode *cur = p;
	while(cur || !s.empty()){
		if(cur){
			s.push(cur);
			cur = cur->lchild;
		}else{
			cur = s.top();
			s.pop();
			printf("%d\n", cur->data);
			cur = cur->rchild;
		}
	}
}
// 后序
void PostTraverseN(BiTree p){
	stack<BiNode*> s;
	BiNode *cur = p, *visit = nullptr; //visit指针用于记录上一个被访问的节点
	while(cur || !s.empty()){
		if(cur){
			s.push(cur);
			cur = cur->lchild;
		}else{
			cur = s.top(); //取栈顶，并无出栈操作
			if(cur->rchild && cur->rchild != visit) //栈顶节点存在右孩子并且右孩子还未访问到
				cur = cur->rchild;
			else{
				s.pop();
				printf("%d\n", cur->data);
				visit = cur; //设置当前节点已被访问
				cur = nullptr; //置空，下一循环取栈顶操作
			}
		}
	}
}
/****递归实现****/
//先序
void PreTraverse(BiTree p){
	if(p){
	printf("%d\n", p->data);
	PreTraverse(p->lchild);
	PreTraverse(p->rchild);
	}
}
//中序
void InTraverse(BiTree p){
	if(p){
		InTraverse(p->lchild);
		printf("%d\n", p->data);
		InTraverse(p->rchild);
	}
}
//后序
void PostTraverse(BiTree p){
	if(p){
	PostTraverse(p->lchild);
	PostTraverse(p->rchild);
	printf("%d\n", p->data);
	}
}
/****其他****/
//二叉树层次遍历
void LayerTraverse(BiTree p){
	queue<BiNode*> q;
	BiNode *cur = p;
	q.push(cur);
	while(!q.empty()){
		cur = q.front();
		q.pop();
		printf("%d\n", cur->data);
		if(cur->lchild){
			q.push(cur->lchild);
		    cout<<"lchild is "<<(cur->lchild)->data<<endl;
		}
		if(cur->rchild){
			q.push(cur->rchild);
			cout<<" rchild is "<<(cur->rchild)->data<<endl;
		}
	}
}
//二叉树线索化
//pre初始值位nullptr
void ThreadBiTree(BiTree p, BiNode *pre){
	if(p){
		ThreadBiTree(p->lchild, pre);
		if(p->lchild == nullptr){
			p->lchild = pre;
			p->ltag = 1;
		}
		if(pre && pre->rchild == nullptr){
			pre->rchild = p;
			p->rtag = 1;
		}
		pre = p;
		ThreadBiTree(p->rchild, pre);
	}
}
//根据线索遍历线索二叉树
void TreadBiTreeTraverse(BiTree p){
	BiNode *cur = p;
	while(cur){
		while(cur->ltag == 0) cur = cur->lchild;
		printf("%d\n", cur->data);
		while(cur->rtag == 1 && cur->rchild){
			cur = cur->rchild;
			printf("%d\n", cur->data);
		}
		cur = cur->rchild;
	}
}
//创建二叉树
void CreateBiTree(BiTree &p, int &tag){
	int a = Tree[tag];
	cout<<"element is ";
	printf("%d\n", a);
	if(a != 0){
		p = (BiNode*)malloc(sizeof(BiNode));
		p->data = a;
		CreateBiTree(p->lchild, ++tag);//这里加++就行，每执行一次函数自增一次
		CreateBiTree(p->rchild, ++tag); //这里加++就行，每执行一次函数自增一次
	}else{
		p = nullptr; //不可以缺少
	}
}
//销毁二叉树
void DestroyBiTree(BiTree &p){
	if(p){
	DestroyBiTree(p->lchild);
	DestroyBiTree(p->rchild);
	free(p);
	p = nullptr;//这里相当于把父节点的左或者右指针置空
	}
}
void TraverseTest(BiTree t){
	printf("%s\n", "start traverse binaty tree whth recursion");
	//二叉树递归递归
	printf("%s\n", "preorder:");
	PreTraverse(t);
	printf("%s\n", "inorder:");
	InTraverse(t);
	printf("%s\n", "postorder:");
	PostTraverse(t);
	//二叉树递归非递归
	printf("%s\n", "start traverse binaty tree without recursion");
	printf("%s\n", "preorder:");
	PreTraverseN(t);
	printf("%s\n", "inorder:");
	InTraverseN(t);
	printf("%s\n", "postorder:");
	PostTraverseN(t);
	//层次遍历
	printf("%s\n", "start layer traverse:");
	LayerTraverse(t);
}
int main(){
	BiTree t;
	int tag = 0;
	printf("%s\n", "start create binary tree");
	CreateBiTree(t, tag);
	TraverseTest(t);
	printf("%s\n", "start destroy binaty tree");
	DestroyBiTree(t);
	return 0;
}