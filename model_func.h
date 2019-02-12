#pragma once

#include "view_func.h"

float max(float, float);
float absf(float);
float model_chart(int, float);
float pension_calc(int, float, float, float, int);
float convert_to_brutto(float);
float convert_to_netto(double);
void display_result(frame*, unsigned, int, float, float, float, int, unsigned);
void result_controller(frame*, unsigned);
