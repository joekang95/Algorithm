#include <iostream>
#include <vector>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <cmath>
#include <fstream>
using namespace std;

void normal_multiplication(vector< vector<int> > &m1, vector< vector<int> > &m2, vector< vector<int> > &fin, int &n);
void strassen_add(vector< vector<int> > &A, vector< vector<int> > &B, vector< vector<int> > &C, int &n);
void strassen_subtract(vector< vector<int> > &A, vector< vector<int> > &B, vector< vector<int> > &C, int &n);
void strassen_prepare(vector< vector<int> > &A, vector< vector<int> > &B, vector< vector<int> > &C, int &n);
void strassen_algorithm(vector< vector<int> > &A, vector< vector<int> > &B, vector< vector<int> > &C, int &n);
void random(vector< vector<int> > &m, int &n);
void read(string);
void test_thr();
void test_c();
void print(vector< vector<int> >, int);
int c = 0;
int counter = 0;
int n;
int loop = 0;
string outname;
int thr = 64;
bool test;
fstream fout;
fstream fin;

int main(int argc, char* argv[]){
    int select;
    test = false;
    cout << "Please Select: (1) Read File input.txt (2) Test Threshold (3) Test C" << endl;
    cin >> select;
    if(select == 1){
        string infile = "input.txt";
        read(infile);
    }
    else if(select == 2){
        test = false;
        do{
            cout << "Please Enter N(0 <= N <= 5000): ";
            cin >> n;
        }while(n > 5000 || n < 0);
        cout << "Please Enter Initial Threshold: ";
        cin >> thr;
        do{
            cout << "Please Enter Loop Times(0< loop <= 10): ";
            cin >> loop;
        }while(loop < 0 || loop > 10);
        cout << "Please Enter Output Name(XXX.txt): ";
        cin >> outname;
        test_thr();
    }
    else if(select == 3){
        test = true;
        do{
            cout << "Please Enter N(0 <= N <= 5000): ";
            cin >> n;
        }while(n > 5000 || n < 0);
        cout << "Please Enter C: ";
        cin >> c;
        do{
            cout << "Please Enter Loop Times(0< loop <= 10): ";
            cin >> loop;
        }while(loop < 0 || loop > 10);
        cout << "Please Enter Output Name(XXX.txt): ";
        cin >> outname;
        test_c();
    }
    system("pause");
    return 0;
}
/***********************************
Function test_thr
1.test the threshold when n is same
***********************************/
void test_thr(){
    float START, END;
    vector<int> row(n);
    vector< vector<int> > matrix1(n, row);
    random(matrix1, n);
    vector< vector<int> > matrix2(n, row);
    random(matrix2, n);
    vector< vector<int> > final_matrix(n, row);
    fstream test;
    test.open(outname, ios::out);
    START = clock();
    normal_multiplication(matrix1, matrix2, final_matrix, n);
    END = clock();
    cout << "done normal" << endl;
    test << "normal: " << (END - START)/CLOCKS_PER_SEC << "s" << endl;
    START = clock();
    strassen_prepare(matrix1, matrix2, final_matrix, n);
    END = clock();
    test << "strassen: " << "thr = " << thr << "\t" << (END - START)/CLOCKS_PER_SEC << "s" << endl;
    float minimum = (END - START)/CLOCKS_PER_SEC;
    int minthr;
    for(int i = 0 ; i < loop - 1 ; i++){
        thr*=2;
        START = clock();
        strassen_prepare(matrix1, matrix2, final_matrix, n);
        END = clock();
        if((END - START)/CLOCKS_PER_SEC < minimum){
            minimum = (END - START)/CLOCKS_PER_SEC;
            minthr = thr;
        }
        test << "strassen: " << "thr = " << thr << "\t" << (END - START)/CLOCKS_PER_SEC << "s" << endl;
        cout << "done loop" << i+1 << endl;
    }
    test << "minimum threshold: " << minthr;
}

/***************************************************
Function test_thr
1.test the threshold when n and threshold are same
***************************************************/
void test_c(){
    float START, END;                                   //set start and end
    vector<int> row(n);
    vector< vector<int> > matrix1(n, row);              //new n*n matrix
    random(matrix1, n);                                 //random elements
    vector< vector<int> > matrix2(n, row);
    random(matrix2, n);
    vector< vector<int> > final_matrix(n, row);
    fstream test;
    test.open(outname, ios::out);                       //create/open output file
    START = clock();                                    //record start time
    normal_multiplication(matrix1, matrix2, final_matrix, n);
    END = clock();                                      //record end time
    cout << "done1";
    test << "normal: " << (END - START)/CLOCKS_PER_SEC << "s" << endl;
    for(int i = 0 ; i < loop - 1 ; i++){
        counter = 0;
        START = clock();
        strassen_prepare(matrix1, matrix2, final_matrix, n);
        END = clock();
        test << "strassen: " << "c = " << c << "\t" << (END - START)/CLOCKS_PER_SEC << "s" << endl;
        c+=1;
    }
}

/***************************************************
Function read
1.reads input.txt
***************************************************/
void read(string name){
    fin.open(name, ios::in);                            //opens name(input.txt)
    fout.open("output.txt", ios::out);                  //opens/creates output.txt
    if(!fin){                                           //if cannot find input file
        cout << "Can't Find File!" << endl;
    }
    else{
        int n, number;
        bool has_next = true;
        while(fin >> n){
            if( n > 5000 || n < 0){
                cout << "Size Cannot be Larger than 5000 or Smaller than 0!";
                break;
            }
            vector<int> row(n);
            vector< vector<int> > matrix1(n, row);
            cout << "matrix1:" << endl;
            for(int i = 0 ; i < n ; i++){
                for(int j = 0 ; j < n ; j++){
                    fin >> number;
                    matrix1[i][j] = number;
                    cout << number << " ";
                }
                cout << endl;
            }
            vector< vector<int> > matrix2(n, row);
            cout << "matrix2:" << endl;
            for(int i = 0 ; i < n ; i++){
                for(int j = 0 ; j < n ; j++){
                    fin >> number;
                    matrix2[i][j] = number;
                    cout << number << " ";
                }
                cout << endl;
            }
            cout << endl;
            vector< vector<int> > final_matrix(n, row);
            strassen_prepare(matrix1, matrix2, final_matrix, n);
            print(final_matrix, n);
        }
        cout << "output done";
    }
}

/**********************************
Function print
1.prints the result into file
**********************************/
void print(vector< vector<int> > result, int n){
    for(int i = 0 ; i < n ; i++){
        for(int j = 0 ; j < n ; j++){
            fout << result[i][j] << " ";
        }
        fout << endl;
    }
    fout << endl;
}

/**********************************
Function random
1.creates random numbers from 1-10
2.store into matrix
**********************************/
void random(vector <vector<int> > &m, int &n){
    srand(time(NULL));
    for(int i = 0 ; i < n ; i++){
        for(int j = 0 ; j < n ; j++){
            m[i][j] = rand()%10+1;                      //random number from 1 to 10
        }
    }
}

/**********************************
Function normal_multiplication
1.normal way of multiplication
O(n^3)
**********************************/
void normal_multiplication(vector< vector<int> > &m1, vector< vector<int> > &m2, vector< vector<int> > &fin, int &n){
    for(int i = 0 ; i < n ; i++){
        for(int j = 0 ; j < n ; j++){
            for(int k = 0 ; k < n ; k++){
                fin[i][j] += m1[i][k]*m2[k][j];
            }
        }
    }
}

/**********************************
Function strassen_add
1.matrix addition
**********************************/
void strassen_add(vector< vector<int> > &A, vector< vector<int> > &B, vector< vector<int> > &C, int &n){
    for(int i = 0 ; i < n ; i++){
        for(int j = 0 ; j < n ; j++){
            C[i][j] = A[i][j] + B[i][j];
        }
    }
}

/**********************************
Function strassen_subtract
1.matrix subtraction
**********************************/
void strassen_subtract(vector< vector<int> > &A, vector< vector<int> > &B, vector< vector<int> > &C, int &n){
    for(int i = 0 ; i < n ; i++){
        for(int j = 0 ; j < n ; j++){
            C[i][j] = A[i][j] - B[i][j];
        }
    }
}

/*****************************************************
Function strassen_prepare
1.resize the input matrices into  even size matrices
2.call function to do Strassen's Algorithm
*****************************************************/
void strassen_prepare(vector< vector<int> > &A, vector< vector<int> > &B, vector< vector<int> > &C, int &n){
    int m = pow(2, int(ceil(log2(n))));             //set new size, which is the power of two
    if(n > 4096){                                   //to prevent using to much memory
        m = 2*ceil(n/2);
    }
    vector<int> new_row(m);                         //set new row of the new size
    vector< vector<int> > A_prep(m, new_row);       //set new empty matrices
    vector< vector<int> > B_prep(m, new_row);
    vector< vector<int> > C_prep(m, new_row);
    for(int i = 0 ; i < n ; i++){                   //to store original matrices
        for(int j = 0 ; j < n ; j++){
            A_prep[i][j] = A[i][j];
            B_prep[i][j] = B[i][j];
        }
    }

    strassen_algorithm(A_prep, B_prep, C_prep, m);  //do the Strassen's Algorithm
    for(int i = 0 ; i < n ; i++){                   //store the result back to original matrix
        for(int j = 0 ; j < n ; j++){
            C[i][j] = C_prep[i][j];
        }
    }
}

/*****************************************************
Function strassen_algorithm
1.if on test then there are two if(s)
2.set new N and new matrices
3.m1 = (a11 + a22)*(b11 + b22)
  m2 = (a21 + a22)*b11
  m3 = a11*(b12 - b22)
  m4 = a22*(b21 - b11)
  m5 = (a11 + a12)*b22
  m6 = (a21 - a11)*(b11 + b12)
  m7 = (a12 - a22)*(b21 + b22)
  c11 = m1 + m4 - m5 + m7
  c12 = m3 + m5
  c21 = m2 + m4
  c22 = m1 - m2 + m3 + m6
*****************************************************/
void strassen_algorithm(vector< vector<int> > &A, vector< vector<int> > &B, vector< vector<int> > &C, int &n){
    if(counter == c && test == true){               //if has divided c times, use normal multiplication
        normal_multiplication(A, B, C, n);
    }
    if(n <= thr && test == false){                  //if size is smaller or equal to threshold, use normal multiplication
      normal_multiplication(A, B, C, n);
    }
    else{
        counter++;
        int newN = ceil(n/2);                       //set new N, which is the ceiling of n/2, in case of odd number
        vector<int> row(newN);                      //set new row of new N and new empty matrices of N*N
        vector< vector<int> > a11(newN, row), a12(newN, row), a21(newN, row), a22(newN, row), afin(newN, row);
        vector< vector<int> > b11(newN, row), b12(newN, row), b21(newN, row), b22(newN, row), bfin(newN, row);
        vector< vector<int> > c11(newN, row), c12(newN, row), c21(newN, row), c22(newN, row);
        vector< vector<int> > m1(newN, row), m2(newN, row), m3(newN, row), m4(newN, row), m5(newN, row), m6(newN, row), m7(newN, row);
        for(int i = 0 ; i < newN ; i++){            //divide both the original A and B matrices into 4 small matrices
            for(int j = 0 ; j < newN ; j++){
                a11[i][j] = A[i][j];
                a12[i][j] = A[i][j + newN];
                a21[i][j] = A[i + newN][j];
                a22[i][j] = A[i + newN][j + newN];
                b11[i][j] = B[i][j];
                b12[i][j] = B[i][j + newN];
                b21[i][j] = B[i + newN][j];
                b22[i][j] = B[i + newN][j + newN];
            }
        }
        strassen_add(a11, a22, afin, newN);         //a11 + a22
        strassen_add(b11, b22, bfin, newN);         //b11 + b22
        strassen_algorithm(afin, bfin, m1, newN);   //m1 = (a11 + a22)*(b11 + b22)

        strassen_add(a21, a22, afin, newN);         //a21 + a22
        strassen_algorithm(afin, b11, m2, newN);    //m2 = (a21 + a22)*b11

        strassen_subtract(b12, b22, bfin, newN);    //b12 - b22
        strassen_algorithm(a11, bfin, m3, newN);    //m3 = a11*(b12 - b22)

        strassen_subtract(b21, b11, bfin, newN);    //b21 - b11
        strassen_algorithm(a22, bfin, m4, newN);    //m4 = a22*(b21 - b11)

        strassen_add(a11, a12, afin, newN);         //a11 + a12
        strassen_algorithm(afin, b22, m5, newN);    //m5 = (a11 + a12)*b22

        strassen_subtract(a21, a11, afin, newN);    //a21 - a11
        strassen_add(b11, b12, bfin, newN);         //b11 + b12
        strassen_algorithm(afin, bfin, m6, newN);   //m6 = (a21 - a11)*(b11 + b12)

        strassen_subtract(a12, a22, afin, newN);    //a12 - a22
        strassen_add(b21, b22, bfin, newN);         //b21 + b22
        strassen_algorithm(afin, bfin, m7, newN);   //m7 = (a12 - a22)*(b21 + b22)

        strassen_add(m3, m5, c12, newN);            //c12 = m3 + m5
        strassen_add(m2, m4, c21, newN);            //c21 = m2 + m4

        strassen_add(m1, m4, afin, newN);           //m1 + m4
        strassen_add(afin, m7, bfin, newN);         //m1 + m4 + m7
        strassen_subtract(bfin, m5, c11, newN);     //c11 = m1 + m4 - m5 + m7

        strassen_add(m1, m3, afin, newN);           //m1 + m3
        strassen_add(afin, m6, bfin, newN);         //m1 + m3 + m6
        strassen_subtract(bfin, m2, c22, newN);     //c22 = m1 - m2 + m3 + m6
        for(int i = 0; i < newN ; i++){             //combine back to last size matrix
            for(int j = 0 ; j < newN ; j++){
                C[i][j] = c11[i][j];
                C[i][j + newN] = c12[i][j];
                C[i + newN][j] = c21[i][j];
                C[i + newN][j + newN] = c22[i][j];
            }
        }
    }
}
