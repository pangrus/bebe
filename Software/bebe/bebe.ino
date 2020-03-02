////////////////////
//  bebe_brain     //
//                 //
//  square lfo     //
//  gate sequencer //
//                 //
//  pangrus 2019   //
/////////////////////


boolean gate1  = LOW;
boolean gate2 = LOW;
byte colpi1;
byte colpi2;
byte clockOut = 6;
byte out1 = 5;
byte out2 = 4;
unsigned char stepCorrente = 0;
char pattern0[16];
char pattern1[16];
int potenziometro1;
int potenziometro2;
int modeSwitch;
int pausa;
long ora;
long prima0 = 0;
long prima1 = 0;
long tempoGate1;
long tempoGate2;


void setup() {
  pinMode (clockOut, OUTPUT);
  pinMode (out1, OUTPUT);
  pinMode (out2, OUTPUT);
  digitalWrite (clockOut, LOW);
  digitalWrite (out1, HIGH);
  digitalWrite (out2, HIGH);
}


void EuclideoInverso(char *pattern, byte colpi, byte lunghezzaPattern) {
  int cur = lunghezzaPattern;
  for (int i = 0; i < lunghezzaPattern; i++) {
    *pattern = '.';
    if (cur >= lunghezzaPattern) {
      cur -= lunghezzaPattern;
      *pattern = 'x';
    }
    if (colpi == 0) *pattern = '.';
    pattern++;
    cur += colpi;
  }
}


void loop() {
  potenziometro1 = analogRead(A1);
  potenziometro2 = analogRead(A0);
  modeSwitch = analogRead (A2);

  // low frequency to audio oscillator mode
  if (modeSwitch > 900) {
    stepCorrente = 0;
    ora = micros();

    if (potenziometro1 <= 10) digitalWrite (out1, LOW);
    if (potenziometro1 >= 1010) digitalWrite (out1, HIGH);
    if (potenziometro1 > 10 & potenziometro1 < 1010) {
      tempoGate1 = map(potenziometro1, 11, 1009, 150000, 3000);
      if (ora - prima0 > tempoGate1) {
        prima0 = ora;
        gate1 = !gate1;
        digitalWrite (out1, gate1);
      }
    }

    if (potenziometro2 <= 10) digitalWrite (out2, LOW);
    if (potenziometro2 >= 1010) digitalWrite (out2, HIGH);
    if (potenziometro2 > 10 & potenziometro2 < 1010) {
      tempoGate2 = map(potenziometro2, 11, 1009, 150000, 3000);
      if (ora - prima1 > tempoGate2) {
        prima1 = ora;
        gate2 = !gate2;
        digitalWrite (out2, gate2);
      }
    }
  }

  // off mode
  if (modeSwitch > 400 & modeSwitch < 600 ) {
    stepCorrente = 0;
    digitalWrite (out1, LOW);
    digitalWrite (out2, LOW);
  }

  // euclidean sequencer mode
  if (modeSwitch < 100 ) {
    pausa = 1027 - potenziometro1;

    if (potenziometro2 > 1000) {
      colpi1 = random (5) + 1;
      colpi2 = random (5) + 1;
    }

    if (potenziometro2 <= 1000)  {
      colpi1 = potenziometro2 / 200 + 1;
      colpi2 = ((potenziometro2 - ((colpi1 - 1) * 200)) / 40) + 1;
    }

    EuclideoInverso((char *)&pattern0, colpi1, 5);
    EuclideoInverso((char *)&pattern1, colpi2, 5);

    digitalWrite (clockOut, HIGH);

    if (pattern0[stepCorrente] == 'x') digitalWrite (out1, HIGH);
    if (pattern1[stepCorrente] == 'x') digitalWrite (out2, HIGH);

    delay (pausa);
    digitalWrite (clockOut, LOW);
    digitalWrite (out1, LOW);
    digitalWrite (out2, LOW);
    delay (pausa);

    if (++stepCorrente == 5) stepCorrente = 0;
  }
}
