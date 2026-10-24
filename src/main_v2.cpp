#include <iostream>
#include <cstring>
#include <fstream>
#include <vector>
#include <limits>
#include <chrono>

using namespace std;


void print_mps_table(int** M, int num_pts){
    cout << "Print MPS length table:" << endl;
    for (int j = (num_pts - 1); j >= 0; j--){
        for (int i = 0; i < (num_pts - j); i++){
            cout << M[j][i] << " ";
        }
        cout << endl;
    }
}

int compute_mps(int** Max, int** P, int* chords, int num_pts){
    //M(i , j) = Max(j - i, i)
    for(int i = 0; i < num_pts; i++){
        Max[0][i] = 0;
    }
    for(int j_index = 1; j_index < num_pts; j_index++){
        for(int i = 0; i < (num_pts - j_index); i++){
            int j = j_index + i;
            int k = chords[j];
            // cout << "(i, j, k): (" << i << ", " << j << ", " << k << ")" << endl;
            if (i == k){ // Case 1, stored as -1
                Max[j_index][i] = Max[j_index - 1][i + 1] + 1;
                P[j_index][i] = -1;
            }
            else if((k > j) or (k < i)){ // Case 2, stored as -2
                Max[j_index][i] = Max[j_index - 1][i];
                P[j_index][i] = -2;
            }
            else{
                int inc_jk = Max[k - i - 1][i] + Max[j - k - 2][k + 1] + 1;
                int not_inc_jk = Max[j_index - 1][i];
                if (inc_jk >= not_inc_jk){
                    Max[j_index][i] = inc_jk; // Case 3, store the value k
                    P[j_index][i] = k;
                }
                else{
                    Max[j_index][i] = not_inc_jk; // Case 2, stored as -2
                    P[j_index][i] = -2;
                }
            }
        }
        // print_mps_table(Max, num_pts);
    }
    return Max[num_pts - 1][0];
}

void print_mps_chords(int** P, int* chords, vector<int>& result, int i, int j, int num_pts){
    if (i >= j){
        return;
    }

    int j_index = j - i;
    
    switch (P[j_index][i]){
        case -1:
            // cout << i << " " << chords[i] << endl;
            result.push_back(i);
            print_mps_chords(P, chords, result, i + 1, j - 1, num_pts);
            break;

        case -2:
            print_mps_chords(P, chords, result, i, j - 1, num_pts);
            break;

        default:
            int k = P[j_index][i];
            print_mps_chords(P, chords, result, i, k - 1, num_pts);
            // cout << chords[j] << " " << j << endl;
            result.push_back(chords[j]);
            print_mps_chords(P, chords, result, k + 1, j - 1, num_pts);
    }
}

int main(int argc, char* argv[]){
    auto start = chrono::steady_clock::now();
    // cout << "Int Max: " << numeric_limits<int>::max() << endl;

    // for (int i = 0; i < argc; i++){
    //     cout << "Arg" << i << ":" << argv[i] << endl;
    // }

    // Read Input
    char buffer[200];
    fstream fin(argv[1]);
    fstream fout;
    fout.open(argv[2], ios::out);
    int pt1, pt2, num_pts;

    fin >> num_pts;
    // cout << "Num points: " << num_pts << endl;

    int *chords = new int[num_pts];
    while(fin >> pt1 >> pt2){
        chords[pt1] = pt2;
        chords[pt2] = pt1;
    }
    // for (int i = 0; i < num_pts; i++){
    //     cout << "Point "  << i << " connects to: " << chords[i] << endl;
    // }

    // Create dynamic programming table. We store M[i, j + i] = Max[j][i] for aim of better efficiency.
    int **Max = new int *[num_pts];
    for (int j = 0; j < num_pts; j++){
        Max[j] = new int [num_pts - j];
    }

    // The path table that records the case number for each M[i, j]. If it falls in Case 3, store the value k. We store the best path for M[i, j + i] = P[j][i] for aim of better efficiency.
    int **P = new int *[num_pts];
    for (int j = 0; j < num_pts; j++){
        P[j] = new int [num_pts - j];
    }
    
    auto start_mps = chrono::steady_clock::now();
    int num_mps_chords = compute_mps(Max, P, chords, num_pts);
    

    // Print table results
    cout << "Num chords: " << num_mps_chords << endl;
    // print_mps_table(Max, num_pts);
    // cout << "Print case table:" << endl;
    // for (int j = (num_pts - 1); j >= 0; j--){
    //     for (int i = 0; i < (num_pts - j); i++){
    //         cout << P[j][i] << " ";
    //     }
    //     cout << endl;
    // }
    auto end_mps = chrono::steady_clock::now();
    auto duration_mps = chrono::duration_cast<chrono::milliseconds>(end_mps - start_mps);
    cout << "Calculate MPS Time: " << duration_mps.count() << " ms" << endl;


    // Print and write output results
    auto start_print = chrono::steady_clock::now();
    vector<int> result;
    print_mps_chords(P, chords, result, 0, num_pts - 1, num_pts);
    fout << num_mps_chords << endl;
    for(int i = 0; i < result.size(); i++){
        fout << result[i] << " " << chords[result[i]] << endl;
    }

    auto end_print = chrono::steady_clock::now();
    auto duration_print = chrono::duration_cast<chrono::milliseconds>(end_print - start_print);
    cout << "Print Time: " << duration_print.count() << " ms" << endl;

    // Close stuff
    fin.close();
    fout.close();
    for (int i = 0; i < num_pts; i++){
        delete [] Max[i];
    }
    for (int i = 0; i < num_pts; i++){
        delete [] P[i];
    }
    delete [] Max;
    delete [] P;
    delete [] chords;

    auto end = chrono::steady_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(end - start);
    cout << "Elapsed time: " << duration.count() << " ms" << endl;
    return 0;

}