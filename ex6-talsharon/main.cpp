//
// Created by talsh on 15/06/2022.
//

#include <vector>
#include <iostream>
#include "HashMap.hpp"


int main ()
{
  std::vector<int> keys = {0, 2, 3, 2, 18};
  std::vector<int> values = {0, 2, 3, 4, 8};
  const HashMap<int, int> hm(keys, values);
  HashMap<int, int> hm2(hm);
  hm2.insert (3, -3);
  hm2[7];
  hm2[11] = 11;
  for (const std::pair<int,int>& elem : hm2)
  {
    std::cout << "key: " << elem.first << ", value: " << elem.second <<
              std::endl;
  }
}