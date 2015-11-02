#define pinoAmpTerra A0 // pino do Amplificador de Sinal relacionado a resistencia da Terra
#define pinoBomba 2 // pino da Bomba
#define tempoLeitura 600000 // 10 minutos
#define tempoLoop 10 // 10 milisegundos (Tempo para disparo do loop)

int tempoBombaLigada; // Vari avel que indica o tempo em que a bomba ficara ligada
int loopsAteLeitura; //Variavel contador de loops para executar leitura
int contadorLoops = 0; // Variavel para contar quantos loops foram executados

char estadoPrograma = 'a'; // Booleana que diz em qual estado esta o programa
                        // Caso a = Aguardando o tempo para a leitura
                        // Caso b = Aguardando o termino da bomba

void setup()
{
  Serial.begin(9600);
  pinMode(pinoAmpTerra, INPUT);
  pinMode(pinoBomba, OUTPUT);
  
  loopsAteLeitura = tempoLeitura/tempoLoop; // Calculo de quantos loops serao necessarios para executar leitura
}

void loop()
{
  
  if (estadoPrograma == 'a' && contadorLoops++ >= loopsAteLeitura)
  {
    contadorLoops = 0;
    executarLeitura();
    estadoPrograma = 'b';
  }
  else if (estadoPrograma == 'b' && contadorLoops++ <= tempoBombaLigada)
  {
    if (contadorLoops == 0)
    {
      digitalWrite(pinoBomba, HIGH); //bomba ligada
    }
    else if (contadorLoops == tempoBombaLigada)
    {
      digitalWrite(pinoBomba, LOW); //bomba desligada
      estadoPrograma = 'a';
    }
  }
}

void executarLeitura()
{
  int sinalAmpTerra = 0; //Valor recebido pelo amplificador de sinal da resistencia da terra
  sinalAmpTerra = analogRead(pinoAmpTerra);
  matrizValores(sinalAmpTerra);
}  

void matrizValores(int sinalAmpTerra)
{
  int verificador;
  tempoBombaLigada = 0;
  int contadorMLinha = 0;
  // X = Faixa Inicio
  // Y = Faixa Fim
  // Z = Tempo de execucao da bomba
  
  //                            X   Y   Z
 int matrizFaixaTempo[][3] = { {0, 256, 0}, {257, 512, 0}, {513, 768, 1}, {514, 1024, 2} };
 
 do
 {
    for (int contadorMColuna = 0; contadorMColuna <= 1; contadorMColuna++)
      {
       // matrizFaixaTempo[contadorMLinha][contadorMColuna];
        if (contadorMColuna == 0 && sinalAmpTerra >= matrizFaixaTempo[contadorMLinha][contadorMColuna])
        {
          verificador = 0;
        }
        if (contadorMColuna == 1 && sinalAmpTerra <= matrizFaixaTempo[contadorMLinha][contadorMColuna])
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
