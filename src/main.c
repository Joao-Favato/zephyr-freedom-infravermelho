#include <zephyr/kernel.h>             // Funções básicas do Zephyr (ex: k_msleep, k_thread, etc.)
#include <zephyr/device.h>             // API para obter e utilizar dispositivos do sistema
#include <zephyr/drivers/gpio.h>       // API para controle de pinos de entrada/saída (GPIO)
#include <pwm_z42.h>                // Biblioteca personalizada com funções de controle do TPM (Timer/PWM Module)
// Definição dos pinos GPIO para controle dos motores e leitura dos sensores
const struct device *gpio_b = DEVICE_DT_GET(DT_NODELABEL(gpiob));
const struct device *gpio_a = DEVICE_DT_GET(DT_NODELABEL(gpioa));
const struct device *gpio_e =  DEVICE_DT_GET(DT_NODELABEL(gpioe));
// Define o valor do registrador MOD do TPM para configurar o período do PWM
#define TPM_MODULE 1000         // Define a frequência do PWM fpwm = (TPM_CLK / (TPM_MODULE * PS))
// Valores de duty cycle correspondentes a diferentes larguras de pulso
uint16_t duty_50  = TPM_MODULE/2;       // 50% de duty cycle (meio brilho)
uint16_t duty_100  = TPM_MODULE;       // 100% de duty cycle
uint16_t duty_25  = TPM_MODULE/4;       // 25% de duty cycle


typedef enum {
    ESTADO_25,
    ESTADO_50,
    ESTADO_100
} estado_t;


int main(void)
{  
    int val1;
       
    // Inicializa o módulo TPM2 com:
    // - base do TPMx
    // - fonte de clock PLL/FLL (TPM_CLK)
    // - valor do registrador MOD
    // - tipo de clock (TPM_CLK)
    // - prescaler de 1 a 128 (PS)
    // - modo de operação EDGE_PWM
    pwm_tpm_Init(TPM2, TPM_PLLFLL, TPM_MODULE, TPM_CLK, PS_128, EDGE_PWM);
    pwm_tpm_Init(TPM0, TPM_PLLFLL, TPM_MODULE, TPM_CLK, PS_128, EDGE_PWM);
   
    // Inicializa o canal do TPM2 para gerar sinal PWM nas portas GPIOB_2 e GPIOB_3
    pwm_tpm_Ch_Init(TPM2, 0, TPM_PWM_H, GPIOB, 2);
    pwm_tpm_Ch_Init(TPM2, 1, TPM_PWM_H, GPIOB, 3);


    // Define o valor do duty cycle
    pwm_tpm_CnV(TPM2, 0, duty_100);
    pwm_tpm_CnV(TPM2, 1, duty_100);


    //Configuração dos pinos para saída digital
    gpio_pin_configure(gpio_b, 0, GPIO_OUTPUT);
    gpio_pin_configure(gpio_b, 1, GPIO_OUTPUT);
    gpio_pin_configure(gpio_a, 4, GPIO_OUTPUT);
    gpio_pin_configure(gpio_a, 5, GPIO_OUTPUT);


    //Configuração dos pinos para entrada digital
    gpio_pin_configure(gpio_e, 4, GPIO_INPUT | GPIO_PULL_UP);
    gpio_pin_configure(gpio_e, 5, GPIO_INPUT | GPIO_PULL_UP);




    //Definição dos estados iniciais dos pinos
    ///Motor esquerda: Portas B0 e B1
    //// IN1 = B0 e IN2 = B1
    ///Motor direita: Portas A4 e A5
    //// IN3 = A4 e IN4 = A5
    gpio_pin_set(gpio_b, 0, 0);
    gpio_pin_set(gpio_b, 1, 0);
    gpio_pin_set(gpio_a, 4, 0);
    gpio_pin_set(gpio_a, 5, 0);


   
    // Loop infinito
    for (;;){  
        val1 = gpio_pin_get(gpio_e, 4); // Leitura do sensor 1
        //// Carro anda com um sensor quando ele estiver em cima da linha preta
        if (val1 == 0){
            gpio_pin_set(gpio_b, 0, 1);
            gpio_pin_set(gpio_a, 4, 1);
        } else {
            gpio_pin_set(gpio_b, 0, 0);
            gpio_pin_set(gpio_a, 4, 0);
        }
    }
    return 0;
}
