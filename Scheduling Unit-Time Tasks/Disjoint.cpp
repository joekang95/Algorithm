#include<iostream>
#include<algorithm>
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<fstream>
#include<vector>

using namespace std;

struct Task{                                    //Task to store a set of number, deadline, penalty
    int number;
    int deadline;
    int penalty;
};
void Mapping(vector<int> , vector<int>, int);   //Function Prototype
void read(string &name, int &n);                //Function Prototype
void Sort(Task [], int );                       //Function Prototype
bool compare(Task, Task);                       //Function Prototype
void Disjoint(Task [], int );                   //Function Prototype
int n;                                          //Store input N
int totalPenalty = 0;                           //Store total penalty
Task tasks[10000];                              //Store tasks maximum 10000 tasks
fstream fin;                                    //For read input file
fstream fout;                                   //For write output file

int main(){
    string infile = "input.txt";                //Input file name
    read(infile, n);                            //call function read
    cout << endl << endl;
    system("pause");
}
/*******************************
Function read
1. Read input.txt
2. Read and Store:
    1) n
    2) deadline
    3) penalty
3. Transform to Task
4. Disjoint Implementation
5. Write output.txt
*******************************/
void read(string &name, int &n){
    fin.open(name, ios::in);
    fout.open("output.txt", ios::out);
    if(!fin){                                           //If cannot find input file
        cout << "Can't Find File!" << endl;
    }
    else{
        int number;                                     //Read input numbers
        int temp;                                       //Read n
        while(fin >> temp){                             //While there is something to read
            n = temp;                                   //Store n
            vector<int> deadline(n);                    //Vector deadline to store deadlines
            vector<int> penalty(n);                     //Vector penalty to store penalties
            for(int i = 0 ; i < n ; i++){               //Read and Store deadline
                fin >> number;
                deadline[i] = number;
            }
            for(int i = 0 ; i < n ; i++){               //Read and Store penalty
                fin >> number;
                penalty[i] = number;
            }
            Mapping(deadline, penalty, n);              //Call Function Mapping to Transform to Task and Store to tasks
            Sort(tasks, n);                             //Call Function Sort to Sort Tasks by penalty
            clock_t START, END;
            START = clock();
            Disjoint(tasks, n);                         //Call Function Disjoint to start scheduling tasks
            END = clock();
            cout << "Number of Tasks: " << n << endl;
            cout << "After Disjoint..." << endl << "Task Order: ";
            for(int i = 0 ; i < n ; i++){               //Print out numbers of task
                if(tasks[i].number != 0){
                    cout << tasks[i].number << " ";
                    fout << tasks[i].number << " ";
                }
            }
            cout << endl << "Time: ";
            fout << endl;
            for(int i = 0 ; i < n ; i++){               //Print out time of task
                if(tasks[i].deadline != 0){
                    cout << tasks[i].deadline << " ";
                    fout << tasks[i].deadline << " ";
                }
            }
            cout << "\nTotal Penalty: " << totalPenalty;//Print out total penalty
            cout << "\nTotal Time: " << (float)(END - START)/CLOCKS_PER_SEC;
            cout << endl << "---------------------------------------------------" << endl;
            fout << endl << endl;
        }
    }
}
/*********************************
Function Mapping
1.Input:
    1) d = deadline
    2) p = penalty
    2) n = n
2.Transform to Task
3.Store to tasks
*********************************/
void Mapping(vector<int> d, vector<int> p, int n){
    for(int i = 0 ; i < n ; i++){
        tasks[i] = {i+1, d[i], p[i]};                   //Store to tasks by sets
    }
}
/*********************************
Function Sort
1.Input:
    1) tasks
    2) n
2.Use sort from <algorithm>
3.Compare by compare
4.Sort by penalty
*********************************/
void Sort(Task tasks[], int n){
    sort(tasks, tasks+n, compare);                      //Sort tasks
}
/********************************************
Function Sort
1.Input:
    1) Task beginning
    2) Task end
2.Return:
    1) True if first penalty > second penalty
********************************************/
bool compare(Task t1, Task t2){
    return (t1.penalty > t2.penalty);                   //Compare by penalty
}
/**************************************************************************
Function Disjoint
1.Input:
    1) tasks
    2) n
2.Obtain maxDeadline
3.Retrieve a task
4.Store:
    1) to result at position of task deadline if no task stored at position
    2) to position front of task deadline where no task has been stored
5.Reassign to tasks and clear rest
6.Calculate total penalty
***************************************************************************/
void Disjoint(Task tasks[], int n){
    vector<int> deadlines(n);                           //Vector deadlines to store deadline of tasks
    Task original[n];                                   //Vector original to store tasks
    for(int i = 0 ; i < n ; i++){                       //Store tasks to original and deadline of tasks to deadlines
        original[i] = tasks[i];
        deadlines[i] = tasks[i].deadline;
    }
    int maxDeadline = *max_element(deadlines.begin(), deadlines.end()); //Store maximum deadline in deadlines
    vector<Task> result(maxDeadline+1);                 //Vector result to store result
    vector<bool> select(n);                             //Vector select to record selected or not
    int start = 0;                                      //Record current number of task
    while(start < n){
        int currentDeadline = tasks[start].deadline;    //Obtain deadline of current task
        if(result[currentDeadline].deadline == 0){      //If position of currentDeadline in result is empty
            result[currentDeadline] = tasks[start];     //Store task to position in result
            select[start] = 1;                          //Task is selected
        }
        else{
            do{
                currentDeadline--;
            }while(result[currentDeadline].deadline != 0);  //Decrease currentDeadline until found a position that has no task stored
            if(currentDeadline > 0){                    //If currentDeadline is before original deadline and not negative or zero
                result[currentDeadline] = tasks[start]; //Store task to the position of currentDeadline in result
                select[start] = 1;                      //Task is selected
            }
        }
        start++;                                        //Move to next task
    }
    totalPenalty = 0;                                   //Initialize totalPenalty
    int ptr = 0;                                        //Pointer for tasks
    for(int i = 0 ; i < maxDeadline+1 ; i++){           //Run through result
        if(result[i].deadline != 0){                    //If there is a task
            tasks[ptr] = result[i];                     //Store to tasks of the position
            tasks[ptr].deadline = i;                    //Change deadline to time of doing task
            ptr++;                                      //Move to next position of tasks
        }
    }
    for(int i = ptr ; i < n ; i++){                     //Clear the rest of tasks (those which have not been overwritten)
        tasks[i] = {0,0,0};
    }
    for(int i = 0 ; i < n ; i++){                       //Count totalPenalty
        if(!select[i]){                                 //If task is not selected
            totalPenalty += original[i].penalty;        //Add the penalty of the task
        }
    }
}
