
byte menuCount = 1;


void dispalyOption() {
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);

  display.setCursor(10, 0);
  display.println("Set mod:");

  display.setTextSize(1);
  display.setCursor(10, 20);
  display.println("Weekly");
  display.setCursor(10, 30);
  display.println("Daily");
  display.setCursor(10, 40);
  display.println("Start now");
  display.setCursor(10, 50);
  display.println("Back");

  display.setCursor(2, (menuCount * 10) + 10);
  display.println(">");
  display.display();
  return;
}

int setWeekly() {
  int d = 0;
  String string_d;
  int encoderValue = 0;
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.print("MON");
  while (1) {
    encoderValue = analogRead(encoder);
    d = map(encoderValue, 0, 1020, 1, 7);
    switch (d) {
      case 1:
        string_d = "SUNDAY";
        break;

      case 2:
        string_d = "MONDAY";
        break;

      case 3:
        string_d = "TUESDAY";
        break;

      case 4:
        string_d = "WEDNESDAY";
        break;

      case 5:
        string_d = "THURSDAY";
        break;

      case 6:
        string_d = "FRIDAY";
        break;

      case 7:
        string_d = "SATURDAY";
        break;


    }
    display.clearDisplay();
    display.setCursor(20, 10);
    display.print("Day:");
    display.setCursor(20, 32);
    display.print(string_d);
    display.display();
    if (digitalRead(button)) {
      break;
    }
  }
  return d;
}

int setHour() {
  int hr = 0;
  int encoderValue = 0;
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);

  while (1) {
    encoderValue = analogRead(encoder);
    hr = map(encoderValue, 0, 1020, 0, 23);
    display.clearDisplay();
    display.setCursor(40, 10);
    display.print("Hour:");
    display.setCursor(40, 32);
    display.print(String(hr) + ":00");
    display.display();
    if (digitalRead(button)) {
      delay(200);
      break;
    }
  }
  return hr;
}

int setMinute(int hr) {
  int mnt = 0;
  int encoderValue = 0;
  while (1) {
    encoderValue = analogRead(encoder);
    mnt = 5 * map(encoderValue, 0, 1020, 0, 11);
    display.clearDisplay();
    display.setCursor(40, 10);
    display.print("Hour:");
    display.setCursor(40, 32);
    display.print(String(hr) + ":" + String(mnt));
    display.display();
    if (digitalRead(button)) {
      delay(200);
      break;
    }
  }
  return mnt;
}

void setLocally() {
  int hr = 0;
  int mnt = 0;
  while (1) {
    int encoderValue = analogRead(encoder);
    if (encoderValue<=256) menuCount = 4;
    else if(encoderValue > 256 && encoderValue <= 512 ) menuCount = 3;
    else if (encoderValue > 512 && encoderValue<=768) menuCount = 2;
    else if (encoderValue > 768)  menuCount = 1; 
    dispalyOption();
    if (digitalRead(button)) {
      delay(200);
      switch (menuCount) {
        case 1:
          daily = false;
          weekly = true;
          hr = setHour();
          mnt = setMinute(hr);
          periodic = getWeekly(hr, mnt, setWeekly());
          break;
        case 2:
          daily = true;
          weekly = false;
          hr = setHour();
          mnt = setMinute(hr);
          periodic = getDaily(hr, mnt);
          break;
        case 3:
          startNow = true;
          break;
        case 4:
          break;
        default:
          break;
      }
      break;
    }
  }
  return;
}
