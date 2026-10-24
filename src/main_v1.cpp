#include <iostream>
#include <cstring>
#include <fstream>
#include <vector>
#include <limits>
#include <chrono>

using namespace std;

int compute_mps(int** M, int** P, int* chords, int num_pts){
    auto start = chrono::steady_clock::now();
    for(int i = 0; i < num_pts; i++){
        M[i][num_pts - 1 - i] = 0;
    }
    for(int l = 2; l <= num_pts; l++){
        for(int i = 0; i <= (num_pts - l); i++){
            int j = i + l - 1;
            int j_index = num_pts - 1 - j;
            int k = chords[j];

            if (i == k){ // Case 1, stored as -1
                M[i][j_index] = M[i + 1][j_index + 1] + 1;
                P[i][j_index] = -1;
            }
            else if((k > j) or (k < i)){ // Case 2, stored as -2
                M[i][j_index] = M[i][j_index + 1];
                P[i][j_index] = -2;
            }
            else{
                int inc_jk = M[i][num_pts - k] + M[k + 1][num_pts - j] + 1;
                int not_inc_jk = M[i][j_index + 1];
                if (inc_jk >= not_inc_jk){
                    M[i][j_index] = inc_jk; // Case 3, store the value k
                    P[i][j_index] = k;
                }
                else{
                    M[i][j_index] = not_inc_jk; // Case 2, stored as -2
                    P[i][j_index] = -2;
                }
            }
        }
    }

    auto end = chrono::steady_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(end - start);
    cout << "Compute_MPS Time: " << duration.count() << " ms" << endl;
    return M[0][0];
}

void print_mps_chords(int** P, int* chords, vector<int>& result, int i, int j, int num_pts){
    if (i >= j){
        return;
    }
    
    int j_index = num_pts - 1 - j;
    switch (P[i][j_index]){
        case -1:
            // cout << i << " " << chords[i] << endl;
            result.push_back(i);
            print_mps_chords(P, chords, result, i + 1, j - 1, num_pts);
            break;

        case -2:
            print_mps_chords(P, chords, result, i, j - 1, num_pts);
            break;

        default:
            int k = P[i][j_index];
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

    // Create dynamic programming table.
    int **M = new int *[num_pts];
    for (int i = 0; i < num_pts; i++){
        M[i] = new int [num_pts - i];
    }

    // The path table that records the case number for each M[i, j]. If it falls in Case 3, store the value k.
    int **P = new int *[num_pts];
    for (int i = 0; i < num_pts; i++){
        P[i] = new int [num_pts - i];
    }
    
    int num_mps_chords = compute_mps(M, P, chords, num_pts);
    
    // Print table results
    cout << "Num chords: " << num_mps_chords << endl;
    // cout << "Print MPS length table:" << endl;
    // for (int j = 0; j < num_pts; j++){
    //     for (int i = 0; i < (num_pts - j); i++){
    //         cout << M[i][j] << " ";
    //     }
    //     cout << endl;
    // }
    // cout << "Print case table:" << endl;
    // for (int j = 0; j < num_pts; j++){
    //     for (int i = 0; i < (num_pts - j); i++){
    //         cout << P[i][j] << " ";
    //     }
    //     cout << endl;
    // }

    // Print and write output results
    vector<int> result;
    print_mps_chords(P, chords, result, 0, num_pts - 1, num_pts);
    fout << num_mps_chords << endl;
    for(int i = 0; i < result.size(); i++){
        fout << result[i] << " " << chords[result[i]] << endl;
    }

    // Close stuff
    fin.close();
    fout.close();
    for (int i = 0; i < num_pts; i++){
        delete [] M[i];
    }
    for (int i = 0; i < num_pts; i++){
        delete [] P[i];
    }
    delete [] M;
    delete [] P;
    delete [] chords;

    auto end = chrono::steady_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(end - start);
    cout << "Elapsed time: " << duration.count() << " ms" << endl;
    return 0;

}