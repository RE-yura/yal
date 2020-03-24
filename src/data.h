#ifndef __YAL_DATA
#define __YAL_DATA

#include "la.h"

namespace Yal {

class BufMS{ // for master-slave communication
 public:
  Yal::Vector<3> Pr;
  Yal::Matrix<3,3> Rot;
  Yal::Vector<3> f;
  
  void init(){
    Pr.zero();
    Rot.unitize();
    f.zero();
  }
};

class BufSM{ // for master-slave communication
 public:
  Yal::Vector<3> Pr;
  Yal::Matrix<3,3> Rot;
  Yal::Vector<3> f;
  
  void init(){
    Pr.zero();
    Rot.unitize();
    f.zero();
  }
};

}

#endif
