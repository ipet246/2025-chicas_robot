// Pines Motores (L298N)
#define IN1 6
#define IN2 7
#define IN3 4
#define IN4 5
#define ENA 9   // PWM motor derecho
#define ENB 10  // PWM motor izquierdo

// Pines Ultrasonido
#define TRIG A0
#define ECHO A2

// Configuración
int velocidad = 200;   // 0-255
int distanciaMin = 25; // cm

// ---------------- FUNCIONES ----------------

// Función medir distancia
long medirDistancia() {
  digitalWrite(TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);

  long duracion = pulseIn(ECHO, HIGH, 20000); // timeout 20ms
  long distancia = duracion * 0.034 / 2; // cm
  if (distancia == 0) distancia = 999;   // por si no detecta nada
  return distancia;
}

// Movimiento hacia adelante
void adelante() {
  analogWrite(ENA, velocidad);
  analogWrite(ENB, velocidad);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

// Movimiento hacia atrás
void atras() {
  analogWrite(ENA, velocidad);
  analogWrite(ENB, velocidad);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

// Girar izquierda
void izquierda() {
  analogWrite(ENA, velocidad);
  analogWrite(ENB, velocidad);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

// Girar derecha
void derecha() {
  analogWrite(ENA, velocidad);
  analogWrite(ENB, velocidad);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}

// Parar
void parar() {
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}

// ---------------- SETUP ----------------
void setup() {
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);

  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);

  Serial.begin(9600);
}

// ---------------- LOOP ----------------
void loop() {
  long d = medirDistancia();
  Serial.print("Distancia: ");
  Serial.print(d);
  Serial.println(" cm");

  if (d > distanciaMin) {
    adelante();
  } else {
    parar();
    delay(200);

    // Gira hasta que encuentre camino libre
    do {
      atras();
      delay(300);
      derecha();       // gira a la derecha siempre
      delay(300);
      parar();
      delay(100);
      d = medirDistancia();
    } while (d <= distanciaMin);

  }
}