#include <yal.h>

int main(int argc, char *argv[]) {

  // === ローパスフィルタ ===================
  print("ローパスフィルタ");
  Yal::Vector<3> des(2.0, 5.0, 4.0);
  Yal::Vector<3> orig(2.0, 0.0, 4.0);
  Yal::Vector<3> ret;

  Yal::LPF<Yal::Vector<3>> lpf(orig);
  lpf.setCutoff(100.0);

  for(int i=0; i<33; i++) {
    ret = lpf.update(des);
    ret.print();
  }

  printf("======================\n");

  // === limitRange =======================
  print("limitRange");
  double volt = 15.0;
  Yal::limitRange(volt, -9.9, 9.9);
  print(volt);
  volt = 5.0;
  Yal::limitRange(volt, -9.9, 9.9);
  print(volt);

  printf("======================\n");

  // === 正負判定 =============================
  print("正負判定");
  print(Yal::sgn(4.0));
  print(Yal::sgn(0));
  print(Yal::sgn(-9.0));

  printf("======================\n");

  // === 閾値フィルタ =============================
  print("閾値フィルタ");
  double x = 0.3;
  x = Yal::threshold(x, 1.0, 3);
  print(x);

  printf("======================\n");  
  
  // === 2値比較 =============================
  print("2値比較");
  print(Yal::get_max(3.0, 7.0));
  print(Yal::get_min(3.0, 7.0));
  
  printf("======================\n");  

  // === Range =============================
  print("Range");
  Yal::Range<double> range(-4.0, 8.0);
  print(range.center());      // (-4.0+8.0)/2 => 2.0
  print(range.check(13.0));   // 範囲外 => 1
  print(range.check(3.0));    // 範囲内 => 0
  print(range.check(-8.0));   // 範囲外 => -1
  double y = 10.0;
  range.limit(y);
  print(y);                   // y: 10.0 => 8.0
  print(range.over(18.0));    // 18.0-8.0 => 10.0
  range.expand(2.0);          // -4~8 => -10~14
  range.shift(3.0);           // -10~14 => -7~17
  print(range.min());         // -7
  print(range.max());         // 17
  print(range.width());       // 17-(-7) => 24
  print(range.normalize(5.0));  // {5-(-7)}/24 => 0.5 


  return 0;
}
