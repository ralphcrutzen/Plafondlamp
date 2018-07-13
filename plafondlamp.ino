int ledR = D1;
int ledG = D2;
int ledB = D3;

int currentR = 0;
int currentG = 0;
int currentB = 0;
int colorWheelPos = 0;

const int discoColors[][3] = {
      {255, 0, 0},   {0, 255, 0},   {0, 0, 255},
      {255, 255, 0}, {255, 0, 255}, {0, 255, 255},

      {255, 15, 0}, {255, 0, 15}, {255, 15, 15},
      {15, 255, 0}, {0, 255, 15}, {15, 255, 15},
      {15, 0, 255}, {0, 15, 255}, {15, 15, 255},

      {255, 127, 0}, {255, 0, 127}, {255, 127, 127},
      {127, 255, 0}, {0, 255, 127}, {127, 255, 127},
      {127, 0, 255}, {0, 127, 255}, {127, 127, 255},

      {255, 240, 0}, {255, 0, 240}, {255, 240, 240},
      {240, 255, 0}, {0, 255, 240}, {240, 255, 240},
      {240, 0, 255}, {0, 240, 255}, {240, 240, 255}
    };
const int discoDuration[] = {125, 250, 375, 500};

#define MODE_RAINBOW  0
#define MODE_SELECT   1
#define MODE_SLEEP    2
#define MODE_DISCO    3
int mode = MODE_SELECT;

bool alarm = false;
int alarmH = 0;
int alarmM = 0;

bool successColor  = Particle.function("sendcolor", sendColor);
bool successMode   = Particle.function("sendmode", sendMode);
bool successAlarm  = Particle.function("sendalarm", sendAlarm);
bool successHour   = Particle.function("sendhour", sendHour);
bool successMinute = Particle.function("sendminute", sendMinute);

void setup()
{
    delay(5000); // Allow board to settle; seems to prevent breathing green led
    pinMode(ledR, OUTPUT);
    pinMode(ledG, OUTPUT);
    pinMode(ledB, OUTPUT);
    Time.zone(1); // UTC+1
    setColor(currentR , currentG , currentB);
}

void loop()
{
    if (alarm && alarmH == Time.hour() && alarmM == Time.minute()) {
      modeWakeUp();
    }
    else {
      if (mode == MODE_RAINBOW) {
         modeRainbow();
      }
      else if (mode == MODE_SELECT) {
        modeSelect();
      }
      else if (mode == MODE_SLEEP) {
        modeGoToSleep();
      }
      else if (mode == MODE_DISCO) {
        modeDisco();
      }
    }
}

void modeRainbow()
{
  setColorWheel(colorWheelPos);
  colorWheelPos = (colorWheelPos + 1) % 256;
  delay(100);
}

void modeSelect()
{
    setColor(currentR, currentG, currentB);
}

void modeWakeUp()
{
  Particle.publish("Wake up!");
  int wait = 5000;
  for (int r = 1; (r < 64) && alarm; r++) {
      setColor(r, 0, 0);
      delay(wait);
  }
  for (int g = 1; (g < 32) && alarm; g++) {
      setColor(63, g, 0);
      delay(wait);
  }
  for (int b = 1; (b < 32) && alarm; b++) {
      setColor(63, 31, b);
      delay(wait);
  }
  mode = MODE_RAINBOW;
}

void modeGoToSleep()
{
  Particle.publish("Going to sleep...");
  int duration = 30; // minutes
  int b = 128; // brightness
  while (b >= 0 && mode == MODE_SLEEP) {
    setColor(0, b, (int)b/2);
    int s = 0;
    // check every second if we're still in sleep mode
    while (s <= duration*60/128 && mode == MODE_SLEEP) { // seconds per step
      delay(1000);
      s++;
    }
    b--;
  }
  if (mode == MODE_SLEEP) {
    currentR = 0;
    currentG = 0;
    currentB = 0;
    mode = MODE_SELECT;
  }
}

void modeDisco()
{
    Particle.publish("Disco!");
    while (mode == MODE_DISCO) {
      int c = random(33);
      setColor(discoColors[c][0], discoColors[c][1], discoColors[c][2]);
      delay(discoDuration[random(4)]);
    }
}

int sendColor(String hexColor)
{
    long long number = strtoll( &hexColor[1], NULL, 16);
    long long r = number >> 16;
    long long g = number >> 8 & 0xFF;
    long long b = number & 0xFF;
    setColor(r, g, b);
    currentR = r;
    currentG = g;
    currentB = b;
    Particle.publish("Color", hexColor);
}

int sendMode(String m) {
  Particle.publish("Color mode", m);
  if (m == "rainbow") {
    mode = MODE_RAINBOW;
  }
  else if (m == "select") {
    mode = MODE_SELECT;
  }
  else if (m == "sleep") {
    mode = MODE_SLEEP;
  }
  else if (m == "disco") {
    mode = MODE_DISCO;
  }
}

int sendAlarm(String al) {
  alarm = (al == "on");
  Particle.publish("Alarm", al);
}

int sendHour(String hour) {
  alarmH = hour.toInt();
  Particle.publish("Alarm hour", hour);
}

int sendMinute(String minute) {
  alarmM = minute.toInt();
  Particle.publish("Alarm minute", minute);
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
void setColorWheel(byte wheelPos) {
  if (wheelPos < 85) {
    setColor(wheelPos*3, 255 - wheelPos*3, 0);
  }
  else if (wheelPos < 170) {
    wheelPos -= 85;
    setColor(255 - wheelPos*3, 0, wheelPos*3);
  }
  else {
    wheelPos -= 170;
    setColor(0, wheelPos*3, 255 - wheelPos*3);
  }
}

void setColor(int r, int g, int b)
{
    currentR = r;
    currentG = g;
    currentB = b;
    analogWrite(ledR, (int)r);
    analogWrite(ledG, (int)g);
    analogWrite(ledB, (int)b);
}
