#ifndef FLT_H
#define FLT_H

#define USE_FLT

#include <Arduino.h>
#include <math.h>

// Максимальная длина буфера
#define FLT_LAG_MAX 50

// Глобальные переменные
extern int FLT_lag;          // Длина измеряемого буфера, не должна быть больше FLT_LAG_MAX
extern float FLT_TOL;        // Не знаю
extern float FLT_THRESHOLD;  // Чувствительность к всплескам
extern float FLT_INFLUENCE;  // Влияние текущего значения на фильтрованное значение
extern int FLT_c;            // Счетчик для накопления при старте
extern byte FLT_type;        // Тип фильтра (0 или 1)
// extern int FLT_D[FLT_LAG_MAX]; // Буфер данных
// extern int FLT_fildy;        // Последнее значение фильтра

extern uint16_t MaxA0;
extern uint16_t MinA0;
extern uint8_t FltA0;

// Инициализация фильтра
void FLT_init();
// Фильтрация
int FLT(int x);
int FLT_ARIFM(int x);

// Вспомогательные функции
// static int get_mean(int* buf, int N);
// static int get_stdd(int* buf, int mean, int N);
// static void FLT_push(int x);

#endif // FLT_H
