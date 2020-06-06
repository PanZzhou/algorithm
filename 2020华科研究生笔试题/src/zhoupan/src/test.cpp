#include "test.h"

int main(){
    if(in.is_open()){
        srand((int)time(0));
        int n,m;//m个camera，n个scene
        time_t s,e;
        s=time(0);
        init_data(m,n);
        ///////////处理数据/////////
        deal_data(m,n);
        print_res(m,n);  //处理结果
        //////////////////////////
        free_data(m);
        e=time(0);
        cout<<"消耗时间"<<e-s<<endl;
    }
    return 0;
}

//处理数据主函数
void deal_data(int m,int n){
    int first,second;//用来比较的两个camera
    int first_min,second_min;//两个camera能够监视的所有点A中,能够监视到点A的所有camera数量的最小值
    int max_min_camera,round=0;
    time_t start=time(NULL),end;
    while(1){
        random_camera(first,second,m);//选出要比较的两个camera
        first_min=min_dot_of_camera(first);
        second_min=min_dot_of_camera(second);
        cout<<"first:"<<first<<"   first_min:"<<first_min<<endl;
        cout<<"second:"<<second<<"   second_min:"<<second_min<<endl;
        if(first_min==1&&second_min==1){
            round++;
            cam_done[first]=cam_done[second]=1;   //设置此camera不能被移除集合
            if(round==FACTOR){ //选择FACTOR次摄像机，每一次的结果都是1==1，则意味着没有多余的摄像机存在了，结束程序!
                cout<<"program exit!!!(not time_out)"<<endl;
                break;
            }
            continue;
        }
        //选出两个相机中返回值更大的摄像机
        if(first_min==1)
            cam_done[first]=1;
        if(second_min==1)
            cam_done[second]=1;
        max_min_camera=first;
        if(first_min<second_min)
            max_min_camera=second;
        if(first_min==second_min)//两个相机的最小值相等且不等于1时
            if(arr[second].scene_num<arr[first].scene_num)//选择拍摄范围最小的相机删除
                max_min_camera=second;
        remove_camera(max_min_camera);//移除选中的相机
        round=0;//删除了相机，说明此时摄像机集合里还存在其他可以清楚的摄像机，重置round
        time(&end);
        // cout<<"end "<<end<<endl;
        if(end-start>300){//超时的话(5分钟)，退出程序
            cout<<"program exit time_out!!!"<<endl;
            break;
        }
    }
}

//验证输出结果
void print_res(int m,int n){
    vector<int> vec;
    int len;
    for(int i=0;i<m;i++){
        if(res[i]>0){
            vec.push_back(i);
        }
    }
    len=vec.size();
    cout<<"camera_set size is:"<<len<<endl;
    bool f=true;
    //第一次验算
    for(int i=0;i<n;i++){
        if(verify[i]>0&&dot[i]<=0){
            cout<<"top"<<i<<" "<<verify[i]<<" "<<dot[i]<<endl;
            f=false;
        }
        if(verify[i]==0&&dot[i]>0){
            cout<<"bot"<<i<<" "<<verify[i]<<" "<<dot[i]<<endl;
            f=false;
        }
    }
    if(!f)
        cout<<"1.result has error"<<endl;
    //第二次验算
    memset(dot,0,sizeof(int)*n);
    for(int i=0;i<len;i++){
        Node temp=arr[vec[i]].next;
        while(temp){
            dot[temp->scene]++;
            temp=temp->next;
        }
    }

    f=true;
    for(int i=0;i<n;i++){
        if(verify[i]>0&&dot[i]<=0){
            cout<<"top"<<i<<" "<<verify[i]<<" "<<dot[i]<<endl;
            f=false;
        }
        if(verify[i]==0&&dot[i]>0){
            cout<<"bot"<<i<<" "<<verify[i]<<" "<<dot[i]<<endl;
            f=false;
        }
    }
    if(!f)
        cout<<"2.result has error"<<endl;
    else{
        int temp=str.find_last_of('\\');
        string ss=str.substr(temp+1);
        str=str.substr(0,temp+1);
        my=my.append(ss);
        my=str.append(my);
        cout<<my<<endl;
        ofstream out(my.c_str(),ofstream::out);
        out<<len<<endl;
        for(int i=0;i<len;i++)
            out<<vec[i]<<' ';
        out.close();
    }
}

//移除相机
void remove_camera(int i){
    res[i]=0;//从集合中移除此摄像机
    Node camera=arr[i].next;
    while(camera){
        dot[camera->scene]--;//移除摄像机后，更新每个场景对应的总摄像机数量
        camera=camera->next;
    }
}

//m个camera中选取两个有效相机出来
void random_camera(int &first, int &second, int m){
     // srand((int)time(0));
     first=random(m);
     second=random(m);
     if(res[first]==0)
        first=next_camera(first,m);
     if(res[second]==0)
        second=next_camera(second,m);
     while(first==second){
        // cout<<"invalide random"<<first<<" "<<second<<" "<<res[first]<<" "<<res[second]<<endl;
        second=next_camera(second,m);
     }
}
//随机的camera生成无效时，依据概率向两边找到符合条件的camera
int next_camera(int i, int m){
    int flag=-1,k;
    float r=rand() % 100 / (float)100;
    if(r<0.5){//%50概率向后搜索
        for(k=i-1;k!=i;k--){
            if(k<0)
                k=m-1;
            if(flag==-1&&res[k]==1){
                if(cam_done[k]==1)
                    flag=k;
                else
                    return k;
            }
            if(flag>=0&&res[k]==1&&cam_done[k]!=1)
                return k;
        }
    }else{//%50概率向前搜索
        for(k=i+1;k!=i;k++){
            if(k>=m)
                k=0;
            if(flag==-1&&res[k]==1){
                if(cam_done[k]==1)
                    flag=k;
                else
                    return k;
            }
            if(flag>=0&&res[k]==1&&cam_done[k]!=1)
                return k;
        }
    }
    return flag;
}

//随机生成函数，因为rand()产生的随机数不大于RAND_MAX，故当m大于RAND_MAX时，使用两次随机数相乘来产生比RAND_MAX大的随机数
int random(int m){
    if(m>RAND_MAX){
        return (rand()*rand())%m;
    }
    else{
        return rand()%m;
    }
}

//camera能够观察多个场景，依次算出每个场景又能被A个cameras观察到，求出每个场景的A值的最小值
int min_dot_of_camera(int i){
    if(cam_done[i]==1)//当前相机已经不能移除
        return 1;
    int min=400000;
    Node camera=arr[i].next;
    while(camera){
        if(dot[camera->scene]<min){
            min=dot[camera->scene];
        }
        camera=camera->next;
    }
    return min;
}

//初始化使用到的数组
void init_data(int &m,int &n){
    int i,j,num;
    in>>m>>n;
    arr=(List)malloc(sizeof(list)*m);
    dot=(int*)malloc(sizeof(int)*n);
    res=(int*)malloc(sizeof(int)*m);
    cam_done=(int*)malloc(sizeof(int)*m);
    verify=(int*)malloc(sizeof(int)*n);

    memset(dot,0,sizeof(int)*n);
    memset(verify,0,sizeof(int)*n);
    for(int k=0;k<m;k++){
        res[k]=1;
        cam_done[k]=0;
        arr[k].scene_num=0;
        arr[k].next=nullptr;
    }

    while(in>>i){
        in>>num;
        arr[i].scene_num=num;
        for(int k=0;k<num;k++){
            in>>j;
            insert_list(arr[i], j);
            dot[j]++;
            verify[j]++;
        }
    }
}

//释放内存
void free_data(int m){
    for(int k=0;k<m;k++)
        delete_list(arr[k]);
    free(arr);
    free(res);
    free(dot);
    free(cam_done);
    arr=nullptr;
    res=nullptr;
    dot=nullptr;
    in.close();
}

//向arr[i]->next链表中插入场景节点，表示当前camera能够看到此场景
void insert_list(list &l, int value){
    insert(l.next,value);
}
//具体插入操作
void insert(Node &root,int value){
    if(root==nullptr){
        root=(Node)malloc(sizeof(node));
        root->scene=value;
        root->next=nullptr;
    }else{
        insert(root->next,value);
    }
}
//重置arr[i](是一个list结构体)
void delete_list(list &l){
    delete_node(l.next);
    l.scene_num=0;
}
void delete_node(Node &root){
    if(root==nullptr)
        return;
    Node temp,pre=root;
    while(pre){
        temp=pre->next;
        free(pre);
        pre=temp;
    }
    root=nullptr;
}