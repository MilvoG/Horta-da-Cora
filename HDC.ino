#define pinoAmpTerra A0 // pino do Amplificador de Sinal relacionado a resistencia da Terra
#define pinoBomba 2 // pino da Bomba
#define TEMPO_LEITURA_SENSOR_TERRA 30000 // 1 minuto
//#define tempoLeitura 600000 // 10 minutos
#define tempoLoop 10 // 10 milisegundos (Tempo para disparo do loop)

int tempoBombaLigada = 0; // Vari avel que indica o tempo em que a bomba ficara ligada
int loopsAteLeitura = 0; //Variavel contador de loops para executar leitura
long contadorLoops = 0; // Variavel para contar quantos loops foram executados

long horaAgora = 0;
long horaUltimaLeitura = 0;

enum ESTADO_PROGRAMA{
  ESTADO_AGUARDANDO_LEITURA_SENSOR, //Aguardando o tempo para a leitura
  ESTADO_AGUARDANDO_BOMBA_LIGADA    //Aguardando o termino da bomba
};

enum ESTADO_PROGRAMA estadoPrograma;

void setup()
{
  Serial.begin(9600);
  Serial.print("\nO Winston esta sendo inicializado...");
  
  pinMode(pinoAmpTerra, INPUT);
  pinMode(pinoBomba, OUTPUT);

  // Calculo de quantos loops serao necessarios para executar leitura
  //loopsAteLeitura = tempoLeitura/tempoLoop;

  // Define estado inicial
  estadoPrograma = ESTADO_AGUARDANDO_LEITURA_SENSOR;

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

      // Verifica se já passou o tempo necessário desde a última leitura do sensor
      if( (horaAgora - horaUltimaLeitura) >= TEMPO_LEITURA_SENSOR_TERRA){

        executarLeitura();
        
        horaUltimaLeitura = millis();

        //Serial.print("\nUltima leitura do sensor de umidade as ");
        //Serial.print(millisParaStringHora(loopsAteLeitura));
      }
  
  }

  /*
  if (estadoPrograma == ESTADO_AGUARDANDO_LEITURA_SENSOR && contadorLoops++ >= loopsAteLeitura)
  {
    Serial.println("Estado de leitura.");
    contadorLoops = 0;
    executarLeitura();
    estadoPrograma = ESTADO_AGUARDANDO_BOMBA_LIGADA;
  }
  if (estadoPrograma == ESTADO_AGUARDANDO_BOMBA_LIGADA && contadorLoops++ <= tempoBombaLigada)
  {
    int contadorB = 0;
    Serial.print("O tempo da bomba ligada corresponde a: ");
    Serial.println(tempoBombaLigada);
    Serial.println(contadorLoops);
    if (contadorB == 0)
    {
      Serial.println("Bomba ligada.");
      digitalWrite(pinoBomba, HIGH); //bomba ligada
    }
    if (contadorLoops == tempoBombaLigada)
    {
      Serial.println("Bomba desligada.");
      digitalWrite(pinoBomba, LOW); //bomba desligada
      estadoPrograma = ESTADO_AGUARDANDO_LEITURA_SENSOR;
    }
    contadorB = 1;
  }*/
}

char* millisParaStringHora(long millisHora){
  
    int horas = ((millisHora / 1000) / 60) / 60;    

    //Retira a parte de horas
    millisHora = millisHora - (horas * 3600000);
    
    int minutos = (millisHora / 1000) / 60;

    // Retira a parte de minutos
    millisHora = millisHora - (minutos * 60000);
    
    int segundos = millisHora / 1000;

    char hora[8];

    sprintf(hora, "%02d:%02d:%02d", horas, minutos, segundos);
    
    return hora;
}

void executarLeitura()
{
  // Leitura do ADC (Analogic to Digital Converter) do sensor de umidade da terra
  int adcSensorTerra = analogRead(pinoAmpTerra);

  Serial.print("\nValor lido do sensor de umidade: ");
  Serial.print(adcSensorTerra);
  
 //matrizValores (adcSensorTerra);
}  

void matrizValores(int adcSensorTerra)
{
  int verificador;
  tempoBombaLigada = 0;
  int contadorMLinha = 0;
  // X = Faixa Inicio
  // Y = Faixa Fim
  // Z = Tempo de execucao da bomba
  
  //                            X   Y   Z
 int matrizFaixaTempo[][3] = { {0, 256, 0}, {257, 512, 0}, {513, 768, 1}, {514, 800, 2} };

int matrizTamanho = sizeof(matrizFaixaTempo);
 
 do
 {
    for (int contadorMColuna = 0; contadorMColuna <= 1; contadorMColuna++)
      {
       // matrizFaixaTempo[contadorMLinha][contadorMColuna];
        if (contadorMColuna == 0 && adcSensorTerra >= matrizFaixaTempo[contadorMLinha][contadorMColuna])
        {
          verificador = 0;
        }
        if (contadorMColuna == 1 && adcSensorTerra <= matrizFaixaTempo[contadorMLinha][contadorMColuna])
        {
          verificador = 1;
          switch (contadorMLinha)
          {
            case 0:
              tempoBombaLigada = matrizFaixaTempo[0][2]*60000;
              break;
            case 1:
              tempoBombaLigada = matrizFaixaTempo[1][2]*60000;
              break;
            case 2:
              tempoBombaLigada = matrizFaixaTempo[2][2]*60000;
              break;
            case 3:
              tempoBombaLigada = matrizFaixaTempo[3][2]*60000;
              break;
          }
        }
      }
    contadorMLinha++;
 } 
 while(verificador != 1);
}
