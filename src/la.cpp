#include<math.h>
#include<stdlib.h>
#include<stdarg.h>
#include"la.h"

using namespace Yal;

const double DEG = 180/M_PI;

const Vector<3>& Yal::ex(){
  static const Vector<3> i(1.0, 0.0, 0.0);
  return i;
}
const Vector<3>& Yal::ey(){
  static const Vector<3> j(0.0, 1.0, 0.0);
  return j;
}
const Vector<3>& Yal::ez(){
  static const Vector<3> k(0.0, 0.0, 1.0);
  return k;
}
const Vector<3>& Yal::o(){
  static const Vector<3> zero;
  return zero;
}
const Matrix<3,3>& Yal::I(){
  static const Matrix<3,3> im(1.0, 0.0, 0.0,
			      0.0, 1.0, 0.0,
			      0.0, 0.0, 1.0 );
  return im;
}
const Matrix<3,3>& Yal::O(){
  static const Matrix<3,3>Om(0.0, 0.0, 0.0,
			     0.0, 0.0, 0.0,
			     0.0, 0.0, 0.0 );
  return Om;
}

/******************************************************
 * Euler Angles
 ******************************************************/
/******************************************************
 * ZYZ  Ek Ej Ek
 ******************************************************/
Vector<3> Yal::ZYZAngle(const Vector<3>& n,const Vector<3>& m){
  Vector<3> euler;
  euler[0] = atan2( n[1], n[0] );// + M_PI;
  double Ca = cos( euler[0] ); 
  double Sa = sin( euler[0] );
  euler[1] = atan2( Ca*n[0] + Sa*n[1],  n[2] );
  euler[2] = atan2( -sin(euler[1]) * ( Ca*m[1] -Sa*m[0] ),  m[2] );
  return euler;
}
Vector<3> Yal::ZYZAngle(const Matrix<3,3>& R){
  Vector<3> euler;
  euler[0] = atan2( R[1][2], R[0][2] );
  //euler[1] = atan2( sqrt(R[0][2]*R[0][2]+R[1][2]*R[1][2]), R[2][2] );
  //euler[2] = atan2( -R[2][1], R[2][0] );
  double Ca = cos( euler[0] ); 
  double Sa = sin( euler[0] );
  euler[1] = atan2( Ca*R[0][2] + Sa*R[1][2],  R[2][2] );
  euler[2] = atan2( -sin(euler[1]) * ( Ca*R[1][0] -Sa*R[0][0] ),  R[2][0] );

  return euler;
}
Matrix<3,3> Yal::ZYZMatrix(double a,double b,double c){
  double Sa = sin(a);  double Ca = cos(a);
  double Sb = sin(b);  double Cb = cos(b);
  double Sc = sin(c);  double Cc = cos(c);
  return Matrix<3,3> ( Ca*Cb*Cc - Sa*Sc, -Ca*Cb*Sc - Sa*Cc, Ca*Sb,
		       Sa*Cb*Cc + Ca*Sc, -Sa*Cb*Sc + Ca*Cc, Sa*Sb,
		       -Sb*Cc,           Sb * Sc,           Cb     );
}
Matrix<3,3> Yal::ZYZMatrix(const Vector<3>& euler){
  return ZYZMatrix(V3(euler));
}
/******************************************************
 * ZYX  Ek Ej Ek
 ******************************************************/
Vector<3> Yal::RPYAngle(const Matrix<3,3>& R){
  return Vector<3>( atan2( R[1][0], R[0][0] ),
		    atan2(-R[2][0], sqrt(R[0][0]*R[0][0]+R[1][0]*R[1][0]) ),
		    atan2( R[2][1],  R[2][2] ) );
}

Matrix<3,3> Yal::RPYMatrix(double r,double p,double y){
  double Sr = sin(r);  double Cr = cos(r);
  double Sp = sin(p);  double Cp = cos(p);
  double Sy = sin(y);  double Cy = cos(y);
  return Matrix<3,3> ( Cr*Cp, Cr*Sp*Sy - Sr*Cy, Cr*Sp*Cy + Sr*Sy,
		       Sr*Cp, Sr*Sp*Sy + Cr*Cy, Sr*Sp*Cy - Cr*Sy,
		       -Sp  ,        Cp * Sy  ,      Cp*Cy    );
}
Matrix<3,3> Yal::RPYMatrix(const Vector<3>& rpy){
  return RPYMatrix(V3(rpy));
}

/******************************************************
 * YXZ  Ej Ei Ek
 ******************************************************/
Matrix<3,3> Yal::YXZMatrix(double p,double y,double r){
  double Sp = sin(p);  double Cp = cos(p);
  double Sy = sin(y);  double Cy = cos(y);
  double Sr = sin(r);  double Cr = cos(r);
  return Matrix<3,3> ( Cp *Cr+Sp*Sy*Sr, -Cp*Sr+Sp*Sy*Cr, Sp*Cy,
		       Cy*Sr,      Cy*Cr         ,  -Sy,
		       -Sp*Cr+Cp*Sy*Sr, Sp *Sr+Cp*Sy*Cr, Cp*Cy );
}
Matrix<3,3> Yal::YXZMatrix(const Vector<3>& pyr){
  return YXZMatrix(V3(pyr));
}
Matrix<3,3> Yal::YXZMatrix(const Vector<3>& n,double roll){
  return YXZMatrix( YXZAngle(n,roll) );
}
Vector<3> Yal::YXZAngle(const Vector<3>& n,double r){
  return Vector<3>( atan2( n[0], n[2] ),
		    atan2( -n[1], sqrt(n[0]*n[0]+n[2]*n[2]) ),
		    r );
}
Vector<3> Yal::YXZAngle(const Matrix<3,3>& R){

  return Vector<3>( atan2( R[0][2], R[2][2] ),
		    atan2( -R[1][2], sqrt(R[0][2]*R[0][2]+R[2][2]*R[2][2]) ),
		    atan2( R[1][0], R[1][1] ) );
}

/****************************************************************
 * ITP  Ek Ej Ei 
 ****************************************************************/
Matrix<3,3> Yal::ITPMatrix(double tz,double ty,double tx){
  double Sx = sin(tx);  double Cx = cos(tx);
  double Sy = sin(ty);  double Cy = cos(ty);
  double Sz = sin(tz);  double Cz = cos(tz);
  return Matrix<3,3> ( Cz*Cy , Cz*Sy*Sx-Sz*Cx, Cz*Sy*Cx+Sz*Sx,
		       Sz*Cy , Sz*Sy*Sx+Cz*Cx, Sz*Sy*Cx-Cz*Sx,
		       -Sy   , Cy*Sx         , Cy*Cx );
}
Matrix<3,3> Yal::ITPMatrix(const Vector<3>& xyz){
  return ITPMatrix(V3(xyz));
}
/*
Matrix<3,3> Yal::ITPMatrix(const Vector<3>& n,double roll){
  return XYZMatrix( XYZAngle(n,roll) );
}
Vector<3> Yal::XYZAngle(const Vector<3>& n,double r){
  return Vector<3>( atan2( -n[1], n[2] ),
		    atan2( n[0], sqrt(n[1]*n[1]+n[2]*n[2]) ),  r );
}
*/
Vector<3> Yal::ITPAngle(const Matrix<3,3>& R){
  return Vector<3>( atan2( R[1][0], R[0][0] ),
		    atan2(-R[2][0], sqrt(R[2][1]*R[2][1]+R[2][2]*R[2][2]) ),
		    atan2( R[2][1], R[2][2] ));
}


/****************************************************************
 * XYZ  Ei Ej Ek XYZ
 ****************************************************************/
Matrix<3,3> Yal::XYZMatrix(double tx,double ty,double tz){
  double Sx = sin(tx);  double Cx = cos(tx);
  double Sy = sin(ty);  double Cy = cos(ty);
  double Sz = sin(tz);  double Cz = cos(tz);
  return Matrix<3,3> (          Cy*Cz,         -Cy*Sz,     Sy,
		                   Cx*Sz+Sx*Sy*Cz, Cx*Cz-Sx*Sy*Sz, -Sx*Cy,
		                   Sx*Sz-Cx*Sy*Cz, Sx*Cz+Cx*Sy*Sz,  Cx*Cy );
}
Matrix<3,3> Yal::XYZMatrix(const Vector<3>& ypr){
  return XYZMatrix(V3(ypr));
}
Matrix<3,3> Yal::XYZMatrix(const Vector<3>& n,double roll){
  return XYZMatrix( XYZAngle(n,roll) );
}
Vector<3> Yal::XYZAngle(const Vector<3>& n,double r){
  return Vector<3>( atan2( -n[1], n[2] ),
		    atan2( n[0], sqrt(n[1]*n[1]+n[2]*n[2]) ),  r );
}
Vector<3> Yal::XYZAngle(const Matrix<3,3>& R){
  return Vector<3>( atan2( -R[1][2], R[2][2] ),
		    atan2( R[0][2], sqrt(R[1][2]*R[1][2]+R[2][2]*R[2][2]) ),
		    atan2( -R[0][1], R[0][0] ) );
}

/****************************************************************
 * XZY  Ei Ek Ej
 ****************************************************************/
Matrix<3,3> Yal::XZYMatrix(double x,double z,double y){
  double Sx = sin(x);  double Cx = cos(x);
  double Sz = sin(z);  double Cz = cos(z);
  double Sy = sin(y);  double Cy = cos(y);
  return Matrix<3,3>(          Cz*Cy,   -Sz,          Cz*Sy,
		                  Cx*Sz*Cy+Sx*Sy, Cx*Cz, Cx*Sz*Sy-Sx*Cy,
		                  Sx*Sz*Cy-Cx*Sy, Sx*Cz, Sx*Sz*Sy+Cx*Cy  );
}
Matrix<3,3> Yal::XZYMatrix(const Vector<3>& pose){
  return XZYMatrix(V3(pose));
}
/*
Matrix<3,3> Yal::XZYMatrix(const Vector<3>& n,double roll){
  return XZYMatrix( XZYAngle(n,roll) );
}
Vector<3> XZYAngle(const Vector<3>& n,double r){
}
*/
Vector<3> Yal::XZYAngle(const Matrix<3,3>& R){
  return Vector<3>( atan2( R[2][1], R[1][1] ),
		    atan2(-R[0][1], sqrt(R[0][0]*R[0][0]+R[0][2]*R[0][2])),
		    atan2( R[0][2], R[0][0] ) );
}


/****************************************************************
 * YZX  Ej Ek Ei
 ****************************************************************/
Matrix<3,3> Yal::YZXMatrix(double y,double z,double x){
  double Sx = sin(x);  double Cx = cos(x);
  double Sz = sin(z);  double Cz = cos(z);
  double Sy = sin(y);  double Cy = cos(y);
  return Matrix<3,3>( Cy*Cz, -Sx*Sy-Sz*Cx*Cy,  Sx*Sz*Cy-Sy*Cx,
		                     Sz,           Cx*Cz,          -Sx*Cz,
		                  Sy*Cz,  Sx*Cy-Sy*Sz*Cx,  Sx*Sy*Sz+Cx*Cy  );
  // return Matrix<3,3>( Cy*Cz, -Cy*Sz*Cx+Sy*Sx,  Cy*Sz*Sx+Sy*Cx,
	// 	                     Sz,           Cz*Cx,          -Cz*Sx,
	// 	                 -Sy*Cz,  Sy*Sz*Cx+Cy*Sx, -Sy*Sz*Sx+Cy*Cx  );
}
Matrix<3,3> Yal::YZXMatrix(const Vector<3>& pose){
  return YZXMatrix(V3(pose));
}
Vector<3> Yal::YZXAngle(const Matrix<3,3>& R){
  return Vector<3>( atan2( -R[2][0], R[0][0] ),
		    //atan2( R[1][0], sqrt(R[0][0]*R[0][0]+R[2][0]*R[2][0])),
		    atan2( R[1][0], sqrt(R[1][1]*R[1][1]+R[1][2]*R[1][2])),
		    atan2( -R[1][2], R[1][1] ) );
}

Matrix<3,3> Yal::ZXYMatrix(double z,double x,double y){
  double Sx = sin(x);  double Cx = cos(x);
  double Sz = sin(z);  double Cz = cos(z);
  double Sy = sin(y);  double Cy = cos(y);
  return Matrix<3,3>( Sx*Sy*Sz+Cy*Cz, -Sz*Cx,  Sx*Sz*Cy-Sy*Cz,
		                 -Sx*Sy*Cz+Sz*Cy,  Cx*Cz, -Sx*Cy*Cz-Sy*Sz,
		                           Sy*Cx,     Sx,           Cx*Cy  );
}
Matrix<3,3> Yal::ZXYMatrix(const Vector<3>& pose){
  return ZXYMatrix(V3(pose));
}
Vector<3> Yal::ZXYAngle(const Matrix<3,3>& R){
  return Vector<3>( atan2( -R[0][1], R[1][1] ),
		    atan2( R[2][1], sqrt(R[2][0]*R[2][0]+R[2][2]*R[2][2])),
		    atan2( R[2][0], R[2][2] ) );
}

Matrix<3,3> Yal::rotationMatrix(const Vector<3>& n,const Vector<3>& m){
  Matrix<3,3> R;
  R.column(0,m);
  R.column(1,n*m);
  R.column(2,n);
  return R;
}


Vector<3> Yal::rotate(const Vector<3>& V,Direction axis,
		      double theta){
  return Matrix<3,3>(axis,theta) * V;
}
Vector<3> Yal::rotate(const Vector<3>& V, const Vector<3>& axis,
		      double theta){
  return Matrix<3,3>(axis,theta) * V;
}
Matrix<3,3> Yal::rotate(const Matrix<3,3>& A,Direction axis,
			double theta){
  return Matrix<3,3>(axis,theta) * A;
}
Matrix<3,3> Yal::rotate(const Matrix<3,3>& A, const Vector<3>& axis,
			double theta){
  return Matrix<3,3>(axis,theta) * A;
}


template class Vector<3>;



