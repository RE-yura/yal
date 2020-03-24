#ifndef __YAL_LA
#define __YAL_LA

#include<stdio.h>
#include<stdarg.h>
#include<stdarg.h>
#include<math.h>
#include<utility> // for std:swap

//#define DEBUG

#ifndef DEBUG_PRINT
#ifdef DEBUG
#define DEBUG_PRINT(...) fprintf(stderr,__VA_ARGS__)
#else
#define DEBUG_PRINT(...) 
#endif
#endif

//#define DEBUG_PRINT(...) fprintf(stderr,__VA_ARGS__)
#define KTLV2(V)  ((V)[0]), ((V)[1])
#define KTLV3(V)  ((V)[0]), ((V)[1]), ((V)[2])

namespace Yal{

#define V3(V)  ((V)[0]), ((V)[1]), ((V)[2])

enum Direction{
  X, Y, Z
};
template<int h,int w>
class Matrix;

/** ベクトル.
 * テンプレートを用いて任意の次元を指定可能
 * size はベクトルの次元，長さ，サイズ
 */
template<int size> class Vector{
  double v[size];
 public:  
  /** 全ての要素を0で初期化 */
  Vector(){
    for(int i=0; i<size;i++)  v[i] = 0.0;
    //DEBUG_PRINT("default constructor\n");
  }
  Vector(const Vector& V){
    //DEBUG_PRINT("copy constructor\n");
    for(int i=0; i<size;i++)  v[i] = V[i];
  }


  double& operator[](unsigned int i){ return v[i];}
  const double operator[](unsigned int i)const { return v[i];}
  /** 代入 */
  Vector& operator=(const Vector& V){
    for(int i=0; i<size;i++)   v[i] = V[i];
    return *this;
  }
  /** 負 */
  Vector operator-()const{
    Vector Vret = *this;
    for(int i=0; i<size;i++)  Vret[i] *= -1;
    return Vret;
  }
  /** 単項加算 */
  Vector& operator+=(const Vector& V){
    for(int i=0; i<size; i++)   v[i] += V[i];
    return *this;
  }
  /** 単項減算 */
  Vector& operator-=(const Vector& V){
    for(int i=0; i<size; i++)   v[i] -= V[i];
    return *this;
  }
  /** 単項スカラー乗算 */
  Vector& operator*=(double c){
    for(int i=0; i<size; i++)  v[i] *= c;
    return *this;
  }
  /** 単項スカラー除算 */
  Vector& operator/=(double c){
    double _c = 1.0/c;
    //*this *= _c;
    for(int i=0; i<size; i++)  v[i] *= _c;
    //for(int i=0; i<size; i++)  v[i] /= c;
    return *this;
  }

  Vector operator+(const Vector& V) const {
    Vector Vret = *this;
    Vret += V;
    return Vret;
  }
  Vector operator-(const Vector& V) const{
    Vector Vret = *this;
    Vret -= V;
    return Vret;
  }
  Vector operator*(double c) const{
    Vector Vret = *this;
    Vret *= c;
    return Vret;
  }
  Vector operator/(double c) const{
    Vector Vret = *this;
    Vret /= c;
    return Vret;
  }

  /** 内積.
   * @attention ( )で囲むこと ex. (a&b)
   */
  double operator&(const Vector& V) const{
    double value=0;
    for(int i=0;i<size;i++)
      value += v[i] * V[i];
    return value;
  }

  /** 外積.
   * @attention 3次元のみ
   */
  Vector operator*(const Vector& V) const{ 
    if( size != 3 ) DEBUG_PRINT("la : outer product size != 3\n");
    return Vector( v[1] * V[2] - v[2] * V[1],
		   v[2] * V[0] - v[0] * V[2],
		   v[0] * V[1] - v[1] * V[0] );
  }

  template<int c>
  Matrix<size,c> operator*(const Vector<c>& V) const{ //
    Matrix<size,c> Mret;
    for( int i=0; i<size; i++ )
      for( int j=0; j<c; j++ )
	Mret[i][j] = v[i] * V[j];
    return Mret;
  }
  
  /** スカラーとの積 */
  friend Vector<size> operator*(double c,const Vector<size>& V){
    return V*c;
  }

  /** 射影 
   * @param n 射影先のベクトル
   * @return 射影されたベクトル
   */
  Vector operator|(const Vector<size>& n)const{
    return (*this&n) * n;  // 射影演算子 user 
  }    
  /** 要素を全て0にする. */
  void zero(){
    for (int i = 0 ; i < size ; i++)  v[i] = 0.0;
  }

  /** 要素の一括代入 **/
  void set(double v0 , ...){
    va_list args;
    va_start(args , v0);
    v[0] = v0;
    for (int i = 1 ; i < size ; i++)
      v[i] = (double) va_arg(args , double);
    va_end(args);
  }

  /** 要素指定コンストラクタ */
  Vector(double v0 , ...){
    va_list args;
    va_start(args , v0);
    v[0] = v0;
    for (int i = 1 ; i < size ; i++)
      v[i] = (double) va_arg(args , double);
    va_end(args);
  }

  /// 絶対値の2乗 平方根を計算しない
  double abs2()const {
    double value=0;
    for(int i=0;i<size;i++)  value += v[i] * v[i];
    return value;
  }
  /// 絶対値を計算する
  double abs()const {
    double value=0;
    for(int i=0;i<size;i++)  value += v[i] * v[i];
    return sqrt(value);
  }

  /// スカラー倍する
  void extend(double c){ 
    *this *= c;
  }

  /// 規格化する
  void normalize(){
    extend( 1.0/abs() );
  }
 
  /// ベクトルを回転する. 3次元のみ
  /// @param axis 回転軸
  /// @param theta 回転角度 [rad]
  void rotate(Direction axis,double theta){
     (*this) = Matrix<3,3>(axis,theta) * (*this);
  }

  /// ベクトルを回転する. 3次元のみ
  /// @param axis 回転軸
  /// @param theta 回転角度 [rad]
  void rotate(const Vector& axis,double theta){
    (*this) = Matrix<3,3>(axis,theta) * (*this);
  }
  
 
  int solve(Matrix<size,size+1>& A){ //拡大行列版
    int s[size];
    int ret = A.decompose(s);
  
    for (int i = size-1; i >= 0; i--) {
      A[s[i]][size] /= A[s[i]][i];
      for (int j = i - 1; j >= 0; j--)
	A[s[j]][size] -=  A[s[i]][size] * A[s[j]][i];
    }
    for(int i=0;i<size;i++)
      v[i] = A[s[i]][size];
    return ret;
  }
  
  /** ガウスの消去法により連立1次方程式を解く
   *  
   * 方程式 A x = b の
   * 解ベクトル x がオブジェクトに代入される
   * @param A 係数行列 (正方行列)
   * @param b 右辺のベクトル
   */
  int solve(Matrix<size,size> A, Vector b){
    int s[size];
    int ret = A.decompose(s);
    
    for( int i = 0; i < size; i++ ){ // bの前進消去
      for( int j = i+1; j < size; j++ )
	b[s[j]] -= b[s[i]] * A[s[j]][i];
    }
    for( int i = size-1; i >= 0; i-- ){ // 後退代入
      for( int j = i+1; j<size; j++ )
	b[s[i]] -= A[s[i]][j] * b[s[j]];
      v[i] = b[s[i]] /= A[s[i]][i];
    }
    return ret;
  }
  
  ///部分ベクトルの挿入
  template <int ns> 
    void insert(int n,Vector<ns> V){
    if( ns + n > size ){ fprintf(stderr,"insert size error\n"); return;}
    for(int i=0;i<ns;i++)
      v[i+n] = V[i];
  }
  /*
  void print() const {
    puts("[ ");
    //printf("[ ");
    for(int i=0; i<size;i++)
      printf("%10.3f ",v[i]);
    putchar(']');
    for(int i=0;i<n;i++) putchar('\n');
  }
  */

  /** 標準出力への値表示 */
  void print(int n=1) const {
    printf("[ ");
    for(int i=0; i<size;i++)
      printf("%10.3f ",v[i]);
    putchar(']');
    for(int i=0;i<n;i++)
      putchar('\n');
  }
  ///標準出力への値表示
  void print(const char* pre) const {
    printf("%s [ ",pre);
    for(int i=0; i<size;i++)
      printf("%10.3f ",v[i]);
    //puts(']');
    puts("]");
  }
  ///標準出力への値表示（前後への修飾付き）
  void print(const char* pre, const char* post) const {
    printf("%s [ ",pre);
    for(int i=0; i<size;i++)
      printf("%10.3f ",v[i]);
    putchar(']');
    puts(post);
  }

};

/** Matrix.
 *  h 行 w 列の行列 \n
 *  横ベクトルを縦に並べたもの
 *  @see Vector
 */
template<int h,int w> class Matrix{
  Vector<w> v[h]; // 横ベクトルを縦に並べたもの

 public:
  /** 単位行列として初期化  */
  /** 零行列として初期化すべき  */
  Matrix(){ // 単位行列
    for (int i = 0; i < h ; i++) 
      for (int j = 0; j < w ; j++)
	//v[i][j] = (i==j) ? 1.0 : 0.0;
	v[i][j] =  0.0;
  }

  /** 全ての要素の値を指定して生成
   * @param v00 最初の要素値
   */
  Matrix(double v00 , ...) {
    va_list args;
    va_start(args ,v00);
    v[0][0] = v00;
    for (int i = 0 ; i < h ; i++) {
      for (int j = 0 ; j < w ; j++){
	if( i == 0 && j == 0 ) continue;
	v[i][j] = (double) va_arg(args , double);
      }
    }
    va_end(args);
  }

  /** 回転行列として生成する(したがって3次元のみ).
   * @param axis 回転軸
   * @param theta 回転角度 [rad]
   */
  Matrix( Direction axis, double theta){
    setRot(axis,theta);
  }

  /** 回転行列として生成する(したがって3次元のみ).
   * @param axis 回転軸
   * @param theta 回転角度 [rad]
   */
  Matrix( Vector<3> axis, double theta){
    setRot(axis,theta);
  }
  
  Vector<w>& operator[](unsigned int i){ return v[i]; }
  const Vector<w> operator[](unsigned int i)const { return v[i]; }


  /** 行ベクトルの取得 
   * @param n 行のインデックス
   * @return n行目の行ベクトル
   */ 
  const Vector<w> row(unsigned int n)const { return v[n]; }
  void row(unsigned int n,const Vector<w>& V){ 
    v[n] = V;
  }

  /** 列ベクトルの取得 
   * @param n 列のインデックス
   * @return n列目の列ベクトル
   */ 
  const Vector<h> column(unsigned int n)const { 
    Vector<h> Vret;
    for(int i=0; i<h;i++ ) Vret[i] = v[i][n];
    return Vret; 
  }

  /** 列ベクトルへの代入 
   * @param n 列のインデックス
   * @param V 代入するベクトル
   */
  void column(unsigned int n,const Vector<h>& V){ 
    for(int i=0; i<h;i++ ) v[i][n] = V[i];
  }

  Matrix& operator=(const Matrix& M){
    for(int i=0; i<h;i++ ) v[i] = M[i];
    return *this;
  }

  /** 転置演算子 */
  Matrix<w,h> operator~() const{ 
    Matrix<w,h> Mret;
    for(int i=0; i<w;i++ )
      for(int j=0; j<h;j++ )
	Mret[i][j] = v[j][i];
    return Mret;
  }
  Matrix& operator+=(const Matrix& M){
    for(int i=0;i<h;i++)  (*this)[i] += M[i];
    return *this;
  }
 Matrix& operator-=(const Matrix& M){
    for(int i=0;i<h;i++)  (*this)[i] -= M[i];
    return *this;
  }
  Matrix& operator*=(const double a){
    for(int i=0;i<h;i++)  (*this)[i] *= a;
    return *this;
  }
  Matrix& operator/=(const double a){
    for(int i=0;i<h;i++)  (*this)[i] /= a;
    return *this;
  }
  Matrix operator+(const Matrix& M) const{
    Matrix Mret= *this; 
    Mret += M;
    return Mret;
  }
  Matrix operator-(const Matrix& M) const{
    Matrix Mret= *this;
    Mret -= M;
    return Mret;
  }
  Matrix operator*(double c) const{
    Matrix Mret = *this;
    Mret *= c;
    return Mret;
  }
  Matrix operator/(double c) const{
    Matrix Mret = *this;
    Mret /= c;
    return Mret;
  }
  template <int M_w> 
    Matrix<h,M_w> operator*(const Matrix<w,M_w>& M) const{
    Matrix<h,M_w> Mret;
    for(int i=0; i<h; i++){
      for(int j=0; j<M_w; j++){
	Mret[i][j] = 0.0;
	for(int k=0; k<w; k++)
	  Mret[i][j] += v[i][k] * M[k][j];
      }
    }
    return Mret;
  }
  
  Vector<h> operator*(const Vector<w>& V)const{
    Vector<h> Vret;
    for(int i=0; i<h; i++)  Vret[i] += (v[i] & V);
    return Vret;
  }
  friend Matrix<h,w> operator*(double c, const Matrix<h,w>& M){
    Matrix<h,w> Mret;
    for(int i=0; i<h; i++)  Mret[i] = c * M[i];
    return Mret;
  }
  friend Matrix<h,w> operator/(double c, const Matrix<h,w>& M){
    Matrix<h,w> Mret;
    for(int i=0; i<h; i++)  Mret[i] =  M[i] / c;
    return Mret;
  }


  Matrix operator-()const{
    Matrix Mret;
    for(int i=0; i<h; i++)  Mret[i] = - v[i];
    return Mret;
  }

  /** LU分解を行う.
   * @param s ピボットのインデックス
   * @return ピボット交換の回数
   */
  int decompose(int *s){ // LU fact
    int i, j, mx;
    int sgn = 1;

    for (i=0; i<h ; i++) s[i] = i; // index of pivot excheange

    for (int k = 0; k < h - 1; k++) { // check pivot
      for (i = k+1 , mx = k; i < h; i++)
	if( fabs(v[i][k]) > fabs(v[mx][k]) )
	  mx = i;

      if( fabs( v[mx][k]) < 1e-6 ){
	fprintf(stderr,"Yal::Matrix::decompse : Warning : pivot is too small\n");
	return 0;
      }

      if (mx != k){ //exchange pivot index
	std::swap( s[k], s[mx] );
	sgn *= -1;
      }
      for (i = k+1 ; i < h; i++) { // forward elimination
	v[s[i]][k] /= v[s[k]][k]; // mulit
	for (j = k+1; j < w; j++)
	  v[s[i]][j] -= v[s[i]][k] * v[s[k]][j];
      }
    }
    return sgn;
  }

  /** 行列式を計算する.
   * @return 行列式の値
   */
  double det() const{
    int s[h];
    Matrix LU = *this;
    double det = LU.decompose(s);
    for(int i=0;i<h;i++)  det *= LU[s[i]][i];
    return det;
  }

  /** 逆行列を計算する.
   * @return 逆行列
   */
  Matrix<w,h> inv()const{

    if( w > h ){ // 横長
      const Matrix<h,w>& A = *this;
      return  ~A * (A*~A).inv();
    }
    else if( w < h ){ // 縦長
      const Matrix<h,w>& A = *this;
      return  (~A*A).inv() * ~A;
    }
    else{ // 正方行列
      if( w==3 ){ // 3x3 LU分解に比べ25%速い
	Matrix<w,h> Mret( v[1][1]*v[2][2] - v[1][2]*v[2][1],
			  v[2][1]*v[0][2] - v[2][2]*v[0][1],
			  v[0][1]*v[1][2] - v[0][2]*v[1][1],
			  v[1][2]*v[2][0] - v[1][0]*v[2][2],
			  v[2][2]*v[0][0] - v[2][0]*v[0][2],
			  v[0][2]*v[1][0] - v[0][0]*v[1][2],
			  v[1][0]*v[2][1] - v[1][1]*v[2][0],
			  v[2][0]*v[0][1] - v[2][1]*v[0][0],
			  v[0][0]*v[1][1] - v[0][1]*v[1][0] );
	double det=
	  v[0][0]*Mret[0][0]+v[1][0]*Mret[0][1]+v[2][0]*Mret[0][2];
	if( fabs(det) < 1e-9 ){
	  fprintf(stderr,"Yal::Matrix::inv : Warning : det << 1\n");
	}
	return (1.0/det) * Mret;
      }
      else{
	Matrix<w,h> Mret;
	Mret.unitize();
	int s[h];
	Matrix LU = *this;
	LU.decompose(s);
	/*
	  for( int k = 0; k < h; k++ )
	  printf("%d ",s[k]);
	  printf("\n");
	*/
	for( int k = 0; k < h; k++ ){
	  for( int i = 0; i < h; i++ ){    /* 解を求める */
	    for( int j = i+1; j < h; j++ )
	      Mret[s[j]][k] -= Mret[s[i]][k] * LU[s[j]][i];
	  }
	  for( int i = h-1; i >= 0; i-- ){
	    for( int j = i+1; j<h; j++ )
	      Mret[s[i]][k] -= LU[s[i]][j] * Mret[s[j]][k];
	    Mret[s[i]][k] /= LU[s[i]][i];
	  }
	}
	for( int i = 0; i < h; i++ ){  
	  if( i == s[i] ) continue;
	  //printf("swap i=%d, %d <-> %d\n",i,s[i],s[s[i]]);
	  for( int j = 0; j < w; j++ )
	    std::swap( Mret[s[s[i]]][j], Mret[s[i]][j] );
	  std::swap( s[s[i]], s[i] ); //indexを元に戻していく
	}
	/*
	  for( int k = 0; k < h; k++ )
	  printf("%d ",s[k]);
	  printf("\n");
	*/
	return Mret;
      }
    
    }    
    
  }

  //Matrix operator^();
  /** 回転行列にする
   * @param axis 回転軸インデックス
   * @param theta 回転角度 [rad]
   */
  void setRot(Direction axis,double theta){
    const double C = cos(theta);
    const double S = sin(theta);
    
    switch(axis){
    case X:
      set(1.0, 0.0, 0.0,
	  0.0, C  , -S ,
	  0.0, S  ,  C  );
      break;
    case Y:
      set(C  , 0.0, S  ,
	  0.0, 1.0, 0.0,
	  -S , 0.0, C    );
      break;
    case Z:
      set(C  , -S , 0.0,
	  S  ,  C , 0.0,
	  0.0, 0.0, 1.0  );
      break; 
    default:
      unitize();
      fprintf(stderr,"la : invalid axis %d\n",axis);
      break;
    }
  }
 
  /** 回転行列にする
   * @param axis 回転軸ベクトル (規格化されていなくてもよい)
   * @param theta 回転角度 [rad]
   */
  void setRot(Vector<3> s,double theta){
    const double C = cos(theta);
    const double S = sin(theta);
    const double mC = 1.0 - C;
    s.normalize();
    v[0][0] = s[0] * s[0]*mC + C;
    v[0][1] = s[0] * s[1]*mC - s[2]*S;
    v[0][2] = s[2] * s[0]*mC + s[1]*S;
    v[1][0] = s[0] * s[1]*mC + s[2]*S;
    v[1][1] = s[1] * s[1]*mC + C;
    v[1][2] = s[1] * s[2]*mC - s[0]*S;
    v[2][0] = s[2] * s[0]*mC - s[1]*S;
    v[2][1] = s[1] * s[2]*mC + s[0]*S;
    v[2][2] = s[2] * s[2]*mC + C;
  }

  /** 正規直交性を調べる
   * @param flag_verbose trueのときに結果を標準出力に表示する
   * @return 正規直交であればtrue
   */
  bool check_orthonormality(bool flag_verbose=false){
    bool flag_error=false;
    for(int i=0;i<3;i++){
      double abs=v[i].abs();
      if( flag_verbose ) printf("abs%d = %f\n",i,abs);
      if( fabs( abs -1.0 ) > 1e-15 )
	flag_error=true;
    }

    for(int i=0;i<3;i++){
      double inner = i<2 ? v[i]&v[i+1] : v[2]&v[0];
      if( flag_verbose ) printf("inner%d = %f\n",i,inner);
      if( fabs(inner) > 1e-15 ) flag_error=true;
    }

    if( flag_error && flag_verbose){
      printf("Yal::Matrix : failed to be orthonormal\n");
      print();
    }
    return !flag_error;
  }
  /** 単位行列化する.   */
  void unitize(){
    for(int i=0;i<h;i++)
      for(int j=0;j<w;j++)
	v[i][j] = (i==j) ? 1.0 : 0.0;
  }

  /** 要素を全て0にする.   */
  void zero(){
    for(int i=0;i<h;i++)
      v[i].zero();
  }


  void rotate(Direction axis, double theta){
    (*this) = Matrix<3,3>(axis,theta) * (*this);
  }
  void rotate(const Vector<3>& axis, double theta){
    (*this) = Matrix<3,3>(axis,theta) * (*this);
  }
  void rotateR(Direction axis, double theta){
    (*this) = (*this) * Matrix<3,3>(axis,theta);
  }
  void rotateR(const Vector<3>& axis, double theta){
    (*this) = (*this) * Matrix<3,3>(axis,theta);
  }
  /*
  void transpose(){
    for(int i=1; i<h;i++)
      for(int j=0; j<i-1; j++)
	Yal::swap( v[i][j] , v[j][i] );
  }
  */
  /** 標準出力への表示 
   * @param n 改行の数
   */
  void print(int n=1) const{
    for(int i=0;i<h;i++){
      for(int j=0;j<w;j++)
	printf("%10.3f ",v[i][j] );
      putchar('\n');
    }
    for(int i=0;i<n-1;i++) putchar('\n');
  }

  /** 要素のセット
   */
  void set(double v00 , ...) {
    va_list args;
    va_start(args ,v00);
    v[0][0] = v00;
    for (int i = 0 ; i < h ; i++) {
      for (int j = 0 ; j < w ; j++){
	if( i == 0 && j == 0 ) continue;
	v[i][j] = (double) va_arg(args , double);
      }
    }
    va_end(args);
  }
  
  /** 部分行列の代入 
   * @param n 部分行列の開始行
   * @param n 部分行列の開始列
   * @param Ms 代入する行列
   */
  template <int ns,int ms> 
  void insert(int n,int m,Matrix<ns,ms> Ms){
    for(int i=0;i<ns;i++)
      for(int j=0;j<ms;j++)
	v[i+n][j+m] = Ms[i][j];
  }

  /** 部分ベクトルの代入 
   * @param n 部分ベクトルの開始行
   * @param n 部分ベクトルの開始列
   * @param V 代入するベクトル
   */
 template <int ns> 
  void insert(int n,int m,Vector<ns> V){
    for(int i=0;i<ns;i++)
      v[i+n][m] = V[i];
  }
 
 /** 対角化する.
  */
 int diagonalize(int loopMax,double eps){
   double eigenVec[h*w];
   const int& n = h;

   int row,column;
   
   // 固有ベクトル準備
   for(int i=0;i<n;i++){
     for(int j=0;j<n;j++){
       if(i != j){ eigenVec[i*n + j] = 0.0; }
     }
     eigenVec[i*n + i] = 1.0;
   }
   
   for(int loop = 0;loop < loopMax; loop++ ){
     double elementAbsMax = 0.0; // 非対角要素の絶対値の最大値

     // 最大値探索
     for(int i=0;i<n;i++){
       for(int j=i+1;j<n;j++){
	 double elementAbs = fabs(v[i][j]);
	 // 非対角要素の絶対値(大きさ比較用)

	 if(elementAbs > elementAbsMax){
	   elementAbsMax = elementAbs;
	   row	   = i;
	   column = j;
	 }
       }
     }
     if(elementAbsMax < eps){
       return 0;
     }

     double aRR = v[row][row]; //最大値に関係する変数     
     double aCC = v[column][column];
     double aRC = v[row][column];
     
     // 以下、公式の記述
     double t1 = fabs(aRR - aCC); //公式を記述するための中間変数
     double t2 = t1 * t1;
     double t3 = 4.0 * aRC * aRC;

     double tan = 2.0 * aRC / (t1 + sqrt(t2 + t3)) ;
     double cos = sqrt(1.0 / (1.0 + tan * tan));
     
     if(aRR < aCC){ tan = -tan; }
     double sin = tan * cos;
     
     for(int i=0;i<n;i++){
       double tmp  = eigenVec[i*n + row];
       eigenVec[i*n + row]    = tmp *  cos + eigenVec[i*n + column] * sin;
       eigenVec[i*n + column] = tmp * -sin + eigenVec[i*n + column] * cos;
       
       if(i != row && i != column){
	 tmp = v[i][row];
	 v[i][row]    = tmp *  cos + v[i][column] * sin;
	 v[i][column] = tmp * -sin + v[i][column] * cos;
	 
	 v[row][i]    = v[i][row];
	 v[column][i] = v[i][column];
       }
     }
     v[row][row] = aRR*cos*cos + aCC*sin*sin + 2.0*aRC*cos*sin;
     
     v[column][column] = aRR + aCC - v[row][row];
     v[row][column]    = 0.0;
     v[column][row]    = 0.0;
     
   }
 }
 
 Matrix expm2(int n=5) const{
   Matrix Mret;
   Matrix T = (*this);
   Mret += T;
   int ka=1;
   for(int k=2;k<n;k++){
     T = T * (*this);
     ka *= k;
     Mret += (1.0/ka) * T;
   }
   return Mret;
 }

 /** 行列の指数関数 */
 Matrix expm() const{
   const int q = 6;
   double cc=0.5;

   double s = ceil( 1 + log( row(h-1).abs2() )/log(2)*0.5 );
   //printf("s = %f \n",s);
   Matrix A = 1.0/pow(2.0,s)*(*this);
   Matrix X = A;
   Matrix I;
   Matrix E = I + cc*A;
   Matrix D = I - cc*A;
   
   for(int k=2;k<=q;k++){
     cc = cc*(q-k+1)/(k*(2.0*q-k+1.0));
     Matrix XX = A*X;
     
     //printf("%f \n", XX[0][0]);
    
     for(int i=0;i<h;i++) {
       for(int j=0;j<w;j++) {
	 X[i][j] = cc*XX[i][j];
	 E[i][j] = E[i][j]+X[i][j];
	 if( k%2 == 0 ) 
	   D[i][j] = D[i][j]+X[i][j];
	 else
	   D[i][j] = D[i][j]-X[i][j];
	 
	 //XX[i][j]=0.0;
       }
    }
    
   }

   Matrix EI = D.inv();
   Matrix F = EI * E;
   Matrix C;
   for(int k=1;k<=s;k++){
     C = F * F;
     for(int i=0;i<h;i++) {
       for(int j=0;j<w;j++) {		    
	 F[i][j] = C[i][j];
       }
     }
   }
   return C;
 }
};

extern Vector<3> EulerAngle(const Vector<3>& n,
			    const Vector<3>& m);
extern Vector<3> EulerAngle(const Matrix<3,3>& R);
extern Vector<3> RPYAngle(const Matrix<3,3>& R);
extern Vector<3> rotate(const Vector<3>& V,Direction axis,
			double theta);
extern Vector<3> rotate(const Vector<3>& V, 
			const Vector<3>& axis,
			double theta);
extern Matrix<3,3> rotate(const Matrix<3,3>& A,Direction axis,
			  double theta);
extern Matrix<3,3> rotate(const Matrix<3,3>& A,
			  const Vector<3>& axis,
			  double theta);
extern Matrix<3,3> ZYZMatrix(double a,double b,double c);
extern Matrix<3,3> ZYZMatrix(const  Vector<3>& euler);
extern Vector<3>   ZYZAngle(const Vector<3>& n,const Vector<3>& m);
extern Vector<3>   ZYZAngle(const Matrix<3,3>& R);
extern Matrix<3,3> RPYMatrix(double r,double p,double y);
extern Matrix<3,3> RPYMatrix(const Vector<3>& rpy);
extern Matrix<3,3> YXZMatrix(double p,double y,double r);
extern Matrix<3,3> YXZMatrix(const Vector<3>& pyr);
extern Matrix<3,3> YXZMatrix(const Vector<3>& n,double roll);
extern Vector<3>   YXZAngle(const Matrix<3,3>& R);
extern Vector<3>   YXZAngle(const Vector<3>& n,double roll);
extern Matrix<3,3> XYZMatrix(double y,double p,double r);
extern Matrix<3,3> XYZMatrix(const Vector<3>& ypr);
extern Matrix<3,3> XYZMatrix(const Vector<3>& n,double roll);
extern Vector<3>   XYZAngle(const Matrix<3,3>& R);
extern Vector<3>   XYZAngle(const Vector<3>& n,double roll);
extern Matrix<3,3> XZYMatrix(double x,double z,double y);
extern Matrix<3,3> XZYMatrix(const Vector<3>& pose);
extern Vector<3>   XZYAngle(const Matrix<3,3>& R);
extern Matrix<3,3> YZXMatrix(double y,double z,double x);
extern Matrix<3,3> YZXMatrix(const Vector<3>& pose);
extern Vector<3>   YZXAngle(const Matrix<3,3>& R);        
extern Matrix<3,3> ZXYMatrix(double z,double x,double y); // 追加
extern Matrix<3,3> ZXYMatrix(const Vector<3>& pose);       // 追加
extern Vector<3>   ZXYAngle(const Matrix<3,3>& R);          // 追加
extern Matrix<3,3> ITPMatrix(double x,double y,double z);
extern Matrix<3,3> ITPMatrix(const Vector<3>& pose);
extern Vector<3>   ITPAngle(const Matrix<3,3>& R);


extern Matrix<3,3> rotationMatrix(const Vector<3>& n,
				  const Vector<3>& m);
template<int l>
Vector<l> normal(const Vector<l>& V){
  Vector<l> Vret = V;
  Vret.normalize();
  return Vret;
}
template<int l>
Vector<l> projection(const Vector<l>& V,const Vector<l>& n){
  return (V&n) * n;
}
inline void inPMPI(double &theta){
  theta = fmod( theta, M_PI ) - (int)(theta/M_PI)%2 * M_PI;
}


extern const Vector<3>& ex();
extern const Vector<3>& ey();
extern const Vector<3>& ez();
extern const Vector<3>& o();
extern const Matrix<3,3>& I();
extern const Matrix<3,3>& O();

}

extern const double DEG;

#undef DEBUG_PRINT

#endif
