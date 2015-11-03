#define PINO_ADC_SENSOR_TERRA A0 // pino do Amplificador de Sinal relacionado a resistencia da Terra
#define PINO_BOMBA 3 // pino da Bomba
#define TEMPO_LEITURA_SENSOR_TERRA 30000 // 1 minuto
//#define tempoLeitura 600000 // 10 minutos
#define tempoLoop 10 // 10 milisegundos (Tempo para disparo do loop)

long tempoBombaLigada = 0; // Vari avel que indica o tempo em que a bomba ficara ligada (milisegundos)
int loopsAteLeitura = 0; //Variavel contador de loops para executar leitura
long contadorLoops = 0; // Variavel para contar quantos loops foram executados

long horaAgora = 0;
long horaUltimaLeitura = 0;
long horaBombaLigada = 0;

enum ESTADO_PROGRAMA{
  ESTADO_AGUARDANDO_LEITURA_SENSOR, //Aguardando o tempo para a leitura
  ESTADO_AGUARDANDO_BOMBA_LIGADA    //Aguardando o termino da bomba
};

enum ESTADO_PROGRAMA estadoPrograma;

void setup()
{
  Serial.begin(9600);
  Serial.print("\nO Winston esta sendo inicializado...");
  
  pinMode(PINO_ADC_SENSOR_TERRA, INPUT);
  pinMode(PINO_BOMBA, OUTPUT);

  // Define estado inicial
  estadoPrograma = ESTADO_AGUARDANDO_LEITURA_SENSOR;


  analogWrite(PINO_BOMBA, 0);
  // Obtém o time atual
  horaAgora = millis();

  Serial.print("\nConfiguracao do sistema concluida!");
}

void loop()
{
  // Obtém o time atual
  horaAgora = millis();

  // Operações enquanto esta aguardando tempo para ler o sensor
  if (estadoPrograma == ESTADO_AGUARDANDO_LEITURA_SENSOR){

      // Verifica se já passou o tempo necessário desde a última leitura do sensor de umidade terra
      if( (horaAgora - horaUltimaLeitura) >= TEMPO_LEITURA_SENSOR_TERRA){

       int adcSensorTerra = leituraADCSensorTerra();

       long millisLigarBomba = tempoLigarBomba(adcSensorTerra);

        if(millisLigarBomba > 0){
            Serial.print("\nLigando bomba por ");
            Serial.print(millisLigarBomba/60000);
            Serial.print(" minutos.");
        
            analogWrite(PINO_BOMBA, 130);
            
            horaBombaLigada = millis();
            
            tempoBombaLigada = millisLigarBomba;
            
            estadoPrograma = ESTADO_AGUARDANDO_BOMBA_LIGADA;
        }else{
          Serial.print("\nNao precisa ligar essa porra.");
        }    
        
        horaUltimaLeitura = millis();        
      }
  
  }
  
  // Operações enquanto esta aguardando tempo para desligar bomba
  if (estadoPrograma == ESTADO_AGUARDANDO_BOMBA_LIGADA){
   
   if((millis() - horaBombaLigada) >= tempoBombaLigada){     
     
     Serial.print("\nDesligando bomba!");
     
     analogWrite(PINO_BOMBA, 0);
                 
     horaUltimaLeitura = millis(); 
     
     estadoPrograma = ESTADO_AGUARDANDO_LEITURA_SENSOR;
   }
    
  }
}

/*
 * Converte milisegundos para horas formatada no padrão HH:mm:ss
 */
String millisParaStringHora(long millisTempo) {
  // Calcula quantos segundos no tempo recebido
  long segundosTempo = millisTempo / 1000L;

  // Obtém a parte de horas
  int horas = (int)((segundosTempo / 60L) / 60L);

  //Retira a parte de horas
  segundosTempo -= (horas * 3600L);

  //Obtém a parte de minutos
  int minutos = (int)(segundosTempo / 60L);

  // Retira a parte de minutos
  segundosTempo -= minutos * 60L;

  //Obtém a parte de segundos
  int segundos = (int)segundosTempo;

  //Vetor para armazenar a hora formatada
  char charTempo[9];

  //Format a hora
  sprintf(charTempo, "%02d:%02d:%02d", (int)horas, (int)minutos, (int)segundos);

  return String(charTempo);
}

int leituraADCSensorTerra()
{
  // Leitura do ADC (Analogic to Digital Converter) do sensor de umidade da terra
  int adcSensorTerra = analogRead(PINO_ADC_SENSOR_TERRA  );

  Serial.print("\nValor lido do sensor de umidade: ");
  Serial.print(adcSensorTerra);
  
  return adcSensorTerra;
}  

long tempoLigarBomba(int adcSensorTerra)
{
  int verificador;
  tempoBombaLigada = 0;
  int contadorMLinha = 0;
  // X = Faixa Inicio
  // Y = Faixa Fim
  // Z = Tempo de execucao da bomba
  
  //                            X   Y   Z
 int matrizFaixaTempo[][3] = { {0, 256, 1}, {257, 512, 2}, {513, 768, 3}, {514, 1024, 4} };

 int matrizTamanho = sizeof(matrizFaixaTempo); 
 
 for(int indexVetor = 0; indexVetor <  matrizTamanho; indexVetor++){
 
   int* itemFaixaTempo = matrizFaixaTempo[indexVetor];
   
   int valorMinimo = itemFaixaTempo[0];
   int valorMaximo = itemFaixaTempo[1];
   
   if(adcSensorTerra >= valorMinimo && adcSensorTerra <= valorMaximo){    
     tempoBombaLigada = itemFaixaTempo[2] * 60000;     
     return tempoBombaLigada;   
   } 
   
 } 
 return 0;
}
