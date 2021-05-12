#include "STPuzzleFunc.h"
/* 1 2 3 4 
   5 6 7 8
   9 10 11 12
   13 14 15 

A 5-5-5 partition whill be chosen 
1st: 1 2 3 4 5 
2nd: 6 7 8 9 10
3rd: 11 12 13 14 15
Al the rest will be zero, including the empty
*/
/*First let us obtain all the states wwith different blank space position
and not chaning any of the 1st pattern final position. This can be done by breadth first search
*/

void get_results(int* state, int pos, char action) {
   if (action=='L') {
      state[pos-1] = state[pos];
      state[pos] = 0;
   }
   else if (action =='R') {
      state[pos+1] = state[pos];
      state[pos] = 0;
   }
   else if (action=='U') {
      state[pos-4] = state[pos];
      state[pos] = 0;
   }
   else {
      state[pos+4] = state[pos];
      state[pos] = 0;
   }
}
void get_actions(int* state, int pos, char* actions) {
    char possible_actions[] = {'L','R','U','D'};
    if (pos % 4==0 || state[pos-1]!=0)  //cannot move left
        possible_actions[0]='N';
    if (pos % 4==3 || state[pos+1]!=0)// cannot mov right
        possible_actions[1] = 'N';
    if ( (pos>=0 && pos<=3) || state[pos-4]!=0)//cannot move up
        possible_actions[2] = 'N';
    if ( (pos>=12 && pos<=15) || state[pos+4]!=0)//cannot move down
        possible_actions[3] = 'N';
    for (int i = 0; i<4; i++) {
        actions[i] = possible_actions[i];
    }
}

std::string toString(int* digits) {
   std::string result= ""; 
   bool flag = true;
   for (int i = 0; i < 16; i++) {
      if (flag==true){
         result += std::to_string(digits[i]);
         flag = false;
      }
      else {
      result+=".";
      result += std::to_string(digits[i]);
      }
   }
   return result;
}

void copy_array(int*arr1, int* arr2) {
   for (int i = 0; i<16; i++) {
      arr2[i] = arr1[i];
   }
}

void bfs_to_database(int* state, std::ofstream* myfile) { //performed on the blank tile
   int* problem = new int[16];
   copy_array(state, problem);
   std::unordered_map<std::string, int>explored;
   std::queue<Node*>myqueue;

   int count = 0;

      /*get all the permutations*/
   std::cout<<"Start filling the map with possible permutations\n";
   std::sort(problem, problem+16);
   do {
      std::string p = toString(problem);
      explored[p] = -1;
      count+=1;
      if (count%10000==0)
         std::cout<<count<<std::endl;
   }while (std::next_permutation(problem,problem+16));
   std::cout<<"Finished u_map permutations\n";
   count = 0;
   /*                                       */
   int* root_state =  new int[16];
   copy_array(state, root_state);
   Node* root = new Node(root_state, NULL);
   myqueue.push(root);
   Node* currentNode = NULL; 
   int i, j;
   while(!myqueue.empty()) { //not null
      currentNode = myqueue.front();
      std::string current_s = toString(currentNode->state);
      if (explored[current_s]==-1) { //not found explored yet
         count+=1;
         std::cout<<count<<std::endl;
         *myfile<<current_s<<","<<currentNode->depth<<std::endl;
         explored[current_s] = currentNode->depth;

         //find all possible states from the current state, and add it into myqueue
         for (i =0 ; i<16; i++) {
            if (currentNode->state[i]!=0) {
               char actions[4] = {'L','R','U', 'D'};
               get_actions(currentNode->state, i, actions); //obtain actions for state[i]
               for (j=0; j<4; j++) { //add children in
                  if (actions[j]!='N') {
                     int* new_state = new int[16]; //copy state
                     copy_array(currentNode->state, new_state);
                     get_results(new_state, i, actions[j]);//get a resulting state               
                     std::string new_s = toString(new_state);//convert it into string
                     if (explored[new_s]==-1) {
                        Node* new_node = new Node(new_state, currentNode);
                        myqueue.push(new_node); //add into queue
                        new_node->parent = NULL;
                     }
                     else {
                        delete [] new_state;
                     }  
                                       
                  }
               }
            } 
         }
      }
      myqueue.pop(); //pop out the current one, since we added it exists in
   }
}

