int ledR = D1;
int ledG = D2;
int ledB = D3;

void setup() {
    Particle.function("sendcolor", setColor);
    pinMode(ledR, OUTPUT);
    pinMode(ledG, OUTPUT);
    pinMode(ledB, OUTPUT);
    setColor("0");
}

void loop() {
}

int setColor(String hexColor) {
    long long number = strtoll( &hexColor[1], NULL, 16);
    long long r = number >> 16;
    long long g = number >> 8 & 0xFF;
    long long b = number & 0xFF;
    analogWrite(ledR, r);
    analogWrite(ledG, g);
    analogWrite(ledB, b);
}
