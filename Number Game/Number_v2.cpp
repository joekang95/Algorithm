#include<iostream>
#include<vector>
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<fstream>

using namespace std;

void read(string &name, int &N, int &n);            //function prototype
void random(vector<int> &A, int &N, int &n);
void worstcase(vector<int> &A, int &N, int &n);
bool hybrid(int , int , int , vector<int> &A, vector<int> &a, int );
static int ptr = 0;                                 //initial pointer for ans in function hybrid
int n = 15;                                         //initial n
int N = 2*n - 1;                                    //initial N
vector<int> input(N);                               //initial vector input size N
vector<int> ans(n);                                 //initial vector ans size n
vector<int> tmp(n);                                 //initial vector temp size n
vector< vector<int> > T(n+1, tmp);                  //initial vector T size n+1 * n
fstream fin;                                        //fin for read file
fstream fout;                                       //fout for output file

int main(){

    /******************************
    This part for reading input.txt
    *******************************/
    string infile = "input.txt";                    //input file name
    read(infile, N, n);                             //call function read

    /**************************
    This part for changing n
    1 <= n <= 1000
    **************************/
//    double START, END;
//    n = 10;                                       //set n
//    N = 2*n - 1;                                  //N = 2n - 1

    /**********************************
    This part for choosing
    Worst Case or Average(Random) Case
    **********************************/
//    //worstcase(input, N, n);                     //call function worst case
//    random(input, N, n);                          //call function random (average case)

    /************************************
    This part for backtracking+DP
    ************************************/
//    START = clock();                              //start time
//    cout << "Hybrid..." << endl;
//    hybrid(N, n, 0, input, ans, n);               //call function hybrid
//    END = clock();                                //end time
//    for(int i = 0 ; i < n ; i++){                 //print answer
//        cout << ans[i] << " ";
//    }                                             //print time used by function hybrid
//    cout << "\nHybrid: " << (END - START)/CLOCKS_PER_SEC << "s" << endl;

    system("pause");
}

/********************************
Function Read
1.Read input.txt
2.Use Function Hybrid to find
3.Output answer to output.txt
Input: file name, N = 2n-1, n
Output: output.txt
********************************/
void read(string &name, int &N, int &n){
    fin.open(name, ios::in);
    fout.open("output.txt", ios::out);
    if(!fin){                                           //if cannot find input file
        cout << "Can't Find File!" << endl;
    }
    else{
        int number;                                     //integer number to store input numbers
        int temp;                                       //integer temp to store n
        while(fin >> temp){
            n = temp;                                   //n equals to temp
            if( n > 1000 || n < 0){                     //n is between 0 to 1000
                cout << "Size Cannot be Larger than 1000 or Smaller than 0!";
                break;
            }
            N = 2*n - 1;                                //N = 2n - 1
            vector<int> read_input(N);                  //set vector size of N
            vector<int> read_ans(n);                    //set vector size of n
            for(int i = 0 ; i < N ; i++){               //store input numbers to vector read_input
                fin >> number;
                read_input[i] = number;
                cout << read_input[i] << " ";
            }
            cout << endl;
            for(int i = 0 ; i < n+1 ; i++){             //clear T[j,k] for loop usage
                for(int j = 0; j < n ; j++){
                    T[i][j] = 0;
                }
            }
            ptr = 0;                                    //reset pointer
            hybrid(N, n, 0, read_input, read_ans, n);   //call function hybrid
            for(int i = 0 ; i < n ; i++){               //output answer from read_ans
                fout << read_ans[i] << " ";
                cout << read_ans[i] << " ";
            }
            fout << endl << endl;
            cout << endl << endl;
        }
    }
}

/*********************************
Function Worst Case
There are:
    1. n-1 of n-2
    2. n-1 of n-1
    3. 1 of n
Input: vector input, N = 2n-1, n
**********************************/
void worstcase(vector<int> &A, int &N, int &n){
    for(int i = 0 ; i < N ; i++){
        if(i < n - 1){                                  // n - 1 of n - 2
            A[i] = n - 2;
        }
        else if(i >= n-1 && i < N - 1){                 // n - 1 of n - 1
            A[i] = n - 1;
        }
        else if(i = N - 1){                             // 1 of n
            A[i] = n;
        }
    }


}

/********************************
Function Random
1. Random numbers
2. Range from 1 to n
Input: vector input, N = 2n-1, n
*********************************/
void random(vector<int> &A, int &N, int &n){
    srand(time(NULL));
    for(int i = 0 ; i < N ; i++){
        int temp = rand()%n + 1;                        //random numbers between 1 to n
        A[i] = temp;                                    //store into vector
    }
}

/*******************************************
Function Hybrid
1. Backtracking+Dynamic Programming
2. Recursive Function
3. T[j,k] to record i point
4. P[i,j,k]=
    P[i-1,j,k] v
    P[i-1,j-1,(k-A[i])%n]
Input: i, j, k, vector input, vector ans, n
********************************************/
bool hybrid(int i , int j , int k, vector<int> &A, vector<int> &a, int n){
    if(j > i){                                          //if j > i return false since can't take more than i
        return 0;
    }
    if(j == 0){
        if(k == 0){                                     //if j = 0 and k = 0 return true since take none than k = 0
            return 1;
        }
        else{                                           //k must be 0 so return false
            return 0;
        }
    }
    if(i <= T[j][k]){                                   //if i <= T[j,k] , return false since T[j,k] stores the upper bound i of false of P[i,j,k]
        return 0;
    }
    if(hybrid(i-1, j, k, A, a, n)){                     //if i is not taken, and is makes sense (true), return true
        return 1;
    }
    if(hybrid(i-1, j-1, (k+n-A[i-1])%n, A, a, n)){      //if i is taken, and the rest makes sense (true), return true and store the number to answer
        a[ptr] = A[i-1];
        ptr++;
        return 1;
    }
    T[j][k] = i;                                        //set upper bound of false of P[i,j,k] so T[j,k] = i
    return 0;                                           //return false;
}
