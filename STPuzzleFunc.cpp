#include "STPuzzleFunc.h"


SixTeenPuzzle::SixTeenPuzzle(int* state, int len ) { //set initial state and goal state
    initial = state;
    goal = new int[len];
    int i;
    for (i = 0; i<len-1; i++) {
        goal[i] = i+1;
    }
    goal[i] = 0;
}
int* SixTeenPuzzle::getInitial() {
    return initial;
}

int SixTeenPuzzle::find_blank_square(int* state, int len) {
    for (int i = 0; i<len; i++)
        if (state[i] == 0) {
            // std::cout<<"blank_pos = "<<i<<std::endl;
            return i;
        }
    std::cout<<"blank square does not exist"<<std::endl;
    return -1;
}
void SixTeenPuzzle::actions_possible(int* state, char actions[]) { //left = 0, 
    int pos = this->find_blank_square(state, 16);
    char possible_actions[] = {'L','R','U','D'};
    if (pos % 4==0)  //cannot move left
        possible_actions[0]='N';
    else if (pos % 4 ==3)// cannot mov right
        possible_actions[1] = 'N';
    if (pos>=0 && pos<=3)//cannot move up
        possible_actions[2] = 'N';
    if (pos>=12 && pos<=15)//cannot move down
        possible_actions[3] = 'N';
    for (int i = 0; i<4; i++) {
        actions[i] = possible_actions[i];
    }
}
void SixTeenPuzzle::result(int* state, int len, char action) {// action is deemed to be a valid choice
    int pos = this->find_blank_square(state, len);
    if (action=='L') {//swap the two
        state[pos] = state[pos-1];
        state[pos-1] = 0;
    }
    else if (action=='R') {
        state[pos] = state[pos+1];
        state[pos+1] = 0;
    }
    else if (action=='U') {
        state[pos] = state[pos-4];
        state[pos-4] = 0;
    }
    else {
        state[pos] = state[pos+4];
        state[pos+4] = 0;
    }
    std::cout<<"action chosen : "<<action<<std::endl;
}
bool SixTeenPuzzle::goal_test(int* state) {
    for (int i = 0; i<16; i++) {
        if (state[i] != this->goal[i]) {
            return false;
        }
    }
    return true;
}
bool SixTeenPuzzle::solvable(int* state) {
    //obtain the invertCount
    int inv_count = 0;
    for (int i = 0; i<16; i++) {
        for (int j = i+1; j<16; j++) {
            if (state[j]!=0 && state[i]!=0 && state[j]<state[i])
                inv_count+=1;
        }
    }
    //check for two cases. 
    //Case1: blank is on even row and inv_count is odd
    //Case2: blank is on odd_row and inv_count is even
    int blank_pos = find_blank_square(state, 16);
    int row_blank = blank_pos/4;
    std::cout<<"blank pos, row  inv_count = "<<blank_pos<<" "<<row_blank<<" "<<inv_count<<std::endl;
    return ( (blank_pos/4%2==0 && inv_count%2==1) || (blank_pos/4%2==1 && inv_count%2==0)); 
}

Node::Node(int* given, Node* parent) {
    for (int i = 0; i<16; i++) 
        this->state[i] = given[i];
    this->parent = parent;
    if (this->parent==NULL)
        this->depth = 0;
    else
        this->depth = parent->getDepth()+1;
}
int Node::getDepth() {
    return depth;
}
int* Node::getState() {
    return state;
}

bool Node::compare_states(int*state) {
    for (int i = 0; i<16; i++) {
        if (this->state[i]!=state[i]) {                    
            return false;
        }
    }
    return true;
}

int h(int* state){ //heuristic used NEEDS TO BE FIXZED
    int sum = 0;
    int init_x, init_y, goal_x, goal_y;
    for (int i = 0; i<16; i++){
        if (state[i]==0) {
            goal_x = 4;
            goal_y = 4;
        }
        else {
            goal_x = (state[i]-1)%4;
            goal_y = (state[i]-1)/4;
        }
        init_x = i%4;
        init_y = i/4;
        sum += abs(goal_x - init_x) + abs(goal_y - init_y);
    }
    return sum;
}

int search(SixTeenPuzzle* problem, std::vector<Node*>* path, int g, int bound, std::unordered_map <std::string, int>*pattern1, 
    std::unordered_map <std::string,int>*pattern2, 
    std::unordered_map <std::string, int>*pattern3) {
    Node* current = (path->back());
    int* test = current->getState();
    int he = P_Hub(pattern1, pattern2, pattern3, current->getState());
    std::cout<<"he  = "<<he<<std::endl;
    int f = g + he;
    if (f> bound) {
        std::cout<<"f > bound"<<std::endl;
        return f;
    }
    if (problem->goal_test(current->getState())) {
        return -1; //this is true
    }
    std::cout<<"1st state: "<<std::endl;
    display_state(current->state);
    int min = INF;
    char actions[] = {'L','R','U','D'};
    problem->actions_possible(current->getState(), actions);
    //get the sucessors !!!
    //copy the state first
    int* current_state = current->state;
    int copy_state[16];
    //create new successor nodes
    for (int i = 0; i<4; i++) {
        if (actions[i]!='N') {
            for (int i =0; i<16; i++)
                copy_state[i] = current_state[i];
            problem->result(copy_state, 16, actions[i]); //actions performed on copy state

            bool flag = false;
            for(std::vector<Node*>::iterator it = path->begin(); it != path->end(); it++) {
                if ( (*it)->compare_states(copy_state)==true) {
                    flag = true;
                    break;
                }
            }
            
            if (flag==false) { //DNE so add it into the end end of path
                Node* child = new Node(copy_state, current);
                path->push_back(child);
                int t = search(problem, path, g+1, bound, pattern1, pattern2, pattern3);
                if (t ==-1) //if it found the path
                    return -1;
                if (t<min) //set the new limit
                    min = t;
                path->pop_back(); //remove this branch, explored and not found
            }
        }
    }
    return min;
}

int ida_star(SixTeenPuzzle* problem, std::unordered_map <std::string, int>*pattern1, 
    std::unordered_map <std::string,int>*pattern2, 
    std::unordered_map <std::string, int>*pattern3){
    Node root(problem->getInitial(), NULL);
    std::vector<Node*> path;
    int limit = INF; //suppose to be a huge number
    path.push_back(&root); //put root back into stack
    int bound = P_Hub(pattern1, pattern2, pattern3, root.getState());
    while(1){
        int t = search(problem, &path, 0, bound, pattern1, pattern2, pattern3);
        if (t==-1) {//if found
            std::cout<<"ANSWER FOUND< PLEASE"<<std::endl;
            return 1;
        }
        if (t==limit){//not found if initial value does not change
            std::cout<<"ANSWER NOT FOUND "<<std::endl;
            return 0;
        }
        bound = t; // set bound to the lowest costing cutoff branch
        path.clear(); //reset the path
        path.push_back(&root);
    } 
}

 void display_state(int* state) {
            for (int i = 0; i<4; i++) {
                for (int j = 0; j<4; j++) {
                    std::cout<<state[i*4+j]<<" ";
                }
                std::cout<<std::endl;
            }
        }


int P_Hub(std::unordered_map <std::string, int>*pattern1, 
    std::unordered_map <std::string,int>*pattern2, 
    std::unordered_map <std::string, int>*pattern3, int* state) {
    //this is stored as 5-5-5, look at djkhalid file for more info
    int pos1[16];
    int pos2[16];
    int pos3[16];
    //get the pattern grouping of each
    for (int i =0; i<16; i++) {
        if (state[i]>0 && state[i]<6) {
            pos1[i] = state[i];
            pos2[i] = 0;
            pos3[i] = 0;
        }
        else if (state[i]>5 && state[i]<11) {
            pos2[i] = state[i];
            pos1[i] = 0;
            pos3[i] = 0;
        }
        else if (state[i]>10 && state[i]<16) {
            pos1[i] = 0;
            pos1[i] = 0;
            pos3[i] = state[i];
        }
        else {
            pos1[i] = 0;
            pos1[i] = 0;
            pos3[i] = 0;
        }
    }
    std::string pat1 = toString(pos1); 
    std::string pat2 = toString(pos2); 
    std::string pat3 = toString(pos3);
    int score1 = (*pattern1)[pat1]; 
    int score2 = (*pattern2)[pat2]; 
    int score3 = (*pattern3)[pat3];
    std::cout<<"phub score = "<<score1 + score2 + score3<<std::endl;
    return score1 + score2 + score3; 
}

int main() {
    std::cout<<"Will begin loading it all into memory"<<std::endl;

    std::unordered_map<std::string, int>pattern1;
    std::unordered_map<std::string, int>pattern2;
    std::unordered_map<std::string, int>pattern3;

    std::ifstream myFile1("pattern1.csv");
    std::ifstream myFile2("pattern2.csv");
    std::ifstream myFile3("pattern3.csv");
    if (!myFile1.is_open())
        std::cout<<"ERROR: FILE1 OPEN\n";
    if (!myFile2.is_open())
        std::cout<<"ERROR: FILE2 OPEN\n";
    if (!myFile3.is_open())
        std::cout<<"ERROR: FILE3 OPEN\n";

    std::string pattern;
    std::string goal;
    int x = 0;
    std::cout<<"reading File1"<<std::endl;
    while (myFile1.good()) {
        getline(myFile1, pattern, ',');
        getline(myFile1, goal, '\n');
        std::stringstream toInt(goal);
        toInt >> x;
        pattern1[pattern] = x; 
    }
    std::cout<<"reading File2"<<std::endl;
    while (myFile2.good()) {
        getline(myFile2, pattern, ',');
        getline(myFile2, goal, '\n');
        std::stringstream toInt(goal);
        toInt >> x;
        pattern2[pattern] = x; 
    }
    std::cout<<"reading File3"<<std::endl;
      while (myFile3.good()) {
        getline(myFile3, pattern, ',');
        getline(myFile3, goal, '\n');
        std::stringstream toInt(goal);
        toInt >> x;
        pattern3[pattern] = x; 
    }

    int choice;
    // while(1) {
    //     std::cout<<"press 1 to input your own 4x4 starting sequence and 2 for random generate"<<std::endl;
    //     std::cin >> choice;
    //     if (choice==1) {
    //         int b[16];
    //         for (int i = 0; i<16; i++) {
    //             std::cin>>b[i];    
    //         }
    //     }
    //     else {
        //int  b[] = {1,2,4,7,5,12,3,8,14,0,6,10,9,13,11,15};
        //int  b[] = {1,2,3,4,5,6,7,8,9,10,11,12,0,13,14,15};
        // }
        //int b[] = {3,9,1,15,14,11,4,6,13,0,10,12,2,7,8,5};
        int b[] = {1,2,3,0,5,6,7,4,9,10,11,8,13,14,15,12};
        int len = 16;
        SixTeenPuzzle Problem(b, len);
        if (Problem.solvable(b)) {
            std::cout<<"IT IS SOLVALBE"<<std::endl;
        // std::cout<<Problem.solvable(b)<<std::endl;
        int res = ida_star(&Problem, &pattern1, &pattern2, &pattern3);
        }
        else {
            std::cout<<"NOT A VALID puzzle"<<std::endl;
        }
    //}
    return 0;
}
