#include <iostream>
#include <cstring>
#include <fstream>
#include <vector>
#include <limits>
#include <chrono>

using namespace std;


void print_mps_table(int* M, size_t* row_first_index, int num_pts){
    cout << "Print MPS length table:" << endl;
    for (int j = (num_pts - 1); j >= 0; j--){
        for (int i = 0; i < (num_pts - j); i++){
            size_t index = row_first_index[j] + i;
            cout << M[index] << " ";
        }
        cout << endl;
    }
}

int compute_mps(int* Max, int* P, size_t* row_first_index, int* chords, int num_pts){
    //M(i , j) = Max(j - i, i)
    row_first_index[0] = 0;
    for(int i = 0; i < num_pts; i++){
        Max[i] = 0;
    }

    for(int j_index = 1; j_index < num_pts; j_index++){
        row_first_index[j_index] = row_first_index[j_index - 1] + (num_pts - j_index + 1);

        int *curr_Max_row = &Max[row_first_index[j_index]];
        int *curr_P_row = &P[row_first_index[j_index]];
        int *prev_Max_row = &Max[row_first_index[j_index - 1]];

        for(int i = 0; i < (num_pts - j_index); i++){
            
            int j = j_index + i;
            int k = chords[j];

            if (i == k){ // Case 1, stored as -1
                curr_Max_row[i] = prev_Max_row[i + 1] + 1;
                curr_P_row[i] = -1;
            }
            else if((k > j) or (k < i)){ // Case 2, stored as -2
                curr_Max_row[i] = prev_Max_row[i];
                curr_P_row[i] = -2;
            }
            else{
                int d1 = k - i - 1;
                int d2 = j - k - 2;
                int inc_jk = Max[row_first_index[d1] + i] + Max[row_first_index[d2] + k + 1] + 1;
                int not_inc_jk = prev_Max_row[i];
                
                if (inc_jk >= not_inc_jk){
                    curr_Max_row[i] = inc_jk; // Case 3, store the value k
                    curr_P_row[i] = k;
                }
                else{
                    curr_Max_row[i] = not_inc_jk; // Case 2, stored as -2
                    curr_P_row[i] = -2;
                }
            }
        }
    }
    size_t last_index = (size_t)num_pts * (num_pts + 1) / 2 - 1;
    return Max[last_index];
}

void print_mps_chords(int* P, size_t* row_first_index, int* chords, vector<int>& result, int i, int j, int num_pts){
    if (i >= j){
        return;
    }

    int j_index = j - i;
    size_t p_index = row_first_index[j_index] + i;
    
    switch (P[p_index]){
        case -1:
            // cout << i << " " << chords[i] << endl;
            result.push_back(i);
            print_mps_chords(P, row_first_index, chords, result, i + 1, j - 1, num_pts);
            break;

        case -2:
            print_mps_chords(P, row_first_index, chords, result, i, j - 1, num_pts);
            break;

        default:
            int k = P[p_index];
            print_mps_chords(P, row_first_index, chords, result, i, k - 1, num_pts);
            // cout << chords[j] << " " << j << endl;
            result.push_back(chords[j]);
            print_mps_chords(P, row_first_index, chords, result, k + 1, j - 1, num_pts);
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
    size_t total = (size_t)num_pts * (num_pts + 1) / 2;
    int *Max = new int [total];

    // The path table that records the case number for each M[i, j]. If it falls in Case 3, store the value k. We store the best path for M[i, j + i] = P[j][i] for aim of better efficiency.
    int *P = new int [total];

    size_t *row_first_index = new size_t [num_pts + 1];
    
    auto start_mps = chrono::steady_clock::now();
    int num_mps_chords = compute_mps(Max, P, row_first_index, chords, num_pts);
    

    // Print table results
    cout << "Num chords: " << num_mps_chords << endl;
    // print_mps_table(Max, row_first_index, num_pts);
    // cout << "Print case table:" << endl;
    // for (int j = (num_pts - 1); j >= 0; j--){
    //     for (int i = 0; i < (num_pts - j); i++){
    //         size_t index = row_first_index[j] + i;
    //         cout << P[index] << " ";
    //     }
    //     cout << endl;
    // }
    auto end_mps = chrono::steady_clock::now();
    auto duration_mps = chrono::duration_cast<chrono::milliseconds>(end_mps - start_mps);
    cout << "Calculate MPS Time: " << duration_mps.count() << " ms" << endl;


    // Print and write output results
    auto start_print = chrono::steady_clock::now();
    vector<int> result;
    print_mps_chords(P, row_first_index, chords, result, 0, num_pts - 1, num_pts);
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

    delete [] Max;
    delete [] chords;
    delete [] row_first_index;
    delete [] P;


    auto end = chrono::steady_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(end - start);
    cout << "Elapsed time: " << duration.count() << " ms" << endl;
    return 0;

}