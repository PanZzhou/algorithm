#include "test.h"

int main(){
    if(in.is_open()){
        srand((int)time(0));
        int n,m;//m��camera��n��scene
        time_t s,e;
        s=time(0);
        init_data(m,n);
        ///////////��������/////////
        deal_data(m,n);
        print_res(m,n);  //������
        //////////////////////////
        free_data(m);
        e=time(0);
        cout<<"����ʱ��"<<e-s<<endl;
    }
    return 0;
}

//��������������
void deal_data(int m,int n){
    int first,second;//�����Ƚϵ�����camera
    int first_min,second_min;//����camera�ܹ����ӵ����е�A��,�ܹ����ӵ���A������camera��������Сֵ
    int max_min_camera,round=0;
    time_t start=time(NULL),end;
    while(1){
        random_camera(first,second,m);//ѡ��Ҫ�Ƚϵ�����camera
        first_min=min_dot_of_camera(first);
        second_min=min_dot_of_camera(second);
        cout<<"first:"<<first<<"   first_min:"<<first_min<<endl;
        cout<<"second:"<<second<<"   second_min:"<<second_min<<endl;
        if(first_min==1&&second_min==1){
            round++;
            cam_done[first]=cam_done[second]=1;   //���ô�camera���ܱ��Ƴ�����
            if(round==FACTOR){ //ѡ��FACTOR���������ÿһ�εĽ������1==1������ζ��û�ж��������������ˣ���������!
                cout<<"program exit!!!(not time_out)"<<endl;
                break;
            }
            continue;
        }
        //ѡ����������з���ֵ����������
        if(first_min==1)
            cam_done[first]=1;
        if(second_min==1)
            cam_done[second]=1;
        max_min_camera=first;
        if(first_min<second_min)
            max_min_camera=second;
        if(first_min==second_min)//�����������Сֵ����Ҳ�����1ʱ
            if(arr[second].scene_num<arr[first].scene_num)//ѡ�����㷶Χ��С�����ɾ��
                max_min_camera=second;
        remove_camera(max_min_camera);//�Ƴ�ѡ�е����
        round=0;//ɾ���������˵����ʱ����������ﻹ����������������������������round
        time(&end);
        // cout<<"end "<<end<<endl;
        if(end-start>300){//��ʱ�Ļ�(5����)���˳�����
            cout<<"program exit time_out!!!"<<endl;
            break;
        }
    }
}

//��֤������
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
    //��һ������
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
    //�ڶ�������
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

//�Ƴ����
void remove_camera(int i){
    res[i]=0;//�Ӽ������Ƴ��������
    Node camera=arr[i].next;
    while(camera){
        dot[camera->scene]--;//�Ƴ�������󣬸���ÿ��������Ӧ�������������
        camera=camera->next;
    }
}

//m��camera��ѡȡ������Ч�������
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
//�����camera������Чʱ�����ݸ����������ҵ�����������camera
int next_camera(int i, int m){
    int flag=-1,k;
    float r=rand() % 100 / (float)100;
    if(r<0.5){//%50�����������
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
    }else{//%50������ǰ����
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

//������ɺ�������Ϊrand()�����������������RAND_MAX���ʵ�m����RAND_MAXʱ��ʹ����������������������RAND_MAX��������
int random(int m){
    if(m>RAND_MAX){
        return (rand()*rand())%m;
    }
    else{
        return rand()%m;
    }
}

//camera�ܹ��۲����������������ÿ���������ܱ�A��cameras�۲쵽�����ÿ��������Aֵ����Сֵ
int min_dot_of_camera(int i){
    if(cam_done[i]==1)//��ǰ����Ѿ������Ƴ�
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

//��ʼ��ʹ�õ�������
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

//�ͷ��ڴ�
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

//��arr[i]->next�����в��볡���ڵ㣬��ʾ��ǰcamera�ܹ������˳���
void insert_list(list &l, int value){
    insert(l.next,value);
}
//����������
void insert(Node &root,int value){
    if(root==nullptr){
        root=(Node)malloc(sizeof(node));
        root->scene=value;
        root->next=nullptr;
    }else{
        insert(root->next,value);
    }
}
//����arr[i](��һ��list�ṹ��)
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