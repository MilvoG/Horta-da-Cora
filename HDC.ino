#define pinoAmpTerra A0 // pino do Amplificador de Sinal relacionado a resistencia da Terra
#define pinoBomba 2 // pino da Bomba
#define tempoLeitura 600000 // 10 minutos
#define tempoLoop 10 // 10 milisegundos (Tempo para disparo do loop)

int loopsAteLeitura; //Variavel contador de loops para executar leitura
int contadorLoops = 0; // Variavel para contar quantos loops foram executados

//int matrizLinha;

void setup()
{
  Serial.begin(9600);
  pinMode(pinoAmpTerra, INPUT);
  pinMode(pinoBomba, OUTPUT);
  
  loopsAteLeitura = tempoLeitura/tempoLoop; // Calculo de quantos loops serao necessarios para executar leitura
}

void loop()
{
  
  if (contadorLoops++ >= loopsAteLeitura)
  {
    contadorLoops = 0;
    executarLeitura();
  }
}

void executarLeitura()

void matrizValores()
{
  // X = Faixa Inicio
  // Y = Faixa Fim
  // Z = Tempo de execucao da bomba
  
  //                          X   Y   Z     X    Y   Z     X    Y   Z     X     Y   Z
  matrizFaixaTempo[3][2] = { {0, 256, 1}, {257, 512, 2}, {513, 768, 3}, {514, 1024, 4} };
}


/*
{
  int sinalAmpTerra = 0; //Valor recebido pelo amplificador de sinal da resistencia da terra
  
  sinalAmpTerra = analogRead(pinoAmpTerra);
  if (sinalAmpTerra >= 768 && sinalAmpTerra <= 1024)
  {
    digitalWrite(pinoBomba, HIGH);
    //DEFINIR DELAY 100%
  }
  else if (sinalAmpTerra >= 512 && sinalAmpTerra <= 767)
  {
    digitalWrite(pinoBomba, HIGH);
    //DEFINIR DELAY 75%
  }
  else if (sinalAmpTerra >= 256 && sinalAmpTerra <= 511)
  {
    digitalWrite(pinoBomba, HIGH);
    //DEFINIR DELAY 50%
  }
  else
  {
    digitalWrite(pinoBomba, HIGH);
    //DEFINIR DELAY 25%
  }
  
}
*/  
 
