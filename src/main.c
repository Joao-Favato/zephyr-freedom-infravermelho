#include <zephyr/kernel.h>             // Funções básicas do Zephyr (ex: k_msleep, k_thread, etc.)
#include <zephyr/device.h>             // API para obter e utilizar dispositivos do sistema
#include <zephyr/drivers/gpio.h>       // API para controle de pinos de entrada/saída (GPIO)
#include <pwm_z42.h>                // Biblioteca personalizada com funções de controle do TPM (Timer/PWM Module)
// Definição dos pinos GPIO para controle dos motores e leitura dos sensores
const struct device *gpio_b = DEVICE_DT_GET(DT_NODELABEL(gpiob));
const struct device *gpio_a = DEVICE_DT_GET(DT_NODELABEL(gpioa));
const struct device *gpio_e =  DEVICE_DT_GET(DT_NODELABEL(gpioe));
// Define o valor do registrador MOD do TPM para configurar o período do PWM
#define TPM_MODULE 4000         // Define a frequência do PWM fpwm = (TPM_CLK / (TPM_MODULE * PS))
// Valores de duty cycle correspondentes a diferentes larguras de pulso
uint16_t duty_50  = TPM_MODULE/2;       // 50% de duty cycle (meio brilho)
uint16_t duty_100  = TPM_MODULE;       // 100% de duty cycle
uint16_t duty_25  = TPM_MODULE/4;       // 25% de duty cycle
uint16_t duty_75  = TPM_MODULE * 3/4;   // 75% de duty cycle
uint16_t duty_0  = TPM_MODULE * 0;   // 0% de duty cycle
uint16_t duty_10  = TPM_MODULE/10;       // 10% de duty cycle (meio brilho)
uint16_t duty_20  = TPM_MODULE/5;       // 20% de duty cycle (meio brilho)
uint16_t duty_30  = TPM_MODULE * 0.3;       // 30% de duty cycle (meio brilho)
uint16_t duty_40  = TPM_MODULE * 0.4;       // 40% de duty cycle (meio brilho)
uint16_t duty_60  = TPM_MODULE * 0.6;       // 50% de duty cycle (meio brilho)
uint16_t duty_70  = TPM_MODULE * 0.7;       // 50% de duty cycle (meio brilho)
uint16_t duty_80  = TPM_MODULE * 0.8;       // 50% de duty cycle (meio brilho)
uint16_t duty_90  = TPM_MODULE * 0.9;       // 50% de duty cycle (meio brilho)

typedef enum {
    ESTADO_25,
    ESTADO_50,
    ESTADO_100
} estado_t;

int main(void)
{  
    int val1, val2;
       
    // Inicializa o módulo TPM2 com:
    // - base do TPMx
    // - fonte de clock PLL/FLL (TPM_CLK)
    // - valor do registrador MOD
    // - tipo de clock (TPM_CLK)
    // - prescaler de 1 a 128 (PS)
    // - modo de operação EDGE_PWM
    pwm_tpm_Init(TPM2, TPM_PLLFLL, TPM_MODULE, TPM_CLK, PS_64, EDGE_PWM);
    pwm_tpm_Init(TPM0, TPM_PLLFLL, TPM_MODULE, TPM_CLK, PS_64, EDGE_PWM);
   
    // Inicializa o canal do TPM2 para gerar sinal PWM nas portas GPIOB_2 e GPIOB_3
    pwm_tpm_Ch_Init(TPM2, 0, TPM_PWM_H, GPIOB, 2);
    pwm_tpm_Ch_Init(TPM2, 1, TPM_PWM_H, GPIOB, 3);


    // Define o valor do duty cycle
    // Canal 0 = PTB2, Canal 1 = PTB3
    pwm_tpm_CnV(TPM2, 0, duty_100);
    pwm_tpm_CnV(TPM2, 1, duty_100);

    //Configuração dos pinos para saída digital
    gpio_pin_configure(gpio_b, 0, GPIO_OUTPUT);
    gpio_pin_configure(gpio_b, 1, GPIO_OUTPUT);
    gpio_pin_configure(gpio_a, 4, GPIO_OUTPUT);
    gpio_pin_configure(gpio_a, 5, GPIO_OUTPUT);

    //Configuração dos pinos para entrada digital
    // Sensor esquerda: Porta E5
    // Sensor direita: Porta E4
    gpio_pin_configure(gpio_e, 4, GPIO_INPUT | GPIO_PULL_UP);
    gpio_pin_configure(gpio_e, 5, GPIO_INPUT | GPIO_PULL_UP);

   //Definição dos estados iniciais dos pinos
    ///Motor esquerda: Portas B0 e B1
    //// IN1 = B0 e IN2 = B1
    ///Motor direita: Portas A4 e A5
    //// IN3 = A4 e IN4 = A5
    gpio_pin_set(gpio_b, 0, 1);
    gpio_pin_set(gpio_b, 1, 0);
    gpio_pin_set(gpio_a, 4, 1);
    gpio_pin_set(gpio_a, 5, 0);

    pwm_tpm_CnV(TPM2, 0, duty_100);
    pwm_tpm_CnV(TPM2, 1, duty_100);
    k_msleep(15);
    // Loop infinito
    for (;;){  
        /// = 1 quando o sensor estiver sobre a linha preta; = 0 quando estiver fora da linha preta
        val1 = gpio_pin_get(gpio_e, 4); // Leitura do sensor direita
        val2 = gpio_pin_get(gpio_e, 5); // Leitura do sensor esquerda
                
        if (val1 == 0 && val2 == 0){ // Linha reta            
            pwm_tpm_CnV(TPM2, 0, duty_60);
            pwm_tpm_CnV(TPM2, 1, duty_60);
        } else if (val1 == 0 && val2 == 1) { // Curva para a direita            
            pwm_tpm_CnV(TPM2, 0, duty_60);
            pwm_tpm_CnV(TPM2, 1, duty_0);
        } else if (val1 == 1 && val2 == 0){ // Curva para a esquerda           
            pwm_tpm_CnV(TPM2, 0, duty_0);
            pwm_tpm_CnV(TPM2, 1, duty_60);
        } else {    
            pwm_tpm_CnV(TPM2, 0, duty_60);
            pwm_tpm_CnV(TPM2, 1, duty_60);
        }
            k_msleep(5); // Aguarda 10 ms antes da próxima leitura dos sensores


    }
    return 0;
}









