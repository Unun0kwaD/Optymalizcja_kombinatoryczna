#include <iostream>     // std::cout
#include <algorithm>    // std::shuffle
#include <array>        // std::array
#include <random>       // std::default_random_engine
#include <chrono>   
#include <vector>
using namespace std;
int W;

class wierzchol{
    public:
    vector<wierzchol *> somsiedzi;
    int kolor=0;
    void czysc(){
        kolor=0;
        /* //tylko dla spójengo grafu
        for(int j=0;j<somsiedzi.size();j++){
            if( (*somsiedzi[j]).kolor!=0)
                (*somsiedzi[j]).czysc();
        }
        */
    }
    void polacz(wierzchol * a){
        somsiedzi.push_back(a);
    }
    int pokoloruj(){
        int n = somsiedzi.size();
        char zajete[W+1];
        for (int j=0;j<=W;j++){
            zajete[j]=0;
        }
        for(int j=0;j<n;j++){
           zajete[(*somsiedzi[j]).kolor]=1;
        }
        int k=1;
        while(zajete[k]){
            k++;
        }
        kolor=k;
        return kolor;
    }
};
void zeruj(int n,wierzchol *graf){
    for(int i=1;i<=n;i++){
        graf[i].czysc();
    }
}
int kolorujsekwencyjnie(int n,int *tab,wierzchol *graf){
    int max_kolor=0,k,t;
    for (int i=0;i<n;i++){
        t=tab[i];
        k=(graf[t]).pokoloruj();
        if(max_kolor<k)
            max_kolor=k;
    }
    return max_kolor;
}

int randomsequence(int liczba_losowan,int n,int *tab,wierzchol *graf){
    int min=n,ans;
    for (int i=0;i<liczba_losowan;i++){
        
        zeruj(n,graf);
        unsigned seed = chrono::system_clock::now().time_since_epoch().count();
        shuffle(tab,&tab[n], default_random_engine(seed));
        ans=kolorujsekwencyjnie(n,tab,graf);
        if (ans<min){
            min=ans;
        }

        cout<<"losowanie "<<i<<"\tans: "<<ans<<endl;
        cout<<"sekwencja: ";
        for (int i=0;i<W;i++){
            cout<<" "<<tab[i];
        }
        cout<<endl;
       
    }
    return min;
}
int main(){
    int a=1,b=1, max_kolor=0,k,r;
    cin>>r;
    cin>>W;
    wierzchol graf[W+1];
    while(a!=0 && b!=0){
        cin>>a>>b;
        graf[a].polacz(&graf[b]);
        graf[b].polacz(&graf[a]);
    }
    int tab[W];

    for (int i=1;i<=W;i++){
        tab[i-1]=i;
    }
    max_kolor=randomsequence(r,W,tab,graf);
    /*
    for (int i=1;i<=W;i++){
        k=graf[i].pokoloruj();
        if(max_kolor<k)
            max_kolor=k;
    }*/
    /*
    for (int i=1;i<=W;i++){
        cout<<"Kolor wierzcholka nr:"<<i<<" "<<graf[i].kolor<<endl;
    }
    */
    cout<<max_kolor;
}
