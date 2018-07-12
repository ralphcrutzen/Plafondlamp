int ledR = D1;
int ledG = D2;
int ledB = D3;

void setup() {
    pinMode(ledR, OUTPUT);
    pinMode(ledG, OUTPUT);
    pinMode(ledB, OUTPUT);
}

void loop() {
    setColor(255, 0, 0);
    delay(500);
    setColor(0, 255, 0);
    delay(500);
    setColor(0, 0, 255);
    delay(500);
}

void setColor(int r, int g, int b) {
    analogWrite(ledR, r);
    analogWrite(ledG, g);
    analogWrite(ledB, b);
}
