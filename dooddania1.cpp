#include <iostream>
#include <algorithm>   
#include <array>
#include <random>
#include <chrono>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <string.h>
#include <utility>  
#include <queue>

//typedef priority_queue< pair<int,pair<int,int>>,vector<pair<int,pair<int,int>>>>,greater<pair<int,pair<int,int>>>> prq
using namespace std;
int W;
class myComparator
{
public:
    int operator() (const pair<int,pair<int,int>>& p1, const pair<int,pair<int,int>>& p2)
    {
        return p1.first > p2.first;
    }
};
typedef priority_queue<pair<int,pair<int,int>>,vector<pair<int,pair<int,int>>>,myComparator> prique;
void wprowadz(int *tab1,int *tab2,int n){
    for (int i=0;i<n;i++){
        tab1[i]=tab2[i];
    }
}
struct wierzchol{
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
    bool istnieje(wierzchol * a){
        for (int i=0;i<somsiedzi.size();i++){
            if ((somsiedzi[i])==a){
                return 1;
                //cerr<<"Wierzchołek powtórzony!!!"<<endl;
            }
        }
        return 0;
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
void showtab(int *tab,int n){
    for(int i = 0; i<n;i++){
        cerr<<tab[i]<<" ";
    }
    cerr<<endl;
}
prique generator_swapow(int *tab,int n,wierzchol *graf){
    prique swapy;
    int v;
    for (int i=0; i<n;i++){
        for (int j=i+1;j<n;j++){
            swap(tab[i],tab[j]);
            //showtab(tab,n);
            v=kolorujsekwencyjnie(n,tab,graf);
            zeruj(n,graf);
            //cerr<<v<<endl;
            swapy.push({v,{i,j}});
            swap(tab[i],tab[j]);
        }
    }
    return swapy;
}

int randomsequence(int liczba_losowan,int n,int *tab,wierzchol *graf){
    int mini=n,ans,mintab[n];
    for (int i=0;i<liczba_losowan;i++){
        zeruj(n,graf);

        unsigned seed = chrono::system_clock::now().time_since_epoch().count();
        shuffle(tab,&tab[n], default_random_engine(seed));

        /*
        cout<<"losowanie "<<i<<"\tans: "<<ans<<endl;
        cout<<"sekwencja: ";
        for (int i=0;i<W;i++){
            cout<<" "<<tab[i];
        }
        cout<<endl;
        */
        ans=kolorujsekwencyjnie(n,tab,graf);
        if (ans<mini){
            mini=ans;
            wprowadz(mintab,tab,n);
            // for (int k=1;k<=W;k++)   cout<<"Kolor wierzcholka nr:"<<k<<" "<<graf[k].kolor<<endl;
        }
    }
    return mini;
}
int tabu_search(int l_iteracji,int dlugosc_tabu,int n,int *tab,wierzchol *graf){
    int mini=n,ans,mintab[n];
    deque<pair<int, int>> tabu;
    zeruj(n,graf);

    ans=kolorujsekwencyjnie(n,tab,graf);
    for (int i=0;i<l_iteracji;i++){
        cerr<<mini<<endl;

        prique swapy=generator_swapow(tab,n,graf);
        cerr<<"wygenerowani swapy"<<endl;
        pair<int,int> sw=swapy.top().second;
        int v=swapy.top().first;
        swapy.pop();
        //cout<<v<<"\t";
        bool czy_w_tabu=0;
        for(int i=0;i<tabu.size();i++){
            if(tabu[i].first==sw.first && tabu[i].second==sw.second){
                czy_w_tabu=1;
                break;
            }
        }
        while(0<swapy.size()){
            if(!czy_w_tabu || mini<v){
                swap(tab[sw.first],tab[sw.second]);
                tabu.push_back(sw);
                if(tabu.size()>dlugosc_tabu)
                    tabu.pop_front();
                if (v<mini){
                    mini=v;
                    cerr<<mini<<endl;
                }
                break;
            }
            else{
                sw=swapy.top().second;
                v=swapy.top().first;
                swapy.pop();
                for(int i=0;i<tabu.size();i++){
                    if(tabu[i].first==sw.first && tabu[i].second==sw.second){
                        czy_w_tabu=1;
                        break;
                    }
                    else czy_w_tabu=0;
                }
            }
        }
    }
    return mini;

}
int main(int argc, char* argv[]){
    int a=1,b=1, min_kolor=0,k,r;
    switch (argc)
    {
    case 0: case 1:
        cout<<"Podaj liczbe wierzcholkow: ";
        cin>>W;
        break;
    
    default:
        ifstream infile(argv[1]);
        infile>>a;
        W = a;
        break;
    }
    
    wierzchol graf[W+1];

    switch (argc)
    {
    case 0: case 1:
        while(a!=0 && b!=0){
            cin>>a>>b;
            if (a!=b){
                if (!graf[a].istnieje(&graf[b])){
                    graf[a].polacz(&graf[b]);
                    graf[b].polacz(&graf[a]);
                }
                //else    cerr<<"Wierzchołek powtórzony!!!"<<endl;
            }
            else    cerr<<"Błąd!!!"<<endl;
        }
        break;
    
    default:
        ifstream infile(argv[1]);
        infile>>a;
        while ((infile >> a >> b)){
            if (a!=b){
                if (!graf[a].istnieje(&graf[b])){
                    graf[a].polacz(&graf[b]);
                    graf[b].polacz(&graf[a]);
                }
                //else    cerr<<"Wierzchołek powtórzony!!!"<<endl;
            }
            else    cerr<<"Błąd!!!"<<endl;
        }
        infile.close();
        break;
    }
    int tab[W];
    for (int i=1;i<=W;i++){
        tab[i-1]=i;
    }
    int wyb;
    cout<<"Wybierz metode kolorowania: "<<endl;
    cout<<"1. Od pierszego do W-tego wierzchołka według indeksu"<<endl;
    cout<<"2. N losowych sekwencji kolorowania: "<<endl;
    cout<<"3. Według jednej podanej sekwencji kolorowania: "<<endl;
    cout<<"4. Tabu search "<<endl;
    /*TODO:
    cout<<"4. Według sąsiedztwa: "<<endl;
    cout<<"5. najpierw wierzcholki o najwiekszym stopniu: "<<endl;
    cout<<"6. : "<<endl;

    //dodaj sprawdzanie spojności i usuwanie wierzchołków powtarzających się
    */
    cin>>wyb;
    switch (wyb){
        case 1:
            min_kolor=kolorujsekwencyjnie(W,tab,graf);
            //for (int i=1;i<=W;i++) cout<<"Kolor wierzcholka nr:"<<i<<" "<<graf[i].kolor<<endl;
            break;
        case 2:
            cout<<"Podaj liczbe losowych sekwencji kolorowania: "<<endl;
            cin>>r;
            min_kolor=randomsequence(r,W,tab,graf);
            break;
        case 3:
            cout<<"Podaj sekwencje liczb od 1 do "<<W<<":"<<endl;
            for (int i =0;i<W;i++){
                cin>>a;
                tab[i]=a;
            }
            min_kolor=kolorujsekwencyjnie(W,tab,graf);
        case 4:
            min_kolor=tabu_search(20,5,W,tab,graf);
    }
    cout<<min_kolor<<endl;
}
/*
./dooddania1 queen6.txt
Podaj liczbe losowych sekwencji kolorowania: 
10000
8
./dooddania1 le450_5a.txt
Podaj liczbe losowych sekwencji kolorowania: 
10000
12
./dooddania1 gc500.txt
Podaj liczbe losowych sekwencji kolorowania: 
10000
84
/dooddania1 gc_1000_300013.txt
Podaj liczbe losowych sekwencji kolorowania: 
3000 
152
./dooddania1 miles250.txt
Podaj liczbe losowych sekwencji kolorowania: 
10000
8
*/
// 5 minut to górna granica czasu działania przy dużych datasetach