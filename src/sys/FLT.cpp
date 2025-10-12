#include <Arduino.h>
#include "FLT.h"

uint16_t MaxA0 = 65535;
uint16_t MinA0 = 0;
uint8_t FltA0 = 255;

int FLT_lag; // длина измеряемого буфера, не должна быть больше FLT_LAG_MAX (50)
float FLT_TOL;
float FLT_THRESHOLD;
float FLT_INFLUENCE;
int FLT_c; // счетчик для первоначального накопления при старте, от 0 до FLT_lag
byte FLT_type = 0;

void FLT_init(void) {
  FLT_lag=5; // длина измеряемой буфера, должна быть менее < FLT_LAG_MAX (50)
  FLT_TOL=64; // хуй знает что такое, похоже вообещ не нужен
  FLT_THRESHOLD=3.0; // чувствительность к всплескам
  FLT_INFLUENCE=0.5;
}

int FLT_D[FLT_LAG_MAX]; // Буфер данных (50)
int FLT_fildy; //,FLT_c=0; 

// среднее арифметическое буфера (длиной FLT_lag)
static int get_mean(int* buf,int N) { int x=0; for(int i=0; i < N; i++) x += buf[i]; return x/N; }

// среднее квадратичное отклонение буфера (длиной FLT_lag)
static int get_stdd(int* buf, int mean, int N) { int x=0; for(int i=0,d;i<N;i++) { d=mean-buf[i]; x+=d*d; } return sqrt(x/N); }

// сдвинуть и записать
static void FLT_push(int x) { if(FLT_c<FLT_lag) FLT_c++; else { for(int i=1;i<=FLT_lag;i++) FLT_D[i-1]=FLT_D[i]; } FLT_D[FLT_lag]=x; }

int FLT(int x) {
    if(FLT_type == 1) return FLT_ARIFM(x);

    // только при старте для первоначального заполнения буфера
    if(FLT_c < FLT_lag) { FLT_fildy=x; FLT_push(x); return 1; }

    FLT_push(FLT_fildy); // добавить в буфер значение последнего всплеска FLT_fildy

    int mu=get_mean(FLT_D,FLT_lag); // взять среднее арифметическое

    // если нынешний x отличается от среднего арифметического менее чем на FLT_THRESHOLD*среднеквадратичное, то изменений нет
    if(abs(x-mu) <= FLT_THRESHOLD*get_stdd(FLT_D,mu,FLT_lag)) { FLT_fildy=x; return 1; }

    // зафиксирован всплеск!

    // только тогда вычислить новое FLT_fildy как смесь x с прошлым всплеском в пропорции (FLT_INFLUENCE 0.5 = 50:50)
    // FLT_TOL хуй знает что такое и похоже вообще не нужен
    FLT_fildy=((FLT_TOL*FLT_INFLUENCE * x) + (FLT_TOL*(1-FLT_INFLUENCE) * FLT_fildy)) / FLT_TOL;
    // FLT_fildy= FLT_INFLUENCE * x + (1-FLT_INFLUENCE) * FLT_fildy ;

    // вернуть результат всплеска: 2 - вверх, 1 - вниз
    return (x > mu) ? 2 : 0;
}

int FLT_ARIFM(int x) {
  int mu=get_mean(FLT_D,FLT_lag); // взять среднее арифметическое
  FLT_push(x); // добавить в буфер новое значение
  if(abs(x-mu) <= FLT_THRESHOLD*mu) return 1;
  return (x > mu) ? 2 : 0;
}
