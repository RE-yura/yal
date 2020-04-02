#include <bits/stdc++.h>
#include <string>
#include "la.h"

using namespace std;

// 引数なしなら空行を出力
inline void print() {
  cout << endl;
}


// Yal::Vector.Marixの出力
template <int T>
inline void print(Yal::Vector<T> &vec) {
  vec.print();
}
template <int T, int U>
inline void print(Yal::Matrix<T,U> &mat) {
  mat.print();
}

// 複数入力時の出力
template <class Head, class... Tail>
inline void print(Head&& head, Tail&&... tail) {
  cout << fixed;
  cout << setprecision(3);
  cout << head;
  if (sizeof...(tail) != 0) cout << " ";
  print(forward<Tail>(tail)...);
}

// std::vectorの出力
template <class T>
inline void print(vector<T> &vec) {
  for (auto& a : vec) {
    cout << a;
    if (&a != &vec.back()) cout << " ";
  }
  cout << endl;
}
template <class T>
inline void print(vector<vector<T>> &df) {
  for (auto& vec : df) {
    print(vec);
  }
}
