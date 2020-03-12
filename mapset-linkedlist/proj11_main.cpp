#include<iostream>
using std::ostream; using std::cout; using std::endl;
#include<string>
using std::string;
#include<utility>
using std::pair;
#include<initializer_list>
using std::initializer_list;
#include<sstream>
using std::ostringstream;

#include "proj11_mapset.h"

//g++ -std=c++11 -Wall -g *.cpp
    

int main()
{
  MapSet<string, long> m1({{"bill", 1}, {"will" , 1}, {"sill", 6}});
  cout << m1 << endl;
  cout << m1.size() << endl;
  
 auto n1 = m1.get("bill");
  cout << "get fred:" << n1 << endl;
 auto n2 = m1.get("irving");
  cout << "get irving:"<< n2 << endl;
  

  cout <<  "george update:" << m1.update("george", 10) << endl;
  
  cout << m1 << endl;
  
  MapSet<string, long> ms_copy(m1);
  
  cout << ms_copy << endl;
  
  cout << "bob update:" << m1.update("bill", 100) << endl;
 
  cout << m1.remove("irving") << endl;
  cout << m1 << endl;
  
  auto union_ms = m1.mapset_union(ms_copy);
  auto union_ms2 = ms_copy.mapset_union(m1);
  cout << "union: " <<union_ms << endl;
  cout << "union2: " << union_ms2 << endl; 
  
   auto intersect_ms = ms_copy.mapset_intersection(m1);
  cout << "intersection: "<< intersect_ms <<endl;
  
  
}