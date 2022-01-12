#pragma once

#define EPSILON 0.00001
//0 to 1
double random_double()
{
    return (double)rand() / (double)RAND_MAX;
}

bool f_equals(double a, double b) {return std::abs(a - b) < EPSILON;}
