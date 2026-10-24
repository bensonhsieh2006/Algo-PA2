#include <iostream>
#include <cstring>
#include <fstream>
#include <vector>
#include <limits>
#include <chrono>

using namespace std;


//2D array case
void print_mps_table(int** M, int num_pts){
    cout << "Print MPS length table:" << endl;
    for (int l = num_pts - 1; l >= 0; l--){
        for (int i = 0; i < (num_pts - l); i++){
            int j = i + l;
            cout << M[j][i] << " ";
        }
        cout << endl;
    }
}

int compute_mps(int** Max, int* chords, int num_pts){
    //M(i , j) = Max(j - i, i)
    Max[0][0] = 0;
    
    for(int j = 1; j < num_pts; j++){

        int *curr_Max_row = Max[j];
        int *prev_Max_row = Max[j - 1];

        curr_Max_row[j] = 0;

        for(int i = 0; i < j; i++){
            int k = chords[j];
            if (i == k){ // Case 1, M(i, j) = M(i + 1, j - 1) + 1
                curr_Max_row[i] = prev_Max_row[i + 1] + 1;
            }
            else if((k > j) or (k < i)){ // Case 2, M(i, j) = M(i, j - 1)
                curr_Max_row[i] = prev_Max_row[i];
            }
            else{
                int inc_jk = Max[k - 1][i] + prev_Max_row[k + 1] + 1;
                int not_inc_jk = prev_Max_row[i];
                if (inc_jk >= not_inc_jk){
                    curr_Max_row[i] = inc_jk; // Case 3, M(i, j) = M(i, k - 1) + M(k + 1, j - 1) + 1
                }
                else{
                    curr_Max_row[i] = not_inc_jk; // Case 2,  M(i, j) = M(i, j - 1)
                }
            }
        }
        // print_mps_table(Max, num_pts);
    }
    return Max[num_pts - 1][0];
}

void print_mps_chords(int** Max, int* chords, vector<int>& result, int i, int j){
    if (i >= j) return;

    int k = chords[j];
    int val = Max[j][i];
    
    if (i == k) {
        result.push_back(i);
        print_mps_chords(Max, chords, result, i + 1, j - 1);
    }
    else if ((k > j) || (k < i)) {
        print_mps_chords(Max, chords, result, i, j - 1);
    }
    else {
        // Calculate the value if included chord jk
        int inc_jk = 1;
        inc_jk += Max[k - 1][i];
        inc_jk += Max[j - 1][k + 1];

        if (val == inc_jk) {
            print_mps_chords(Max, chords, result, i, k - 1);
            result.push_back(k);
            print_mps_chords(Max, chords, result, k + 1, j - 1);
        } 
        else {
            print_mps_chords(Max, chords, result, i, j - 1);
        }
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
        Max[j] = new int [j + 1];
    }
    vector<int> result;
    
    auto start_mps = chrono::steady_clock::now();
    int num_mps_chords = compute_mps(Max, chords, num_pts);
    

    // Print table results
    cout << "Num chords: " << num_mps_chords << endl;
    // print_mps_table(Max, num_pts);
    auto end_mps = chrono::steady_clock::now();
    auto duration_mps = chrono::duration_cast<chrono::milliseconds>(end_mps - start_mps);
    cout << "Calculate MPS Time: " << duration_mps.count() << " ms" << endl;


    // Print and write output results
    auto start_print = chrono::steady_clock::now();
    print_mps_chords(Max, chords, result, 0, num_pts - 1);
    fout << num_mps_chords << endl;
    for(int i = 0; i < result.size(); i++){
        fout << result[i] << " " << chords[result[i]] << endl;
    }

    auto end_print = chrono::steady_clock::now();
    auto duration_print = chrono::duration_cast<chrono::milliseconds>(end_print - start_print);
    cout << "Reconstruct Time: " << duration_print.count() << " ms" << endl;

    // Close stuff
    fin.close();
    fout.close();
    for (int i = 0; i < num_pts; i++){
        delete [] Max[i];
    }
    delete [] Max;
    delete [] chords;

    auto end = chrono::steady_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(end - start);
    cout << "Elapsed time: " << duration.count() << " ms" << endl;
    return 0;

}



// 1D array case
// void print_mps_table(int* M, size_t* row_first_index, int num_pts){
//     cout << "Print MPS length table:" << endl;
//     for (int l = num_pts - 1; l >= 0; l--){
//         for (int i = 0; i < (num_pts - l); i++){
//             int j = i + l;
//             cout << M[row_first_index[j] + i] << " ";
//         }
//         cout << endl;
//     }
// }

// int compute_mps(int* Max, size_t* row_first_index, int* chords, int num_pts){
//     //M(i , j) = Max[row_first_index[j - i] + i]
//     row_first_index[0] = 0;
//     Max[0] = 0;

//     for(int j = 1; j < num_pts; j++){
//         row_first_index[j] = row_first_index[j - 1] + j;
//         int *curr_Max_row = &Max[row_first_index[j]];
//         int *prev_Max_row = &Max[row_first_index[j - 1]];

//         curr_Max_row[j] = 0;

//         for(int i = 0; i < j; i++){
//             int k = chords[j];

//             if (i == k){ // Case 1, M(i, j) = M(i + 1, j - 1) + 1
//                 curr_Max_row[i] = prev_Max_row[i + 1] + 1;
//             }
//             else if((k > j) || (k < i)){ // Case 2, M(i, j) = M(i, j - 1)
//                 curr_Max_row[i] = prev_Max_row[i];
//             }
//             else{
//                 int inc_jk = Max[row_first_index[k - 1] + i] + prev_Max_row[k + 1] + 1;
//                 int not_inc_jk = prev_Max_row[i];
                
//                 if (inc_jk >= not_inc_jk){
//                     curr_Max_row[i] = inc_jk; // Case 3, M(i, j) = M(i, k - 1) + M(k + 1, j - 1) + 1
//                 }
//                 else{
//                     curr_Max_row[i] = not_inc_jk; // Case 2,  M(i, j) = M(i, j - 1)
//                 }
//             }
//         }
//     }
//     return Max[row_first_index[num_pts - 1]];
// }

// void print_mps_chords(int* Max, size_t* row_first_index, int* chords, vector<int>& result, int i, int j) {
//     if (i >= j) return;

//     int k = chords[j];
//     int val = Max[row_first_index[j] + (size_t)i];

//     if (i == k) {
//         result.push_back(i);
//         print_mps_chords(Max, row_first_index, chords, result, i + 1, j - 1);
//     }
//     else if ((k > j) || (k < i)) {
//         print_mps_chords(Max, row_first_index, chords, result, i, j - 1);
//     }
//     else {
//         // Calculate the value if included chord jk
//         int inc_jk = 1;
//         inc_jk += Max[row_first_index[k - 1] + (size_t)i];
//         inc_jk += Max[row_first_index[j - 1] + (size_t)(k + 1)];

//         if (val == inc_jk) {
//             print_mps_chords(Max, row_first_index, chords, result, i, k - 1);
//             result.push_back(k);
//             print_mps_chords(Max, row_first_index, chords, result, k + 1, j - 1);
//         } 
//         else {
//             print_mps_chords(Max, row_first_index, chords, result, i, j - 1);
//         }
//     }
// }

// int main(int argc, char* argv[]){
//     auto start = chrono::steady_clock::now();
//     // cout << "Int Max: " << numeric_limits<int>::max() << endl;

//     // for (int i = 0; i < argc; i++){
//     //     cout << "Arg" << i << ":" << argv[i] << endl;
//     // }

//     // Read Input
//     char buffer[200];
//     fstream fin(argv[1]);
//     fstream fout;
//     fout.open(argv[2], ios::out);
//     int pt1, pt2, num_pts;

//     fin >> num_pts;
//     // cout << "Num points: " << num_pts << endl;

//     int *chords = new int[num_pts];
//     while(fin >> pt1 >> pt2){
//         chords[pt1] = pt2;
//         chords[pt2] = pt1;
//     }
//     // for (int i = 0; i < num_pts; i++){
//     //     cout << "Point "  << i << " connects to: " << chords[i] << endl;
//     // }

//     // Create dynamic programming table. We compress the 2D table into a 1D array.
//     size_t total = (size_t)num_pts * (num_pts + 1) / 2;
//     int *Max = new int [total];
//     size_t *row_first_index = new size_t [num_pts + 1];
//     vector<int> result;
    

//     auto start_mps = chrono::steady_clock::now();
//     int num_mps_chords = compute_mps(Max, row_first_index, chords, num_pts);

//     // Print table results
//     cout << "Num chords: " << num_mps_chords << endl;
//     // print_mps_table(Max, row_first_index, num_pts);

//     auto end_mps = chrono::steady_clock::now();
//     auto duration_mps = chrono::duration_cast<chrono::milliseconds>(end_mps - start_mps);
//     cout << "Calculate MPS Time: " << duration_mps.count() << " ms" << endl;


//     // Print and write output results
//     auto start_print = chrono::steady_clock::now();
//     print_mps_chords(Max, row_first_index, chords, result, 0, num_pts - 1);
//     fout << num_mps_chords << endl;
//     for(int i = 0; i < result.size(); i++){
//         fout << result[i] << " " << chords[result[i]] << endl;
//     }

//     auto end_print = chrono::steady_clock::now();
//     auto duration_print = chrono::duration_cast<chrono::milliseconds>(end_print - start_print);
//     cout << "Reconstruct Time: " << duration_print.count() << " ms" << endl;

//     // Close stuff
//     fin.close();
//     fout.close();

//     delete [] Max;
//     delete [] chords;
//     delete [] row_first_index;


//     auto end = chrono::steady_clock::now();
//     auto duration = chrono::duration_cast<chrono::milliseconds>(end - start);
//     cout << "Elapsed time: " << duration.count() << " ms" << endl;
//     return 0;

// }