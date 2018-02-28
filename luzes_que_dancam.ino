#include <TimerOne.h>   // inclui biblioteca Timer1
 
#define triacApin 5 // Define que o Dimmer será comandado pelo pino 5

int frequencia = 60;
int stateTriacA = 0;
int power = 0;  //inicializa variavel que controla potencia na lampada com 0 (lampada apagada)

long sensor = 100;

const int trigPin = 3;
const int echoPin = 4;

long microsecondsToInches(long microseconds)
{
  // According to Parallax's datasheet for the PING))), there are
  // 73.746 microseconds per inch (i.e. sound travels at 1130 feet per
  // second).  This gives the distance travelled by the ping, outbound
  // and return, so we divide by 2 to get the distance of the obstacle.
  // See: http://www.parallax.com/dl/docs/prod/acc/28015-PING-v1.3.pdf
  return microseconds / 74 / 2;
}

long microsecondsToCentimeters(long microseconds)
{
  // The speed of sound is 340 m/s or 29 microseconds per centimeter.
  // The ping travels out and back, so to find the distance of the
  // object we take half of the distance travelled.
  return microseconds / 29 / 2;
}

void gateTRIAC() {  // trata interrupcao do TIMER1 gerando pulso no gate do TRIAC
  digitalWrite(triacApin, HIGH);  // dispara o Triac
  delayMicroseconds(5);  // aguarda 5 microsegundos para garantir disparo do TRIAC
  digitalWrite(triacApin, LOW);   // desabibilta gate do TRIAC
  Timer1.stop();
}

void zero_cross_detect() {
  if (power > 0) {
    long dimtime = int(map(power,0,100,7950,150));  // calcula o tempo de delay para o disparo do TRIAC 7950,100
    Timer1.attachInterrupt(gateTRIAC, dimtime);  //associa a funcao gateTRIAC com Interrupcao do TIMER1
    Timer1.start();  // inicia contagem TIMER1
  } else {
    digitalWrite(triacApin, LOW);   // mantem gate do TRIAC desativado.
    Timer1.stop();
  }
}

int Concatena_Numeros(int variavel1, int variavel2) {
  int resultado;
  resultado = ((variavel1-'0')*10)+(variavel2-'0');
  return resultado;
}

void setup() {
  Serial.begin(9600);
  pinMode(triacApin, OUTPUT);
  digitalWrite(triacApin, LOW);
  //Serial.println("Controle de TRIAC.\r\nDigite o Percentual de Brilho (duas casas) Ex.: 25");
  // Initialize the interruption time Timer1
  Timer1.initialize(); // Initialize TimerOne library for the freq we need
  // Attach the interrupt 0 to pin 2 for the detection of zero crossing (Zero Cross Detection)
  attachInterrupt(0, zero_cross_detect, FALLING); // Attach an Interrupt to Pin 2 (Interrupt 0) for Zero Cross Detection
}
 
void loop() {
  
  long duration, inches, cm;
  
  pinMode(trigPin, OUTPUT);
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  pinMode(echoPin, INPUT);
  duration = pulseIn(echoPin, HIGH);

  cm = microsecondsToCentimeters(duration);

  // debug
  //Serial.print(inches);
  //Serial.print("in, ");
  //Serial.print(cm);
  //Serial.print("cm");
  //Serial.println();

  if (cm < sensor) {
    sensor = cm;
  } else {
    sensor = sensor + 4;
  }

  //Serial.print("sensor: ");
  //Serial.println(sensor);
    
  //debug
  //int i = 0;
  //int read_data[2];
  //while (i < 2) {
    //while (Serial.available()) {
      //read_data[i] = Serial.read();
      //i++;
    //}
   //}
  //power = Concatena_Numeros(read_data[0],read_data[1]);
  //Serial.println(power);

  sensor = constrain(sensor, 12, 100);
  power = map(sensor, 12, 100, 99, 00);
  //Serial.print("power: ");
  //Serial.println(power);

  delay(100);
}
