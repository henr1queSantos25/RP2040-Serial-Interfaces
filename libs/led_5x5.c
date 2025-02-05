#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "hardware/adc.h"
#include "pico/bootrom.h"
#include "hardware/timer.h"

// ARQUIVO .pio
#include "RP2040SerialInterfaces.pio.h"

// NÚMERO DE LEDS
#define NUM_PIXELS 25

// PINO DA MATRIZ DE LED
#define OUT_PIN 7

uint8_t volatile led_r = 0; // Intensidade do vermelho
uint8_t volatile led_g = 0; // Intensidade do verde
uint8_t volatile led_b = 100; // Intensidade do azul

int volatile count_color = 1; //Variável que cuida da cor dos leds na matriz

double numbers[10][NUM_PIXELS] = {
    // Número 0
    {0, 0, 1, 1, 1,
     1, 0, 1, 0, 0,
     0, 0, 1, 0, 1,
     1, 0, 1, 0, 0,
     0, 0, 1, 1, 1},

    // Número 1
    {0, 0, 0, 0, 1,
     1, 0, 0, 0, 0,
     0, 0, 0, 0, 1,
     1, 0, 0, 0, 0,
     0, 0, 0, 0, 1},

    // Número 2
    {0, 0, 1, 1, 1,
     1, 0, 0, 0, 0,
     0, 0, 1, 1, 1,
     0, 0, 1, 0, 0,
     0, 0, 1, 1, 1},

    // Número 3
    {0, 0, 1, 1, 1,
     1, 0, 0, 0, 0,
     0, 0, 1, 1, 1,
     1, 0, 0, 0, 0,
     0, 0, 1, 1, 1},

    // Número 4
    {0, 0, 1, 0, 1,
     1, 0, 1, 0, 0,
     0, 0, 1, 1, 1,
     1, 0, 0, 0, 0,
     0, 0, 0, 0, 1},

    // Número 5
    {0, 0, 1, 1, 1,
     0, 0, 1, 0, 0,
     0, 0, 1, 1, 1,
     1, 0, 0, 0, 0,
     0, 0, 1, 1, 1},

    // Número 6
    {0, 0, 1, 1, 1,
     0, 0, 1, 0, 0,
     0, 0, 1, 1, 1,
     1, 0, 1, 0, 0,
     0, 0, 1, 1, 1},

    // Número 7
    {0, 0, 1, 1, 1,
     1, 0, 0, 0, 0,
     0, 0, 0, 0, 1,
     1, 0, 0, 0, 0,
     0, 0, 0, 0, 1},

    // Número 8
    {0, 0, 1, 1, 1,
     1, 0, 1, 0, 0,
     0, 0, 1, 1, 1,
     1, 0, 1, 0, 0,
     0, 0, 1, 1, 1},

    // Número 9
    {0, 0, 1, 1, 1,
     1, 0, 1, 0, 0,
     0, 0, 1, 1, 1,
     1, 0, 0, 0, 0,
     0, 0, 1, 1, 1}};

// FUNÇÃO QUE RETORNA A COR DO LED
static inline uint32_t urgb_u32(double r, double g, double b)
{
    return ((uint32_t)(r) << 8) | ((uint32_t)(g) << 16) | (uint32_t)(b);
}

//LIGA OS LEDS NECESSÁRIOS PARA FORMAR UM NÚMERO
static inline void put_pixel(uint32_t pixel_grb)
{
    pio_sm_put_blocking(pio0, 0, pixel_grb << 8u);
}

//DESENHA O NÚMERO NA MATRIZ 5X5
void drawMatrix(int digit)
{
    uint32_t valor_led;
    for (int16_t i = 0; i < NUM_PIXELS; i++)
    {
        if (numbers[digit][24 - i])
        {
            valor_led = urgb_u32(led_r, led_g, led_b);
        }
        else
        {
            valor_led = urgb_u32(0, 0, 0);
        }
        put_pixel(valor_led);
    }
}

//MUDAR A COR DOS LEDS NA MATRIZ 5X5
void changeMatrixColor()
{

        if (count_color == 1)
        {
            led_r = 0;
            led_g = 0;
            led_b = 100;
        }
        else if (count_color == 2)
        {
            led_r = 100;
            led_g = 0;
            led_b = 0;
        }
        else
        {
            led_r = 0;
            led_g = 100;
            led_b = 0;
        }

        count_color++;

        if (count_color > 3)
        {
            count_color = 1;
        }
    
}
