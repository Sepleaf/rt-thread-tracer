#include "control.h"

int error[8] = {-7, -5, -3, -1, 1, 3, 5, 7};
int k[8] = {5, 3, 1, 0, 0, 1, 3, 5};

uint8_t gray_value;
uint8_t gray_bit[8];
uint8_t index;

PID_SAVE pid_save = {
    {0, 0, 0},
    {450, 0.5, 4600},
    {160, 0, 90}};

void gray_bias(int *k, int *error)
{
    uint8_t i;
    gray_value = get_gray_value();
    for (i = 0; i < 8; i++)
        gray_bit[i] = gray_value & (0x01 << i);

    for (i = 0; i < 8; i++)
        if (gray_bit[i] == 0)
            index = 7 - i;

    *k = k[index];
    *error = error[index];
}

int pwm_basic;
int pwm_location;

void motor_control(int k, int error)
{
    pwm_basic = pid_basic(k, pid_save.basic);
    pwm_location = pid_location(error, pid_save.location);

    load_pwm(pwm_basic + pwm_location, pwm_basic - pwm_location);
}

