#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <string.h>
#include <chrono>
/* TODO:
    mo¿liwoœæ tworzenia n plików testowych o rozmiarach z danego zakresu
*/
using namespace std;

int main(int argc, char* argv[]){
    srand((unsigned) time(0));
    int choice, size;
    int first[300];
    int second[300];
    while (true){
    cout << "What do you want to do?\n";
    cout << "1. Generate new file\n";
    cout << "2. Import data.txt file\n";
    cout << "Any other: Exit\n\n";
    cin >> choice;
    int x = 0,a,b;

    if (choice == 1) {
        cout << "Choose your size: \n";
        cin >> size;
        ofstream infile(argv[1]);
        infile<<size<<endl;
        for (int i = 0; i < size; i++) {
            int random = rand() % (size / 3);
            int temp =i+1;
            while(temp<size) {
                first[i] = i + 1;
                temp += rand() % 3 + 1;
                if (temp>=size) break;
                second[i] = temp;
                cout<<first[i]<<" "<<second[i]<<endl;
                infile<<first[i]<<" "<<second[i]<<endl;
            }
        }
        infile.close();
    }
    else if (choice == 2) {
        std::ifstream infile(argv[1]);
        int a, b;
        infile>>a;
        size = a;
       // cout<<a<<endl;
        while ((infile >> a >> b)){
            first[x] = a;
            second[x++] = b;
            //cout<<a<<" "<<b<<endl;
        }
       // for (int i=0;i<x;i++){
          //  cout<<first[i]<<"\t"<<second[i]<<endl;}
        infile.close();
    }
    else
        return 0;
    }

}