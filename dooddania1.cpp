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
#include <signal.h>

//typedef priority_queue< pair<int,pair<int,int>>,vector<pair<int,pair<int,int>>>>,greater<pair<int,pair<int,int>>>> prq
int kolory[1000],maxkolor[1000];
int bignumberedge=600;
int gap=1;
int fragment=200;//dla dużych 200, dla małych jakieś 2000 albo i więcej
int I=0,J=0;
int garbage=0;
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
typedef pair<int,int> pi;
typedef priority_queue<pair<int,pi>,vector<pair<int,pi>>,myComparator> prique;
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
void showtab(int *tab,int n){
    for(int i = 0; i<n;i++){
        cerr<<tab[i]<<" ";
    }
    cerr<<endl;
}
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
    if (s==0){
        maxkolor[0]=(graf[tab[s]-1]).pokoloruj2();
        s++;
        }
    for (int i=s;i<n;i++){
        maxkolor[i]=max(maxkolor[i-1],(graf[tab[i]-1]).pokoloruj2());
    }
    return maxkolor[n-1];
}
int kolorujsort(int n,int *tab,wierzchol *graf){
    priority_queue<pi, vector<pi> > pq;
    for (int i =0;i<n;i++){
        pq.push({graf[i+1].somsiedzi.size(),i+1});
    }
    for (int i =0;i<n;i++){
        tab[i]=pq.top().second;
        pq.pop();
    }
    //showtab(tab,n);
    return kolorujsekwencyjnie(n,tab,graf);
}

prique generator_swapow(int *tab,int n,wierzchol *graf,int val,int times){
    //dodadać parametr który będzie sprawdzał maksymalnie X sąsiadów zaczynając od N który będzie powiększony o krok poprzedniego lub jężeli znajdzie wystarczająco lepsze rozwiązanie
    prique swapy;
    int t=0;
    if(J+1>=n){ I=0; J=0; }
    int v;
    //if (n>bignumberedge){
    zeruj(n,graf);
    v=kolorujsekwencyjnie(n,tab,graf);
    //}
    while( I<n){
        //J=I+1;// dla większych wukomentuj
        while (J<n){
            t++;
            swap(tab[I],tab[J]);
            //showtab(tab,n);
            zeruj(n,graf);
            v=kolorujsekwencyjnie(n,tab,graf);
            //cerr<<v<<endl;
            swapy.push({v,{I,J}});
            swap(tab[I],tab[J]);
            J++;
            if (t>times || v<val){ 
            //&& n>bignumberedge){
                //cout<<v<<"\n";
                //showtab(tab,n);
                return swapy;
            }
        }
        I++;
        J=I+1;
    }
    I=0;
    J=0;
    return swapy;
}
prique generator_swapow_reversed(int *tab,int n,wierzchol *graf,int val,int times){
    //dodadać parametr który będzie sprawdzał maksymalnie X sąsiadów zaczynając od N który będzie powiększony o krok poprzedniego lub jężeli znajdzie wystarczająco lepsze rozwiązanie
    prique swapy;
    int t=0;
    if(J<=0){
        I=n;
        J=n;
    }
    int v;
    //if (n>bignumberedge){
    zeruj2(0,n,graf);
    v=kolorujsekwencyjnie2(0,n,tab,graf);
   // }
    while( I>0){
        while (J>I){
            t++;
            //cerr<<t<<"  times\n";
            swap(tab[I],tab[J]);
            //showtab(tab,n);
            zeruj2(I,n,graf);
            v=kolorujsekwencyjnie2(max(0,I-2),n,tab,graf);
            //cerr<<v<<endl;
            swapy.push({v,{I,J}});
            swap(tab[I],tab[J]);
            J--;
            if (t>times || v<val){ //&& n>bignumberedge){
                t=0;
                return swapy;
            }
        }
        J=n;
        I--;
    }
    I=n;
    J=n;
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
            cerr<<mini<<"\n";
            wprowadz(mintab,tab,n);
            // for (int k=1;k<=W;k++)   cout<<"Kolor wierzcholka nr:"<<k<<" "<<graf[k].kolor<<endl;
        }
    }
    return mini;
}
int tabu_search(int l_iteracji,int dlugosc_tabu,int n,int *tab,wierzchol *graf){
    int mini=n,ans,v,nowval,lastv,timesrepeat=0;
    deque<pair<int, int>> tabu;
    //zeruj(n,graf);
    pair<int,int> sw;
    zeruj(n,graf);
    //usun dla jednolitego sprawdzania czasu przy testach
    //unsigned seed = chrono::system_clock::now().time_since_epoch().count();
    //shuffle(tab,&tab[n], default_random_engine(seed));
    mini=kolorujsort(n,tab,graf);
    //cerr<<mini<<"\n";
    for (int i=0;i<l_iteracji;i++){

        cerr<<mini<<endl;
        prique swapy=generator_swapow(tab,n,graf,mini-gap,fragment);
        //mini-gap);
        //cerr<<"wygenerowane swapy"<<endl;
        sw=swapy.top().second;
        v=swapy.top().first;
        swapy.pop();
        nowval=v;
        /*
        //przy długiej powtarającej się serii przetasowuje sekwencję
        //nic to nie daje gdyż tabusearch samemu stopniowo zmienia sekwencję w trakcie gdy powtarzają się wyniki
        //więc losując wracamy do punktu początkowego
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
        //cout<<v<<"\t";
        bool czy_w_tabu=0;
        for(int i=0;i<tabu.size();i++){//przeszukuje tabu czy nie ma tam nowego swapa
            if(tabu[i].first==sw.first && tabu[i].second==sw.second){
                czy_w_tabu=1;
                break;
            }
        }
        while(0<swapy.size()){//jeżeli znajdzie swapa który nie jest w tabu to go uzyj i wyjdz
            if(!czy_w_tabu && mini>=v){
                swap(tab[sw.first],tab[sw.second]);
                nowval=v;
                tabu.push_back(sw);
                if(tabu.size()>dlugosc_tabu)
                    tabu.pop_front();
                if (v<mini){
                    showtab(tab,n);
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
       // cout<<"szuka w śmieciah\n";
       /*
        while (swapy.size()>0 && swapy.top().first < mini+garbage){// szuka dobrych swapów wśród tych zostawionych
            
            sw=swapy.top().second;
            swapy.pop();
            swap(tab[sw.first],tab[sw.second]);
            zeruj(n,graf);
            v=kolorujsekwencyjnie(n,tab,graf);
            if (v>nowval){
                swap(tab[sw.first],tab[sw.second]);
                break;
            }
            else{
                tabu.push_back(sw);
                if (v<mini){
                    mini=v;
                    cerr<<"garbage: "<<mini<<endl;
                }
            }
        }
        */
        //cerr<<"koniec smieci\n";
        
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
    int l_tabu=6;//wielkosc tablicy tabu
    if(argc<3)
        cin>>wyb;
    else{
        wyb=stoi(argv[2]);//4-tabu
        l_tabu=stoi(argv[3]);//wielkośc tablicy tabu
        gap=stoi(argv[4]);// najlepiej 1
        fragment=stoi(argv[5]);//dla dużych 200 dla mniejszych 2000 dla najmniejszych 100000
        garbage=stoi(argv[6]);//dla o ile gorszych wartości szuka w pozostałych swapach
    }
    if(wyb==1)
        min_kolor=kolorujsekwencyjnie(W,tab,graf);
            //for (int i=1;i<=W;i++) cout<<"Kolor wierzcholka nr:"<<i<<" "<<graf[i].kolor<<endl;
    if(wyb==2){
            cout<<"Podaj liczbe losowych sekwencji kolorowania: "<<endl;
            cin>>r;
            min_kolor=randomsequence(r,W,tab,graf);
    }
     if(wyb==3){
            cout<<"Podaj sekwencje liczb od 1 do "<<W<<":"<<endl;
            for (int i =0;i<W;i++){
                cin>>a;
                tab[i]=a;
            }
            min_kolor=kolorujsekwencyjnie(W,tab,graf);
     }
     if(wyb==4){
            zeruj2(0,W,graf);
            min_kolor=tabu_search(10000,l_tabu,W,tab,graf);
     }
     if(wyb==5){
            min_kolor=kolorujsort(W,tab,graf);
     }
    // showtab(tab,W);
    cout<<min_kolor<<endl;
}
/*
./dooddania1 queen6.txt
Podaj liczbe losowych sekwencji kolorowania: 
10000
8
tabu, zwykle koloruj bez sort: 6 //fragment 2000 ./dooddania1 queen6.txt 4 1 100 200000 0
./dooddania1 le450_5a.txt
Podaj liczbe losowych sekwencji kolorowania: 
10000
12
tabu koloruj sort: 10  ./dooddania1 le450_5a.txt 4 6 10 20000000 2
better tabu: 8 //dla fragment 2000 tabusize=10
./dooddania1 gc500.txt
Podaj liczbe losowych sekwencji kolorowania: 
10000
84
tabu: 83 ./dooddania1 gc500.txt 4 6 10 2000000000 0
tabu:80 koloruj sort ./dooddania1 gc500.txt 4 10 1 200 0 //bez garbage

/dooddania1 gc_1000_300013.txt
Podaj liczbe losowych sekwencji kolorowania: 
3000 
152
tabu reversed:
tabu z kolorujsort: 146 ./dooddania1 gc_1000_300013.txt 4 10 1 200 //z gabage
better tabu 149 // dla fragment=200 tabusize=10
taby reversed 150 // dla fragment=200 tabusize=10
tabu: 150 - po 3 minutach bez wykomentowania
tabu: 152- po chwili
151 - po chwili przy zakomentowaniu linii 135
151 - poniżej 30 minut 12.12.2021
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

 5 minut to górna granica czasu działania przy dużych datasetach

25
50
100
200
500
1000
*/
/*
./dooddania1 gc500.txt 4 20 2000 1
80
./dooddania1 gc_1000_300013.txt 4 1 50 100 1
146
./dooddania1 le450_5a.txt 4 1 50 200 1
10
./dooddania1 gc500.txt 4 20 20 2000 1
79

gc_1000 sekwencja:
646 747 631 267 852 297 671 409 606 580 923 113 687 439 89 689 737 724 311 174 975 281 126 599 452 377 370 926 850 481 859 624 386 40 956 816 446 434 324 803 669 585 203 986 798 692 582 310 260 146 86 890 887 857 825 710 675 536 253 245 56 921 723 663 525 520 263 119 749 735 500 368 286 278 225 151 5 955 760 700 554 521 349 209 205 199 165 77 854 830 806 610 516 512 496 433 400 362 322 269 185 121 30 11 4 990 892 767 763 523 591 549 204 35 31 881 715 691 635 250 233 218 153 97 10 949 917 915 856 717 697 535 228 206 195 156 106 28 27 996 962 882 823 810 726 676 442 408 114 3 998 980 929 847 814 658 653 626 577 533 528 515 493 451 445 348 320 290 212 172 133 98 23 972 913 895 764 708 619 572 312 238 226 187 88 84 918 870 779 752 743 738 716 634 609 592 558 443 407 389 361 346 333 328 274 244 179 70 66 41 863 829 709 648 552 473 436 414 388 358 254 149 82 72 13 937 884 831 818 812 808 786 730 636 514 511 502 415 356 330 329 321 138 76 54 17 7 994 950 802 790 781 249 713 694 640 541 509 485 472 417 404 393 213 129 105 29 987 912 872 851 732 719 701 698 650 600 593 588 510 490 449 347 315 303 128 246 164 145 124 45 999 974 948 791 785 754 618 586 470 375 354 337 304 291 219 159 99 63 44 15 942 920 903 871 838 794 774 771 759 733 693 656 649 647 450 422 373 355 326 216 200 176 167 122 107 973 953 943 935 907 906 894 877 867 800 758 718 673 655 613 556 507 489 458 444 277 231 147 136 116 61 997 873 841 834 811 741 736 705 666 642 595 561 551 540 529 468 448 447 307 296 248 207 202 71 25 932 928 896 858 828 795 762 740 722 721 686 684 672 661 641 612 590 513 410 309 298 183 169 120 26 24 981 916 901 855 789 744 651 545 538 491 463 456 455 429 392 383 371 353 338 319 265 255 252 227 220 197 140 135 92 39 978 959 886 731 682 564 560 543 537 522 465 462 457 454 336 327 273 78 73 34 8 6 914 899 889 819 804 756 699 645 604 569 431 366 364 335 271 214 184 182 166 87 69 961 951 941 938 299 897 883 879 853 793 765 639 601 579 574 469 459 379 313 276 268 81 48 927 891 878 824 821 782 596 563 546 482 420 342 341 331 266 236 229 132 91 38 988 967 860 845 797 753 695 637 628 614 602 589 539 499 495 430 423 365 343 323 300 259 251 241 198 192 150 148 62 55 47 33 945 939 876 837 827 799 597 553 501 416 398 357 208 201 194 180 139 83 59 32 22 16 933 925 865 861 809 796 778 644 638 581 530 486 479 471 464 453 413 397 382 325 301 272 243 242 240 237 230 173 163 162 110 93 79 58 1000 995 787 777 751 742 702 657 570 483 360 223 191 178 154 108 67 64 983 977 911 832 815 761 745 662 630 505 438 426 399 387 380 369 285 189 152 100 969 968 965 947 805 784 748 706 654 633 605 550 542 519 424 394 372 334 305 282 239 234 210 177 157 134 118 57 19 18 985 966 924 862 826 792 769 696 681 659 517 498 467 412 403 401 374 316 262 196 190 171 160 141 43 14 2 946 904 893 880 868 846 843 768 728 704 665 565 562 559 508 478 351 345 314 293 284 257 211 117 991 984 957 931 930 910 839 755 898 567 548 544 419 261 193 188 186 137 95 65 37 934 902 801 788 729 725 625 607 573 402 302 292 222 68 52 993 976 960 958 766 685 632 622 555 484 460 339 308 264 217 109 36 1 989 919 866 775 594 378 367 283 181 155 101 817 746 620 584 547 527 503 440 340 332 294 258 168 142 96 50 21 979 739 734 703 664 629 575 557 488 475 428 418 396 381 256 170 143 49 12 905 840 835 807 712 608 603 526 487 461 435 432 385 350 306 287 127 74 952 842 578 359 131 115 60 964 690 679 660 643 598 421 384 344 247 144 125 9 900 874 836 813 773 772 576 566 534 504 497 390 288 270 123 922 888 848 822 727 583 571 531 518 474 318 317 235 112 94 46 992 963 940 909 885 757 617 492 480 476 411 215 111 908 714 677 670 627 623 477 427 376 232 224 104 90 85 80 51 833 776 711 466 441 391 289 175 53 970 954 936 683 680 678 668 425 130 42 20 869 849 844 532 103 652 587 295 158 783 615 506 494 944 770 280 221 75 720 707 437 279 161 971 820 750 611 667 621 616 275 102 875 780 982 864 688 524 674 395 405 568 352 406 363
*/