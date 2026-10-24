#include <iostream>
#include <fstream>
#include <cstdlib>
using namespace std;


// Compile & Execute:
// g++ -O2 generate.cpp -o generate
// ./generate <case>

void help_message(){
    cout << "./generate <case>" << endl;
    cout << "case == 0: generate random chords" << endl;
    cout << "case == 1: chords 0-1, 2-3, ..., 179998-179999" << endl;
    cout << "case == 2: chords 0-90000, 1-90001, ..., 89999-179999" << endl;
    cout << "case == 3: chords 1-2, 3-4, ..., 179999-0" << endl;
    cout << "case == 4: chords 1-4, 3-6, 5-8, ... , 179995-179998, 179997-179999, 0-2" << endl;
    cout << "case == 5: chords 0-179999, 1-179998, ..., 89999-90000" << endl;
}

void generate_random_input(){
    fstream fout;
    srand(20);
    fout.open("./inputs/P180000_rand.in", ios::out);
    if (!fout.is_open()) {
        cout << "cannot open file." << endl; 
        return;
    }
    fout << 180000 << endl;
    int node2[180000] = {};
    int line = 0;
    int curNode = 0;
    int setNode = 0;
    while(line != 90000) {
        if(node2[curNode] == 1) {
            curNode++;
            continue;
        }

        setNode = rand() % (180000 - curNode - 1) + curNode + 1;
        while(node2[setNode] == 1) {
            setNode = rand() % (180000 - curNode - 1) + curNode + 1;
        }
        node2[setNode] = 1;
        fout << curNode << " " << setNode << endl;
        curNode++;
        line++;
    }
    fout << 0;
    fout.close();
}

void generate_worst_case_1(){
    int N = 90000;
    fstream fout;
    srand(20);
    fout.open("./inputs/P180000.in", ios::out);
    if (!fout.is_open()) {
        cout << "cannot open file." << endl; 
        return;
    }
    fout << 2 * N << endl;
    int node2[2 * N] = {};
    int line = 0;
    int curNode = 0;
    int setNode = 0;
    for(int i = 0; i < N; i++){
        fout << 2 * i << " " << 2 * i + 1 << endl;
    }
    fout << 0;
    fout.close();
}


void generate_worst_case_2(){
    fstream fout;
    srand(20);
    fout.open("./inputs/P180000_2.in", ios::out);
    if (!fout.is_open()) {
        cout << "cannot open file." << endl; 
        return;
    }
    fout << 180000 << endl;
    int node2[180000] = {};
    int line = 0;
    int curNode = 0;
    int setNode = 0;
    for(int i = 0; i < 90000; i++) {
        fout << i << " " << i + 90000 << endl;
    }
    fout << 0;
    fout.close();
}


void generate_worst_case_3(){
    int N = 90000;
    fstream fout;
    srand(20);
    fout.open("./inputs/P180000_3.in", ios::out);
    if (!fout.is_open()) {
        cout << "cannot open file." << endl; 
        return;
    }
    fout << 2 * N << endl;
    int node2[2 * N] = {};
    int line = 0;
    int curNode = 0;
    int setNode = 0;
    for(int i = 1; i < N; i++){
        fout << 2 * i - 1 << " " << 2 * i << endl;
    }
    fout << 179999 << " " << 0 << endl;
    fout << 0;
    fout.close();
}

void generate_worst_case_4(){
    int N = 90000;
    fstream fout;
    srand(20);
    fout.open("./inputs/P180000_4.in", ios::out);
    if (!fout.is_open()) {
        cout << "cannot open file." << endl; 
        return;
    }
    fout << 2 * N << endl;
    int node2[2 * N] = {};
    int line = 0;
    int curNode = 0;
    int setNode = 0;
    for(int i = 1; i < N - 1; i++){
        fout << 2 * i - 1 << " " << 2 * (i + 1) << endl;
    }
    fout << 0 << " " << 2 << endl;
    fout << 179997 << " " << 179999 << endl;
    fout << 0;
    fout.close();
}

void generate_worst_case_5(){
    fstream fout;
    srand(20);
    fout.open("./inputs/P180000_5.in", ios::out);
    if (!fout.is_open()) {
        cout << "cannot open file." << endl; 
        return;
    }
    fout << 180000 << endl;
    int node2[180000] = {};
    int line = 0;
    int curNode = 0;
    int setNode = 0;
    for(int i = 0; i < 90000; i++) {
        fout << i << " " << 179999 - i << endl;
    }
    fout << 0;
    fout.close();
}

int main(int argc, char* argv[]){
    switch(argv[1][0]){
        case '0':
            generate_random_input();
            break;
        case '1':
            generate_worst_case_1();
            break;
        case '2':
            generate_worst_case_2();
            break;  
        case '3':
            generate_worst_case_3();
            break;
        case '4':
            generate_worst_case_4();
            break;  
        case '5':
            generate_worst_case_5();
            break;
        default:
            help_message();
    }
    
}