#include<stdio.h>
#include<stdarg.h>
#include<stdlib.h>
#include<math.h>
#include"util.h"

/** 数値を時刻テキストに変換する.
 * @param t 時刻 [s]
 * @return 成形された文字列ポインタ
 */
const char* Yal::formatTime(double t,int ftype){
  static char time_str[128];
  int time = (int)t;
  switch(ftype){
  case HMSD:{
    int h    = time / 3600;
    int m    = (time%3600) / 60;
    int s    = time % 60;
    int ss   = (int)(10*(t-time));
    sprintf(time_str,"%3d:%02d:%02d:%01d",h,m,s,ss);
  }
    break;
  case HMS:{
    int h    = time / 3600;
    int m    = (time%3600) / 60;
    int s    = time % 60;
    sprintf(time_str,"%3d:%02d:%02d",h,m,s);
  }
    break;
  case MSD:{
    int m    = time / 60;
    int s    = time % 60;
    int ss   = (int)(10*(t-time));
    sprintf(time_str,"%02d:%02d:%01d",m,s,ss);
  }
    break;
  case MS:{
    int m    = time / 60;
    int s    = time % 60;
    sprintf(time_str,"%02d:%02d",m,s);
  }
    break;
  case SD:{
    int s    = time % 60;
    int ss   = (int)(10*(t-time));
    sprintf(time_str,"%02d:%01d",s,ss);
  }
    break;
  }
  /*
  int time = (int)t;
  int h    = time / 3600;
  int m    = (time%3600) / 60;
  int s    = time % 60;
  //int ss   = (int)(100*(t-time));
  int ss   = (int)(10*(t-time));
  //sprintf(time_str,"%3d:%02d:%02d:%02d",h,m,s,ss);
  sprintf(time_str,"%3d:%02d:%02d:%01d",h,m,s,ss);
  */
  return time_str;
}



/** シェルのコマンドを実行する.
 * systemと同じ機能.
 * printfと同様に変換指示子が使える.
 * @param format コマンドのフォーマット
 */
void Yal::shell(const char *format, ...){
  char   buf[256];
  va_list args;
  va_start(args,format);
  vsprintf(buf, format, args);
  va_end(args);

  system(buf);
}

FILE* Yal::popenf(const char *format,const char *rw, ...){
  char   buf[512];
  va_list args;
  va_start(args,rw);
  vsprintf(buf, format, args);
  va_end(args);
  return popen(buf,rw);
}

double Yal::get_cpufreq(){ //only user space
  double tps=1000;
  FILE* fp = popen("grep \"cpu MHz\" < /proc/cpuinfo","r");
  fscanf(fp,"%*s %*s %*s %lf",&tps);
  pclose(fp);
  tps *= 1e6;
  //printf("tps = %f\n",tps);
  return tps;
}


