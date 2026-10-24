/*
 * Chapter 3.2 - Parameters and return values
 * Parameters carry values in; return carries one value out.
 */
#include <stdio.h>
#include <stdbool.h>

/* several parameters, one result */
double monthly_payment(double loan, double yearly_rate_percent, int years)
{
    double r = yearly_rate_percent / 100.0 / 12.0;   /* monthly rate */
    int    n = years * 12;                           /* number of payments */
    if (r == 0)
        return loan / n;
    double growth = 1.0;
    for (int i = 0; i < n; i++)
        growth *= (1 + r);                           /* (1 + r)^n */
    return loan * r * growth / (growth - 1);
}

/* returning a yes/no answer */
bool is_strong_password_length(int length)
{
    return length >= 12;      /* return can hand back any expression */
}

/* early return: leave as soon as the answer is known */
const char *bmi_category(double weight_kg, double height_m)
{
    if (height_m <= 0)
        return "invalid height";
    double bmi = weight_kg / (height_m * height_m);
    if (bmi < 18.5) return "underweight";
    if (bmi < 25.0) return "healthy";
    if (bmi < 30.0) return "overweight";
    return "obese";
}

int main(void)
{
    printf("Car loan $20000 at 6%% for 5 years: $%.2f a month\n",
           monthly_payment(20000, 6.0, 5));
    printf("House loan $300000 at 4.5%% for 30 years: $%.2f a month\n",
           monthly_payment(300000, 4.5, 30));
    printf("Password of 8 chars long enough? %s\n", is_strong_password_length(8) ? "yes" : "no");
    printf("70 kg, 1.75 m: %s\n", bmi_category(70, 1.75));
    printf("70 kg, 0 m:    %s\n", bmi_category(70, 0));
    return 0;
}
