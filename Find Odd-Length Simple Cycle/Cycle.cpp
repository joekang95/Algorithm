#include<iostream>
#include<fstream>
#include<stdio.h>
#include<stdlib.h>
#include<sstream>
#include<time.h>
#include<vector>
#include<algorithm>
#include<map>
using namespace std;

enum Color {WHITE, GRAY, BLACK};                //Set Colors White for Unprocessed, Gray for Processing, Black for Done
struct Path{                                    //Structure Path for recording paths (used for checking)
    int start;
    int destination;
};                                              //Function Prototypes
bool checkCycle(int &u, vector<int> &color, map<int, vector<int> > &graph, vector<int> &cycle);
bool isCycle(map<int, vector<int> > &graph, vector<int> &cycle, int &n);
map<int, vector<int> > Graph(vector<int> &s, vector<int> &d, int &n);
int counter;                                    //Counter for counting path length
int cyclePoint;                                 //Cycle Point same as Starting Point
void read(string &name);                        //Function Prototype
fstream fin;                                    //For read input file
fstream fout;                                   //For write output file

int main(){
    string infile = "input.txt";                //Input file name
    read(infile);                               //call function read
    cout << endl << endl;
    system("pause");
}

void read(string &name){
    fin.open(name, ios::in);
    fout.open("output.txt", ios::out);
    if(!fin){                                   //If cannot find input file
        cout << "Can't Find File!" << endl;
    }
    else{
        string line;                            //Used to read lines
        int num1 = 0;                           //Read First Number
        int num2 = 0;                           //Read Second Number
        int lineNumber = 0;                     //Read lineNumber, which is number of edges
        while(fin >> num1 >> num2){             //While there are two numbers to read in
            lineNumber = num2;                  //The Second Number of first line is number of edges
            vector<int> s(lineNumber);          //Set vector s to store starting points
            vector<int> d(lineNumber);          //Set vector d to store destination
            for(int i = 0 ; i < lineNumber ; i++){  //Read in numbers
                fin >> num1 >> num2;
                s[i] = num1;
                d[i] = num2;
            }
            vector<Path> paths(lineNumber);         //Store Edges
            for(int i = 0 ; i < lineNumber ; i++){  //Used for printing inputs
                paths[i] = {s[i], d[i]};
                //cout << paths[i].start << " " << paths[i].destination << endl;
            }
            int maxStart = *max_element(s.begin(), s.end());        //Find max Start vertex
            int maxDest = *max_element(d.begin(), d.end());         //Find max Destination vertex
            int maxNode = max(maxStart, maxDest);                   //Find max vertex
            map<int, vector<int> > graph = Graph(s,d,lineNumber);   //Make into graph which is adjacency list
            map<int, vector<int> >::iterator it;                    //Map iterator
            /*for(it = graph.begin() ; it != graph.end() ; it++){   //For Printing out adjacency list
                int i = it -> first;
                vector<int> edge = graph[i];
                cout << i << " ";
                for(int j = 0 ; j < edge.size() ; j++){
                    cout << edge[j] << " ";
                }
                cout << endl;
            }*/
            vector<int> cycle(maxNode + 1);                         //For Storing cycle of the result
            clock_t START, END;
            START = clock();
            bool oddCycle = isCycle(graph, cycle, maxNode);         //Start Finding if there is a odd cycle
            END = clock();
            if(oddCycle){                                           //If there is odd cycle
                cout << "Yes" << endl;
                fout << "Yes" << endl;
                cout << "Cycle: ";
                for(int i = 0 ; i < cycle.size() ; i++){
                    if(cycle[i] != 0){
                        cout << cycle[i] << " ";
                        fout << cycle[i] << " ";
                    }
                }
                cout << endl;
                fout << endl;
            }
            else{
                cout << "No" << endl;
                fout << "No" << endl;
            }
            lineNumber = 0;                                         //Initialize lineNumber
            cout << "Total Time: " << (float)(END - START)/CLOCKS_PER_SEC << endl;
            cout << endl;
            fout << endl;
        }
    }
}
/*********************************
Function Graph
Input:
    1. vector of starting points
    2. vector of destinations
    3. number of edges
Output:
    1. map of vectors
    (Adjacency List)
*********************************/
map<int, vector<int> > Graph(vector<int> &s, vector<int> &d, int &n){
    map<int, vector<int> > graph;       //New Map, Key: Vertex, Values: Neighbors
    for(int i = 0 ; i < n ; i++){
        graph[s[i]].push_back(d[i]);
        graph[d[i]];
    }
    return graph;
 }
/*********************************
Function isCycle
Input:
    1. Adjacency List
    2. Cycle to store result
    3. Max Node
Output:
    1. Boolean
*********************************/
bool isCycle(map<int, vector<int> > &graph, vector<int> &cycle, int &n){
    bool result = false;                                    //Initialize result as false
    counter = 0;                                            //Initialize counter (path length)
    vector<int> color(n+1, WHITE);                          //Initialize vertex as white
    map<int, vector<int> >::iterator it;                    //Map Iterator
    for(it = graph.begin() ; it != graph.end() ; it++){     //For all vertex
        cyclePoint = it -> first;                           //Set cyclePoint
        for(int i = 0 ; i < n+1 ; i++){                     //Initialize vertex as white every loop
            color[i] = WHITE;
        }
        if(color[cyclePoint] == WHITE){                     //If unprocessed
            if(checkCycle(cyclePoint, color, graph, cycle)){//If has odd cycle
                result = true;                              //Set result as true
                break;
            }
        }
    }
    return result;
}

bool checkCycle(int &u, vector<int> &color, map<int, vector<int> > &graph, vector<int> &cycle){
    color[u] = GRAY;                                        //Set vertex as Gray (Processing)
    counter++;                                              //Increase counter (length = vertex passed)
    vector<int>::iterator v;                                //Vector Iterator
    for(v = graph[u].begin() ; v != graph[u].end() ; v++){  //For all neighbors
        if(color[*v] == GRAY){                              //If next vertex(neighbor) going to is Gray
            if(counter % 2 != 0 && *v == cyclePoint){       //If counter is odd and is the starting point
                cycle.push_back(u);                         //Store current vertex to cycle
                return true;
            }
            else if(counter % 2 == 0){                      //If counter is even
                color[u] = WHITE;                           //Set current vertex back to White
                counter--;                                  //Decrease counter since go back one time
                return false;
            }
        }

        if(color[*v] == WHITE && checkCycle(*v, color, graph, cycle)){  //If next vertex(neighbor) going to is White , then check is has cycle after include next vertex
            cycle.insert(cycle.begin(), u);                             //If yes, store current vertex to cycle
            return true;
        }
    }
    color[u] = BLACK;                                       //If all neighbors are searched and does not have cycle, set current vertex to Black
    counter--;                                              //Decrease counter
    return false;
}
