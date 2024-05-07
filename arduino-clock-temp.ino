#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DHT11.h>
#include <I2C_RTC.h>

LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display

DHT11 dht11(3);

static DS1307 RTC;

unsigned int hours = 0;
unsigned int minutes = 0;
unsigned int seconds = 0;
unsigned int day = 0;
unsigned int month = 0;

int temperature, humidity, tMin, tMax, hMin, hMax = 0;

void setup() {
    lcd.init();                      // initialize the lcd
    lcd.backlight();
    dht11.readTemperatureHumidity(temperature, humidity);
    tMin = temperature;
    tMax = temperature;
    hMin = humidity;
    hMax = humidity;

    RTC.begin();
    RTC.setHourMode(CLOCK_H24);
    //RTC.setDateTime(__DATE__, __TIME__);
    //RTC.updateWeek();           //Update Weekdaybased on new date.
}

void loop() {
    // Attempt to read the temperature and humidity values from the DHT11 sensor.
    int result = dht11.readTemperatureHumidity(temperature, humidity);

    String temp = "T:" + String(temperature) + "C";
    String humi = "H:" + String(humidity) + "%";

    lcd.setCursor(1,0);
    if (result == 0) {
        lcd.print(temp + " | " + humi);
    } else {
        Serial.println(DHT11::getErrorString(result));
    }

    if (temperature < tMin) {
        tMin = temperature;
    } else if (temperature > tMax) {
        tMax = temperature;
    }

    if (humidity < hMin) {
        hMin = humidity;
    } else if (humidity > hMax) {
        hMax = humidity;
    }

    lcd.setCursor(0,1);
    //lcd.print(String(tMin) + "/" + String(tMax) + "C | " + String(hMin) + "/" + String(hMax) + "%");
    hours = RTC.getHours();
    minutes = RTC.getMinutes();
    seconds = RTC.getSeconds();

    day=RTC.getDay();
    month=RTC.getMonth();

    if(hours<10)
        lcd.print("0");
    lcd.print(hours);
    lcd.print(":");
    if(minutes<10)
        lcd.print("0");
    lcd.print(minutes);
    lcd.print(":");
    if(seconds<10)
        lcd.print("0");
    lcd.print(RTC.getSeconds());
    lcd.print("  ");

    if (seconds<=5) {
        lcd.setCursor(10,1);
        lcd.print(String(tMin) + "/" + String(tMax) + "C");
    } else if (seconds>5 && seconds<=10) {
        lcd.setCursor(10,1);
        lcd.print(String(hMin) + "/" + String(hMax) + "%");
    } else {
        lcd.setCursor(10,1);
		if(day<10)
			lcd.print("0");
		lcd.print(RTC.getDay());
		lcd.print("/");
		if(month<10)
			lcd.print("0");
		lcd.print(RTC.getMonth());
        lcd.print(" ");
    }

    delay(450);
}

