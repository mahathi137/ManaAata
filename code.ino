// =====================================================
// ESP32 + 74HC4067 + 11 A3144 HALL SENSORS + BUZZER
// Channels: C0-C8, C10, C15
// =====================================================

#define S0 4
#define S1 5
#define S2 16
#define S3 17
#define SIG 18

#define BUZZER 25

// Your exact channels
const int hallChannels[11] = {
  0, 1, 2, 3, 4,
  5, 6, 7, 8,
  10, 15
};

// Stores previous state of each Hall sensor
int previousState[11] = {
  HIGH, HIGH, HIGH, HIGH, HIGH,
  HIGH, HIGH, HIGH, HIGH,
  HIGH, HIGH
};

void selectChannel(int channel)
{
  digitalWrite(S0, (channel >> 0) & 1);
  digitalWrite(S1, (channel >> 1) & 1);
  digitalWrite(S2, (channel >> 2) & 1);
  digitalWrite(S3, (channel >> 3) & 1);

  // Give multiplexer time to settle
  delayMicroseconds(500);
}

void beep()
{
  digitalWrite(BUZZER, HIGH);
  delay(100);
  digitalWrite(BUZZER, LOW);
}

void setup()
{
  Serial.begin(115200);

  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);

  // Hall sensor signal
  pinMode(SIG, INPUT_PULLUP);

  // Buzzer
  pinMode(BUZZER, OUTPUT);
  digitalWrite(BUZZER, LOW);

  Serial.println();
  Serial.println("======================================");
  Serial.println("   HERITAGE CORE HALL TEST");
  Serial.println("   ESP32 + 74HC4067 + 11 HALL");
  Serial.println("   BUZZER ENABLED");
  Serial.println("======================================");
  Serial.println();

  delay(1000);
}

void loop()
{
  for (int i = 0; i < 11; i++)
  {
    int channel = hallChannels[i];

    selectChannel(channel);

    // Read several times for stability
    int reading1 = digitalRead(SIG);
    delayMicroseconds(100);
    int reading2 = digitalRead(SIG);
    delayMicroseconds(100);
    int reading3 = digitalRead(SIG);

    // Majority decision
    int state;

    if (reading1 + reading2 + reading3 >= 2)
      state = HIGH;
    else
      state = LOW;

    Serial.print("H");
    Serial.print(i + 1);

    Serial.print(" | C");
    Serial.print(channel);

    Serial.print(" | RAW: ");
    Serial.print(reading1);
    Serial.print(reading2);
    Serial.print(reading3);

    Serial.print(" | ");

    if (state == LOW)
    {
      Serial.println("MAGNET DETECTED");

      // Beep only when magnet is newly detected
      if (previousState[i] == HIGH)
      {
        beep();
      }
    }
    else
    {
      Serial.println("NO MAGNET");
    }

    // Save current state
    previousState[i] = state;
  }

  Serial.println("--------------------------------------");

  delay(100);
}
