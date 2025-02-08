#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "hardware/adc.h"
#include "hardware/timer.h"
#include "hardware/i2c.h"
#include "libs/ssd1306.h"
#include "libs/font.h"

// ARQUIVO .pio
#include "RP2040SerialInterfaces.pio.h"

// DEFINIÇÃO DA PINAGEM
#define OUT_PIN 7
#define BUTTON_A 5
#define BUTTON_B 6
#define LED_GREEN 11
#define LED_BLUE 12
#define I2C_SDA 14
#define I2C_SCL 15

#define I2C_PORT i2c1
#define endereco 0x3C

ssd1306_t ssd; // Inicializa a estrutura do display

// FUNÇÃO EXTERNA DO ARQUIVO "led_5x5.c"
extern void drawMatrix(int digit);

// VARIÁVEIS VOLÁTEIS
uint32_t volatile last_time = 0; // variável que auxilia no debounce
bool volatile cor = false;

// PROTÓTIPOS
void initButtons(); // inicializa os botões
void initLeds(); // inicializa os leds
void initPIO(); // inicializa e configura o PIO
void initI2C(); // inicializa e configura a comunicação I2C com o display
void display_config(); // CONFIGURAÇÃO DO DISPLAY
void setLEDs(uint gpio, char *corLED); // alterna o estado do led e informa no display
void gpio_irq_handler(uint gpio, uint32_t events); // função que é chamada na interrupção

int main()
{
    // INICIALIZA UART PADRÃO
    stdio_init_all(); 

    // INICIALIZANDO BOTÕES E LEDS
    initButtons();
    initLeds();

    // INICIALIZA E CONFIGURA A COMUNICAÇÃO I2C COM O DISPLAY
    initI2C();

    // CONFIGURAÇÃO DO DISPLAY
    display_config();

    // configurações da PIO
    initPIO();

    // CONFIGURAÇÃO DAS INTERRUPÇÕES
    gpio_set_irq_enabled_with_callback(BUTTON_A, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);
    gpio_set_irq_enabled_with_callback(BUTTON_B, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);

    while (true)
    {
        // LOOP PRINCIPAL
        char caractere;
        scanf(" %c", &caractere);

        if (caractere >= '0' && caractere <= '9')
        {
            int digit = caractere - '0'; 
            drawMatrix(digit);
        }

        if(!ssd1306_is_empty(&ssd)){
            sleep_ms(50);
            ssd1306_fill(&ssd, false);
        }
        ssd1306_draw_char(&ssd, caractere, 0, 10);
        ssd1306_send_data(&ssd); //Atualiza display
        sleep_ms(1000);
    }
}

void gpio_irq_handler(uint gpio, uint32_t events)
{
    // Obtém o tempo atual em microssegundos para debounce
    uint32_t current_time = to_us_since_boot(get_absolute_time());

    // Verifica se o tempo mínimo de debounce passou (300ms)
    if (current_time - last_time > 300000)
    {
        // Limpa o display. O display inicia com todos os pixels apagados.
        ssd1306_fill(&ssd, false);
        ssd1306_send_data(&ssd);
        last_time = current_time;
        if (gpio == BUTTON_A)
        {
            setLEDs(LED_GREEN, "O LED VERDE");
            ssd1306_send_data(&ssd); // Atualiza o display
        }
        if (gpio == BUTTON_B)
        {
            setLEDs(LED_BLUE, "O LED AZUL");
            ssd1306_send_data(&ssd); // Atualiza o display
        }
    }
}

void setLEDs(uint gpio, char *corLED)
{
    gpio_put(gpio, !gpio_get(gpio));
    char *estado = (gpio_get(gpio)) ? "ESTA LIGADO" : "ESTA DESLIGADO";

    if (gpio_get(LED_BLUE) && gpio_get(LED_GREEN))
    {
        ssd1306_draw_string(&ssd, "O LED VERDE", 0, 10);
        ssd1306_draw_string(&ssd, "E O LED AZUL", 0, 25);
        ssd1306_draw_string(&ssd, "ESTAO LIGADOS", 0, 40);
        printf("O LED VERDE E O AZUL ESTAO LIGADOS\n");
    }
    else
    {
        ssd1306_draw_string(&ssd, corLED, 0, 10);
        ssd1306_draw_string(&ssd, estado, 0, 25);
        printf("%s %s\n", corLED, estado);
    }
}

// INICIALIZAÇÃO DOS BUTTONS
void initButtons()
{
    gpio_init(BUTTON_A);
    gpio_set_dir(BUTTON_A, GPIO_IN);
    gpio_pull_up(BUTTON_A);

    gpio_init(BUTTON_B);
    gpio_set_dir(BUTTON_B, GPIO_IN);
    gpio_pull_up(BUTTON_B);
}

// INICIALIZAÇÃO DOS LEDS
void initLeds()
{
    gpio_init(LED_BLUE);
    gpio_set_dir(LED_BLUE, GPIO_OUT);

    gpio_init(LED_GREEN);
    gpio_set_dir(LED_GREEN, GPIO_OUT);
}

// INICIALIZAÇÃO E CONFIGURAÇÃO DO PIO
void initPIO(){
    PIO pio = pio0;
    uint offset = pio_add_program(pio, &RP2040SerialInterfaces_program);
    uint sm = pio_claim_unused_sm(pio, true);
    RP2040SerialInterfaces_program_init(pio, sm, offset, OUT_PIN);
}

void initI2C(){
    // INICIALIZAÇÃO DO I2C COM 400Khz
    i2c_init(I2C_PORT, 400 * 1000);
    // CONFIGURAÇÃO DO I2C
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C); // Define a função do pino GPIO para I2C.
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C); // Define a função do pino GPIO para I2C.
    gpio_pull_up(I2C_SDA); // Configura pull-up para a linha de dados
    gpio_pull_up(I2C_SCL); // Configura pull-up para a linha de clock
}

void display_config(){
    ssd1306_init(&ssd, WIDTH, HEIGHT, false, endereco, I2C_PORT); // Inicializa o display
    ssd1306_config(&ssd); // Configura o display
    ssd1306_send_data(&ssd); // Envia os dados para o display
    ssd1306_fill(&ssd, false); // Limpa o display. O display inicia com todos os pixels apagados.
    ssd1306_send_data(&ssd); //Atualiza display
}