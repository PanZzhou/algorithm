#include <iostream>
using namespace std;
int main(){
    int N;
    while(cin>>N){
        int ans[N],front[N],tail[N],res[N],r=0;
        for(int k=0;k<N;k++){
            front[k]=1;
            tail[k]=1;
            cin>>ans[k];
        }
        for(int i=0;i<N;i++){
            for(int j=i-1;j>=0;j--){
                if(ans[i]>=ans[j]){
                    front[i]=front[j]+1;
                    break;
                }
            }
        }
        for(int i=N-2;i>=0;i--){
            for(int j=i+1;j<N;j++){
                if(ans[i]>=ans[j]){
                    tail[i]=tail[j]+1;
                    break;
                }
            }
        }
        for(int i=0;i<N;i++){
            res[i]=front[i]+tail[i]-1;
            if(r<res[i])
                r=res[i];
        }
        cout<<N-r<<endl;
    }
    return 0;
}