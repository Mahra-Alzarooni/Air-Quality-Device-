
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>

#define DHTPIN 2
#define DHTTYPE DHT11
#define AIR_QUALITY_PIN A0
#define BUZZER_PIN 8
#define GREEN_LED_PIN 4
#define YELLOW_LED_PIN 5
#define RED_LED_PIN 6

DHT dht(DHTPIN, DHTTYPE);
LiquidCrystal_I2C lcd(0x27, 16, 2); // I2C address 0x27, 16 columns, 2 rows

bool buzzerActive = false;
unsigned long buzzerOffTime = 0;
unsigned long buzzerDelay = 30000;

void setup() {
    Serial.begin(9600);
    lcd.init();
    lcd.backlight();
    dht.begin();
    pinMode(AIR_QUALITY_PIN, INPUT);
    pinMode(BUZZER_PIN, OUTPUT);
    pinMode(GREEN_LED_PIN, OUTPUT);
    pinMode(YELLOW_LED_PIN, OUTPUT);
    pinMode(RED_LED_PIN, OUTPUT);

    lcd.setCursor(0, 0);
    lcd.print("Air Quality:");
    delay(2000);
}

void loop() {
    float temp = dht.readTemperature();
    float humidity = dht.readHumidity();

    if (isnan(temp) || isnan(humidity)) {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("DHT11 Error");
        delay(2000);
        return;
    }

    int air_quality_raw = analogRead(AIR_QUALITY_PIN);

    String airQualityStatus;
    if (air_quality_raw < 100) {
        airQualityStatus = "Good";
        digitalWrite(GREEN_LED_PIN, HIGH);
        digitalWrite(YELLOW_LED_PIN, LOW);
        digitalWrite(RED_LED_PIN, LOW);
        noTone(BUZZER_PIN);
        buzzerActive = false;
    } else if (air_quality_raw < 300) {
        airQualityStatus = "Moderate";
        digitalWrite(YELLOW_LED_PIN, HIGH);
        digitalWrite(GREEN_LED_PIN, LOW);
        digitalWrite(RED_LED_PIN, LOW);
        noTone(BUZZER_PIN);
        buzzerActive = false;
    } else {
        airQualityStatus = "Bad";
        digitalWrite(RED_LED_PIN, HIGH);
        digitalWrite(GREEN_LED_PIN, LOW);
        digitalWrite(YELLOW_LED_PIN, LOW);
        if (!buzzerActive) {
            tone(BUZZER_PIN, 1000);
            buzzerActive = true;
            buzzerOffTime = millis();
        }
    }

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Air: " + airQualityStatus);
    lcd.setCursor(0, 1);
    lcd.print("H:");
    lcd.print(humidity);
    lcd.print("% ");
    lcd.setCursor(9, 1);
    lcd.print("T:");
    lcd.print(temp);
    lcd.print("C");

    delay(2000);
}
