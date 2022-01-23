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
int kolory[1000],maxkolor[1000];
using namespace std;
int W=10;


int main(int argc, char* argv[]){
    int s=2,n=10;
    for (int i=0;i<n+10;i++){
        kolory[i]=i+10;
    }
    memset(&kolory[s],0,sizeof(n)*(n-s));
    for (int i=0;i<n;i++){
        cout<<kolory[i]<<" ";
    }
    cout<<endl;
    char zajete[W+1];
    memset(zajete,0,sizeof(zajete));
    for (int j=0;j<=W;j++){
        if(!zajete[j])
            cout<<"0 ";
    }
}