
#include "conversion.h"

#define CALIB_PENDING 2  // Led Vermelho
#define CALIB_DONE 8      // Led Verde
#define CALIB_COV 13       // Led Amarelo - Calibração Matrizes de covariância

// Classe para conversão da biblioteca do MPU
mpu_conv_class mpu_new(CALIB_PENDING, CALIB_DONE);

void setup() {
  Serial.begin(9600); // Inicializa a porta serial

  /**** MPU ****/
  // Inicializa a calibração da MPU

  if (mpu_new.config_mpu()) {
    // Se a calibração foi bem sucedida
    digitalWrite(CALIB_PENDING, LOW);
    digitalWrite(CALIB_DONE, HIGH);
  } else {
    // Calibração mal sucedida
    digitalWrite(CALIB_PENDING, LOW);
    digitalWrite(CALIB_DONE, LOW);
    while (1); // trava o programa caso a calibração não funcione e desliga os LEDs
  }
}

void loop() {
  delay(100); 
  float acc_N_Filtered = 0;
  float acc_E_Filtered = 0;
  char counterFilter = 0;
  float acc_N_Array[10] = { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 };
  float acc_E_Array[10] = { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 };
  
  // Leitura da MPU
  // Faz a aferição dos sensores
  float acc_N, acc_E;
  if (mpu_new.update_data()) {
    mpu_new.make_conversion(&acc_N, &acc_E); // Faz a leitura dos acelerometro e a conversão das coordenadas
    acc_N_Array[counterFilter] = acc_N;
    acc_E_Array[counterFilter] = acc_E;

    for (char i = 0; i < 10; i++) {
      acc_N_Filtered += acc_N_Array[i];
      acc_E_Filtered += acc_E_Array[i];
    }
    float resultACC_N = acc_N_Filtered / 10;
    float resultACC_E = acc_E_Filtered / 10;

    Serial.print(resultACC_N);
    Serial.print(" ");
    Serial.print(resultACC_E);
    Serial.println(" ");
  }
}
