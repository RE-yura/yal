#include <yal.h>

int main(int argc, char *argv[]) {
  // ベクトルの定義
  Yal::Vector<3> vec = {10.0, 10.0, 30.0};
  print("vec = ");
  print(vec);
  print();

  // 行列の定義
  Yal::Matrix<3,4> mat = { 100.0,    0.0, -100.0,    0.0, 
                             0.0,  100.0,    0.0, -100.0, 
                           100.0,  100.0,  100.0,  100.0 };
  print("mat = ");
  print(mat);

  // 逆行列の計算
  Yal::Matrix<4,3> mat_inv = mat.inv();   
  printf("mat_inv = \n");
  mat_inv.print();

  // 列数の取得
  printf("rowSize = %ld\n", sizeof(mat.row(0))/sizeof(mat.row(0)[0]));
  printf("rowSize = %d\n", mat.rows());

  // 行数の取得
  printf("columnSize = %ld\n", sizeof(mat.column(0))/sizeof(mat.column(0)[0]));
  printf("columnSize = %d\n", mat.columns());
  print();

  // 単位行列の定義
  Yal::Matrix<3,3> E;
  E.unitize();
  printf("E = \n");
  E.print();

  
  // 行列とベクトルの掛け算
  Yal::Vector<4> I = mat_inv * vec;
  printf("I = ");
  print(I);
  print();

  // 1行1列行列･1要素ベクトルの掛け算
  Yal::Matrix<1,1> Q = {4.0};
  Yal::Vector<1> vec2 = {3.0};
  (Q*vec).print();
  (Q*vec2).print();
  (Q.inv() * vec2).print();
  (Q * 4).print();
  (2 * Q * pow(2,2)).print();
  
  return 0;
}

