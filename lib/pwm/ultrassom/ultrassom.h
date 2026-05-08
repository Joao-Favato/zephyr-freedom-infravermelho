#ifndef ULTRASSOM_Z42_H
#define ULTRASSOM_Z42_H

#include <zephyr/kernel.h>

/**
 * @brief Inicializa o sensor ultrassônico usando os periféricos TPM.
 */
void ultrassom_init(void);

/**
 * @brief Obtém a última distância lida em centímetros.
 * @return Distância em cm.
 */
double ultrassom_get_distancia(void);

/**
 * @brief Obtém o tempo da última captura em microssegundos.
 * @return Tempo em us.
 */
double ultrassom_get_largura_us(void);

#endif