#include<iostream>
#include<vector>
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<fstream>

using namespace std;

void random(vector<int> &A, int &N, int &n);
void worstcase(vector<int> &A, int &N, int &n);
void dynamic_programming(vector<int> &A, vector<int> &a, int &N, int &n);
bool hybrid(int , int , int , vector<int> &A, vector<int> &a, int );
int back_tracking(int, int, int, int, vector<int> &A, int, vector<int> &a);
void read(string &name, int &N, int &n);
static int ptr = 0;
int n = 15;
int N = 2*n - 1;
vector<int> input(N);
vector<int> ans(n);
vector<int> tmp(n);
vector< vector<int> > T(n+1, tmp);
fstream fin;
fstream fout;
int main(){

    /******************************
    This part for reading input.txt
    *******************************/
    string infile = "input.txt";
    read(infile, N, n);

//    /**************************
//    This part for changing n
//    **************************/
//    double START, END;
//    n = 10;
//    N = 2*n - 1;
//
//    /**********************************
//    This part for choosing
//    Worst Case or Average(Random) Case
//    **********************************/
//    //worstcase(input, N, n);
//    random(input, N, n);
//
//    /***********************************
//    This part for backtracking
//    ***********************************/
//    cout << "Back Tracking..." << endl;
//    START = clock();
//    back_tracking(n, n, 0, N, input, 0, ans);
//    END = clock();
//    for(int i = 0 ; i < n ; i++){
//        cout << ans[i] << " ";
//    }
//    cout << "\nBack Tracking: " << (END - START)/CLOCKS_PER_SEC << "s" << endl;
//
//    /*************************************
//    This part for dynamic programming
//    *************************************/
//    START = clock();
//    dynamic_programming(input, ans, N, n);
//    END = clock();
//    for(int i = 0 ; i < n ; i++){
//        cout << ans[i] << " ";
//    }
//    cout << "\nDynamic Programming: " << (END - START)/CLOCKS_PER_SEC << "s" << endl;
//
//    /************************************
//    This part for backtracking+DP
//    ************************************/
//    START = clock();
//    cout << "Hybrid..." << endl;
//    hybrid(N, n, 0, input, ans, n);
//    END = clock();
//    for(int i = 0 ; i < n ; i++){
//        cout << ans[i] << " ";
//    }
//    cout << "\nHybrid: " << (END - START)/CLOCKS_PER_SEC << "s" << endl;

    system("pause");
}

/********************************
Function Read
1.Read input.txt
2.Use Function Hybrid to find
3.Output answer to output.txt
********************************/
void read(string &name, int &N, int &n){
    fin.open(name, ios::in);
    fout.open("output.txt", ios::out);
    if(!fin){                                           //if cannot find input file
        cout << "Can't Find File!" << endl;
    }
    else{
        int number;
        int temp;
        while(fin >> temp){
            n = temp;
            if( n > 1000 || n < 0){
                cout << "Size Cannot be Larger than 1000 or Smaller than 0!";
                break;
            }
            N = 2*n - 1;
            vector<int> read_input(N);
            vector<int> read_ans(n);
            for(int i = 0 ; i < N ; i++){
                fin >> number;
                read_input[i] = number;
                cout << read_input[i] << " ";
            }
            cout << endl;
            for(int i = 0 ; i < n+1 ; i++){
                for(int j = 0; j < n ; j++){
                    T[i][j] = 0;
                }
            }
            ptr = 0;
            hybrid(N, n, 0, read_input, read_ans, n);
            for(int i = 0 ; i < n ; i++){
                fout << read_ans[i] << " ";
                cout << read_ans[i] << " ";
            }
            fout << endl << endl;
            cout << endl << endl;
        }
    }
}

/************************
Function Worst Case
There are:
    1. n-1 of n-2
    2. n-1 of n-1
    3. 1 of n
************************/
void worstcase(vector<int> &A, int &N, int &n){
    for(int i = 0 ; i < N ; i++){
        if(i < n - 1){
            A[i] = n - 2;
        }
        else if(i >= n-1 && i < N - 1){
            A[i] = n - 1;
        }
        else if(i = N - 1){
            A[i] = n;
        }
    }


}

/************************
Function Random
1. Random numbers
2. Range from 1 to n
************************/
void random(vector<int> &A, int &N, int &n){
    srand(time(NULL));
    for(int i = 0 ; i < N ; i++){
        int temp = rand()%n + 1;
        A[i] = temp;
    }
}

/************************
Function Backtracking
1. r = remaining numbers
2. s = starting point
3. e = ending point
************************/
int back_tracking(int n, int r, int s, int e, vector<int> &A, int sum, vector<int> &a){
    //r = remain, s = start, e = end
    if(e - s < r){
        return 1;
    }
    if(r == 1){
        for(int i = s ; i < e ; i++){
            if(((sum + A[i])%n) == 0){
                a[n-1] = A[i];
                return 0;
            }
        }
    }
    for(int i = s ; i < e ; i++){
        int value = back_tracking(n, r-1, i+1, e, A, sum+A[i], a);
        if(value == 0){
            a[n-r] = A[i];
            return 0;
        }
    }
    return 1;
}

/**********************************
Function Dynamic Programming
1. Set table P[i,j,k]
2. P[i,j,k]=
    P[i-1,j,k] v
    P[i-1,j-1,(k-A[i])%n]
3. Select by table P
**********************************/
void dynamic_programming(vector<int> &A, vector<int> &a, int &N, int &n){
    bool P[N+1][n+1][n];
    cout << "Dynamic Programming..." << endl;
    for(int i = 0 ; i <= N ; i++){
        for(int j = 0 ; j <= n ; j++){
            for(int k = 0 ; k < n ; k++){
                if(j > i){
                    P[i][j][k] = false;
                }
                else if(j == 0){
                    if(k == 0){
                        P[i][j][k] = true;
                    }
                    else{
                        P[i][j][k] = false;
                    }
                }
                else if(P[i-1][j][k] == true){
                    P[i][j][k] = true;
                }
                else if(i > 0){
                    if(P[i-1][j-1][(k + n - A[i-1])%n] == true){
                        P[i][j][k] = true;
                    }
                    else{
                        P[i][j][k] = false;
                    }
                }
                else{
                    P[i][j][k] = false;
                }
            }
        }
    }
    int i = N;
    int j = n;
    int k = 0;
    while(i > 0){
        if(P[i-1][j][k] == true){
            i--;
        }

        else if(P[i-1][j-1][(k + n - A[i-1])%n] == true){
            a[j-1] = A[i-1];
            k = (k + n - A[i-1])%n;
            i--;
            j--;
        }

    }
}

/*************************************
Function Hybrid
1. Backtracking+Dynamic Programming
2. Recursive Function
3. T[j,k] to record i point
4. P[i,j,k]=
    P[i-1,j,k] v
    P[i-1,j-1,(k-A[i])%n]
*************************************/
bool hybrid(int i , int j , int k, vector<int> &A, vector<int> &a, int n){
    if(j > i){
        return 0;
    }
    if(j == 0){
        if(k == 0){
            return 1;
        }
        else{
            return 0;
        }
    }
    if(i <= T[j][k]){
        return 0;
    }
    if(hybrid(i-1, j, k, A, a, n)){
        return 1;
    }
    if(hybrid(i-1, j-1, (k+n-A[i-1])%n, A, a, n)){
        a[ptr] = A[i-1];
        ptr++;
        return 1;
    }
    T[j][k] = i;
    return 0;
}


