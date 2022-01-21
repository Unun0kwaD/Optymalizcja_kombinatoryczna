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
int kolory[1000],maxkolor[1000];
int I,J;
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
    int id;
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
    int pokoloruj2(){
        int n = somsiedzi.size();
        char zajete[W+1];
        memset(zajete,0,sizeof(zajete));
        /*
        for (int j=0;j<=W;j++){
            zajete[j]=0;
        }
        */
        for(int j=0;j<n;j++){
           zajete[kolory[(*somsiedzi[j]).id]]=1;
        }
        int k=1;
        while(zajete[k]){
            k++;
        }
        kolory[id]=k;
        return kolory[id];
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
void zeruj2(int s,int n,wierzchol *graf){
    memset(&kolory[s],0,sizeof(n)*(n-s));
    memset(&maxkolor[s],0,sizeof(n)*(n-s));
}
int kolorujsekwencyjnie(int n,int *tab,wierzchol *graf){
    int max_kolor=0;
    for (int i=0;i<n;i++){
        max_kolor=max(max_kolor,(graf[tab[i]]).pokoloruj());
    }
    return max_kolor;
}
int kolorujsekwencyjnie2(int s,int n,int *tab,wierzchol *graf){
    if (s==0)
        maxkolor[0]=(graf[tab[s]]).pokoloruj2();
    for (int i=s;i<n;i++){
        maxkolor[i]=max(maxkolor[i-1],(graf[tab[i]]).pokoloruj2());
    }
    return maxkolor[n-1];
}
void showtab(int *tab,int n){
    for(int i = 0; i<n;i++){
        cerr<<tab[i]<<" ";
    }
    cerr<<endl;
}
prique generator_swapow(int *tab,int n,wierzchol *graf,int val){
    //dodadać parametr który będzie sprawdzał maksymalnie X sąsiadów zaczynając od N który będzie powiększony o krok poprzedniego lub jężeli znajdzie wystarczająco lepsze rozwiązanie
    prique swapy;
    int t=0;
    if(J+1>=n){
        I=0;
        J=0;
    }
    int v;
    if (n>600){
        zeruj2(0,n,graf);
        v=kolorujsekwencyjnie2(0,n,tab,graf);
    }
    while( I<n){
        J=I+1;// dla większych wukomentuj
        while (J<n){
            if(I==J){
                J=I+1;
            }
            swap(tab[I],tab[J]);
            //showtab(tab,n);
            zeruj2(I,n,graf);
            v=kolorujsekwencyjnie2(max(0,I-2),n,tab,graf);
            //cerr<<v<<endl;
            swapy.push({v,{I,J}});
            swap(tab[I],tab[J]);
            J++;
            if (v<val && n>600){
                return swapy;
            }
        }
        I++;
    }
    I=0;
    J=0;
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
    int mini=n,ans,mintab[n],v,nowval,lastv,timesrepeat=0;
    deque<pair<int, int>> tabu;
    //zeruj(n,graf);
    pair<int,int> sw;
    zeruj2(0,n,graf);
    //usun dla jednolitego sprawdzania czasu przy testach
    //unsigned seed = chrono::system_clock::now().time_since_epoch().count();
    //shuffle(tab,&tab[n], default_random_engine(seed));
    
    mini=kolorujsekwencyjnie2(0,n,tab,graf);
    cerr<<mini<<"\n";
    for (int i=0;i<l_iteracji;i++){

        cerr<<mini<<endl;
        prique swapy=generator_swapow(tab,n,graf,mini-1);
        //cerr<<"wygenerowane swapy"<<endl;
        sw=swapy.top().second;
        v=swapy.top().first;
        swapy.pop();
        /*
        //przy długiej powtarającej się serii przetasowuje sekwencję
        if (v>=lastv){
            timesrepeat++;
            if (timesrepeat>100){
                cerr<<"losowanie\n";
                zeruj2(0,n,graf);
                v=randomsequence(dlugosc_tabu,n,tab,graf);
                timesrepeat=0;
            }
        }
        else timesrepeat=0;
        lastv=v;
        */
        cout<<v<<"\t";
        bool czy_w_tabu=0;
        for(int i=0;i<tabu.size();i++){
            if(tabu[i].first==sw.first && tabu[i].second==sw.second){
                czy_w_tabu=1;
                break;
            }
        }
        while(0<swapy.size()){
            if(!czy_w_tabu || mini>v){
                swap(tab[sw.first],tab[sw.second]);
                nowval=v;
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
        while (swapy.size()>0){
            sw=swapy.top().second;
            swapy.pop();
            swap(tab[sw.first],tab[sw.second]);
            zeruj2(max(0,min(sw.first,sw.second)-2),n,graf);
            v=kolorujsekwencyjnie2(max(0,min(sw.first,sw.second)-2),n,tab,graf);
            if (v>nowval){
                swap(tab[sw.first],tab[sw.second]);
                break;
            }
            else{
                nowval=v;
                if (v<mini){
                    mini=v;
                    cerr<<mini<<endl;
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
    for (int i=0;i<=W;i++){
        graf[i].id=i;
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
            zeruj2(0,W,graf);
            min_kolor=tabu_search(100000,6,W,tab,graf);
    }
    cout<<min_kolor<<endl;
}
/*
./dooddania1 queen6.txt
Podaj liczbe losowych sekwencji kolorowania: 
10000
8
tabu: 6 dla 200 iteracji tabusize:6
./dooddania1 le450_5a.txt
Podaj liczbe losowych sekwencji kolorowania: 
10000
12
./dooddania1 gc500.txt
Podaj liczbe losowych sekwencji kolorowania: 
10000
84
tabu:81
/dooddania1 gc_1000_300013.txt
Podaj liczbe losowych sekwencji kolorowania: 
3000 
152
tabu: 151 - poniżej 30 minut 12.12.2021
        150 -po 1h
./dooddania1 miles250.txt
Podaj liczbe losowych sekwencji kolorowania: 
10000
8
/dooddania1 data.txt
zachłanne: 19
tabu: 13
/dooddania1 data2.txt
zachłanne: 4
tabu: 2
/dooddania1 data3.txt
zachłanne: 22
tabu: 15
/dooddania1 data5.txt
zachłanne: 19
tabu: 12
*/
// 5 minut to górna granica czasu działania przy dużych datasetach