#include <yal.h>
// #include "../../src/yal.h"
// #include "../../src/print.h"

int main(int argc, char *argv[]) {
  int a = 1;
  print("Test");
  print(a, 3.14, "Hello");
  print();

  vector<int> arr{1, 2, 3, 2};
  vector<string> str_arr{"Hello", "World"};
  print(arr);
  print(str_arr);
  print();

  vector<vector<int>> table{{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};
  print(table);
  print();

  Yal::Vector<4> vec(1.0, 2.0, 3.0, 2.0);
  print(vec);
  print();

  Yal::Matrix<3,3> mat(1.0, 2.0, 3.0, 
                       4.0, 5.0, 6.0, 
                       7.0, 8.0, 9.0);
  print(mat);
}
