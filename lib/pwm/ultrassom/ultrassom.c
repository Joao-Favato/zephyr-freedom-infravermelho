#include "ultrassom.h"
#include <zephyr/drivers/gpio.h>
#include "pwm_z42.h"

#define TPM_IRQ_LINE TPM1_IRQn
#define TPM_IRQ_PRIORITY 1
#define TPM_MODULE 22734
#define PULSO_TICKS 4

// Variáveis internas da biblioteca
static volatile uint16_t t_atual = 0;
static volatile uint16_t t_anterior = 0;
static volatile uint16_t intervalo_ticks = 0;

// ISR Interna
void tpm1_isr(void *arg)
{
    TPM1->STATUS |= TPM_STATUS_CH0F_MASK; 
    t_anterior = t_atual;
    t_atual = TPM1->CONTROLS[0].CnV;
    intervalo_ticks = t_atual - t_anterior;
}

void ultrassom_init(void)
{
    // Conecta a interrupção
    IRQ_CONNECT(TPM_IRQ_LINE, TPM_IRQ_PRIORITY, tpm1_isr, NULL, 0);
    irq_enable(TPM_IRQ_LINE);

    // Configuração do Trigger (TPM0, Ch 3, Pino D3)
    pwm_tpm_Init(TPM0, TPM_PLLFLL, TPM_MODULE, TPM_CLK, PS_128, EDGE_PWM);
    pwm_tpm_Ch_Init(TPM0, 3, TPM_PWM_H, GPIOD, 3);
    pwm_tpm_CnV(TPM0, 3, PULSO_TICKS);

    // Configuração do Echo/Capture (TPM1, Ch 0, Pino A12)
    pwm_tpm_Init(TPM1, TPM_PLLFLL, 65535, TPM_CLK, PS_128, EDGE_PWM);
    pwm_tpm_Ch_Init(TPM1, 0, TPM_INPUT_CAPTURE_BOTH | TPM_CHANNEL_INTERRUPT, GPIOA, 12);
}

double ultrassom_get_largura_us(void)
{
    // (ticks * 1.000.000) / 48.500.000
    return (double)(intervalo_ticks * 1000000.0) / (48500000.0/128.0);
}

double ultrassom_get_distancia(void)
{
    double largura_us = ultrassom_get_largura_us();
    return (largura_us * 0.0343) / 2.0;
}