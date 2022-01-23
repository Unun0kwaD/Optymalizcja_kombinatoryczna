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

using namespace std;
class myComparator
{
public:
    int operator() (const pair<int,pair<int,int>>& p1, const pair<int,pair<int,int>>& p2)
    {
        return p1.first > p2.first;
    }
};
typedef priority_queue<pair<int,pair<int,int>>,vector<pair<int,pair<int,int>>>,myComparator> prique;
int main(){
    prique kolej;
    kolej.push({2,{23,44}});
    kolej.push({4,{8783,12}});
    kolej.push({5,{643,54}});
    cout<<kolej.top().first<<endl;
    kolej.pop();
    cout<<kolej.top().first<<endl;
    kolej.pop();
    cout<<kolej.top().first<<endl;
    kolej.pop();

}