// next_permutation example
#include <iostream>     // std::cout
#include <algorithm>    // std::next_permutation, std::sort

int main () {
  int myints[] = {1,3,0,0,0};

  std::sort (myints,myints+5);
  int count = 0;
  std::cout << "The 5! possible permutations with 3 elements:\n";
  do {
    std::cout << myints[0] << ' ' << myints[1] << ' ' << myints[2]<<' ' << myints[3]<<' ' << myints[4]<<'\n';
    count+=1;
  } while ( std::next_permutation(myints,myints+5) );

  std::cout << myints[0] << ' ' << myints[1] << ' ' << myints[2] <<' '  << myints[3]<<' ' << myints[4] << '\n';
  std::cout<<"count = "<<count<<std::endl;
  return 0;
}