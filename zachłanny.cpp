#include <iostream>
#include <vector>
using namespace std;

struct wierzchol{
    vector<int> somsiedzi;
    int kolor=0;
};

int main(){
    int W,a=1,b=1,kol=1, max_kolor=0;
    cin>>W;
    wierzchol graf[W+1];
    while(a!=0 && b!=0){
        cin>>a>>b;
        graf[a].somsiedzi.push_back(b);
        graf[b].somsiedzi.push_back(a);
    }
    for (int i=1;i<=W;i++){
        char zajete[W];
        for (int j=0;j<W;j++){
            zajete[j]=0;
        }
        if (i==1){
            graf[i].kolor=kol;
        }
        else{
            for(int j=0;j<graf[i].somsiedzi.size();j++){
                zajete[graf[graf[i].somsiedzi[j]].kolor]=1;
            }
            int k=1;
            while(zajete[k]){
                k++;
            }
            if(max_kolor<k){
                max_kolor=k;
            }
            graf[i].kolor=k;
        }
    }

    for (int i=1;i<=W;i++){
        cout<<"Kolor wierzcholka nr:"<<i<<" "<<graf[i].kolor<<endl;
    }
    cout<<max_kolor;
}