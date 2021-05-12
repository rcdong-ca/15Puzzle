
# 15-Puzzle and Pattern Databases




Introduction

The 15-puzzle is a sliding puzzle that consists of 4x4 tiles. The object of the puzzle is to move the empty space around until reaching the goal state. An image of the goal state is provided in figure 1.  This project was coded in C++ using modules from the  standard library such as iostream, vector, algorithm,cmath, queue, unordered_map, string, fstream,sstream, time.h. A class for the 15-puzzle was created containing the functions FifteenPuzzle, getInitial, find_blank_square, actions_possible, result, goal_test and solvable. The Node class contains the functions Node, getDepth, getState, compare_states. Additional functions to help create the solver for the 15-puzzle was search, ida_star, display_state, bfs_to_database, copy_array, toString, get_actions, get_results, P_Hub.






 
Figure 1


![image](https://user-images.githubusercontent.com/52853478/106701909-4e6cf880-659c-11eb-8131-28815274fa6a.png)






Functions similar to the ones in EightPuzzle were used. Using AI techniques such as ida_star, admissible heuristics, search trees and databases enabled the solver to work on the 15-puzzle. To efficiently solve the puzzle, a memory-efficient A* algorithm IDA was implemented. To compliment the IDA* search, a pattern database heuristic is used. The 5-5-5 partition was chosen which splits the 15-puzzle into three blocks containing 5 tiles because the time it takes to construct the database is shorter (16!/(16-5)! * 3 entries) compared to other partitions such as 6-6-3 (16!/(16-6)!*2 + 16!/(16-3)! entries) or 7-8. Figure 2 shows a visualization of the three different partitioning techniques for the pattern database.



![image](https://user-images.githubusercontent.com/52853478/106702052-925ffd80-659c-11eb-9ac6-e0b287824ed9.png)

 
Figure 2



Then, BFS was used on the partitions with the blank tile for each group or pattern. The database is in the form of a table containing both the pattern value and the respective BFS cost. Combining the IDA with the heuristic, at each cycle of this algorithm when creating nodes in the graph, the cost for the node at the puzzle state is calculated. This is done by discovering the cost of the groups using our pattern database thus giving us the heuristic value. We could then use these AI techniques to solve the 15-puzzle in a reasonable amount of time.









Reasoning for C++

As stated in the introduction, this assignment was coded in C++ and not python. The reason for this is that C++ is an efficient programming language which allows for the 15-puzzle to be optimally solved. C++ executes faster and creates more time-efficient programs. Compared to python where the programs are interpreted, C++ is compiled. Another benefit is that there will be no signal type errors on runtime due to the initialization of variable types.


Explanation of IDA* 

IDA*, a graph traversal and path search algorithm identifies the smallest path between the goal nodes and a specified start node. Similar to iterative deepening DFS, IDA* uses a heuristic function to determine the cost to reach the goal from A* search. IDA* uses the function f(n) = g(n) + h(n), where g(n) is the traversal cost to node n from the root and h(n) is a heuristic estimate of the traversal cost to the goal from n. At every iteration a DFS is executed. When the total cost of f(n) = g(n) + h(n) is larger than the initial cost estimate, a branch is cut off. Two important properties of IDA* are as followed:

1.	IDA* finds the shortest path from the initial node to any goal node when the heuristic function h is admissible: h(n) < h*(n)
2.	IDA* does not maintain the nodes in memory except the current path which means the memory is linear in the length of the solution: h(n) < cost(n,n’) + h(n’)






Pseudocode:  
![image](https://user-images.githubusercontent.com/52853478/106702107-a99eeb00-659c-11eb-9ac6-ead1251a9dcd.png)

Figure 3





Explanation of Database

The pattern database is a heuristic function which is saved in a look-up table containing the cost of the subproblems. To create the database, dj_pattern.cpp contains a function called bfs_to_database which utilizes breadth first search, an algorithm which traverses a tree/graph. BFS continuously explores the branch until backtracking to expand other nodes with time complexity O(|V| + |E|). The pseudocode of BFS is given in figure 4, and the pseudocode bfs_to_database is shown in figure 5. The patterns are split up in 3 different formats shown in figure 6. These patterns were stored as csv files as pattern1.csv, pattern2.csv and pattern3.csv respectively. These files are then read in the main function as a heuristic function for our IDA* search algorithm.



![image](https://user-images.githubusercontent.com/52853478/106702157-bb808e00-659c-11eb-9574-b8cbbb85a56a.png)
 

Figure 4


![image](https://user-images.githubusercontent.com/52853478/106702181-c804e680-659c-11eb-8e54-3767cd3129a6.png)






                        
                                                                         Figure 5
			




![image](https://user-images.githubusercontent.com/52853478/106702220-d3f0a880-659c-11eb-9786-1b83dd84a4db.png)


		
 
Figure 6






Instructions to run

Enter in the terminal the following command:

g++ --std=c++11 dj_pattern.cpp main.cpp -o main


The current main.cpp will the 15-puzzle solver with the state:

 7   5   2   4
 1   6   3   8
 9  14 10 11
13  0  15  12

The time to run IDA* star and the number of nodes will be shown in the terminal.

Results

Using a memory efficient A* algorithm IDA combined with a pattern database to make use of heuristics and search trees, the 15-puzzle was solved in an efficient manner. The 5-5-5 pattern database was chosen for this assignment. The table and chart below provide trials for 25 different 15-puzzles. Most of the states were solved in less than a second with a few outliers which skewed the average. The average then was still only 3.768 seconds for the 25 trials. The node length travelled on average was 18.16.

 
![image](https://user-images.githubusercontent.com/52853478/106702270-e965d280-659c-11eb-85e1-94684569fc35.png)


