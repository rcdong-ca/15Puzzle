#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <queue>
#include <unordered_map>
#include <string>
#include <fstream>
#include <sstream>

#define INF 100000
/*  1 2 3 4
    5 6 7 8 
    9 10 11 12
    13 14 15 0 

actions :"Left","Right","Up","Down"
int actions array based off position above, 1 is true, 0 is false
*/
class SixTeenPuzzle {
    int* initial;
    int* goal;
    int p;
    public:
        SixTeenPuzzle(int* state, int len );
        int* getInitial();
        int find_blank_square(int* state, int len);
        void actions_possible(int* state, char actions[]);
        void result(int* state, int len, char action);
        bool goal_test(int* state);
        bool solvable(int* state);
        
};
class Node {
    public:
    int state[16];
    Node* parent;
    int depth; //==path cost
        Node(int* state, Node* parent);
        int getDepth() ;
        int* getState();
        bool compare_states(int*state);
};

int h(int* state);

int search(SixTeenPuzzle* problem, std::vector<Node>* path, int g, int bound, std::unordered_map <std::string, int>*pattern1, 
    std::unordered_map <std::string,int>*pattern2, 
    std::unordered_map <std::string, int>*pattern3);
int ida_star(SixTeenPuzzle* problem, std::unordered_map <std::string, int>*pattern1, 
    std::unordered_map <std::string,int>*pattern2, 
    std::unordered_map <std::string, int>*pattern3);
void display_state(int* state);
void bfs_to_database(int* state, std::ofstream* myfile);
void copy_array(int*arr1, int* arr2);
std::string toString(int* digits);
void get_actions(int* state, int pos, char* actions);
void get_results(int* state, int pos, char action);
int P_Hub(std::unordered_map <std::string, int>*pattern1, 
    std::unordered_map <std::string,int>*pattern2, 
    std::unordered_map <std::string, int>*pattern3, int* state);