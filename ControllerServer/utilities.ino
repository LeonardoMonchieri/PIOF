
//Get minDate from all the setted allarm (periodic or inside the list)
time_t getMinAllarm(void){
  time_t minAllarm = fromString(F("2100-01-01 00:00"));
  if (!allarms_list.isEmpty()) {
    if (allarms_list.getValue(allarms_list.getSize()-1) < minAllarm) {
      minAllarm = allarms_list.getValue(allarms_list.getSize()-1);
    }
  }
  if (daily || weekly) {
    if ( periodic < minAllarm ) {
      minAllarm = periodic;
    }
  }
  return minAllarm;
}



//Return daily Allaram
time_t getDaily(int h,int m){
  tmElements_t dailyAllarm;
  dailyAllarm.Year = year() - 1970;
  dailyAllarm.Month = month();
  dailyAllarm.Day = day();
  dailyAllarm.Minute = m;
  dailyAllarm.Hour = h;
  dailyAllarm.Second = 0;
  if (hour() >= h) {
    if (minute() >= m) {
      dailyAllarm.Day = dailyAllarm.Day + 1;
    }
  }
  return makeTime(dailyAllarm);
}

//Return the next daily allarm
time_t nextDaily(time_t allarm){
  tmElements_t nextD;
  nextD.Year = year(allarm) - 1970;
  nextD.Month = month(allarm);
  nextD.Day = day(allarm)+1;
  nextD.Minute = minute(allarm);
  nextD.Hour = hour(allarm);
  nextD.Second = 0;

  return makeTime(nextD);
}

//Return the next weekly allarm
time_t nextWeekly(time_t allarm){
  tmElements_t nextW;
  nextW.Year = year(allarm) - 1970;
  nextW.Month = month(allarm);
  nextW.Day = day(allarm)+7;
  nextW.Minute = minute(allarm);
  nextW.Hour = hour(allarm);
  nextW.Second = 0;
  
  return makeTime(nextW);
} 


//Return weekly Allaram
time_t getWeekly(int h, int m, int wd){
  tmElements_t weeklyAllarm;
  weeklyAllarm.Year = year() - 1970;
  weeklyAllarm.Month = month();
  weeklyAllarm.Day = day();
  weeklyAllarm.Minute = m;
  weeklyAllarm.Hour = h;
  weeklyAllarm.Second = 0;
  time_t allarm = makeTime(weeklyAllarm);
  
  //If the day is today but the time is passed
  if (wd == weekday(allarm)) {
    if (hour() >= weeklyAllarm.Hour) {
      if (minute() >= weeklyAllarm.Minute) {
        weeklyAllarm.Day = weeklyAllarm.Day + 7;
        allarm = makeTime(weeklyAllarm);
        return allarm;
      }
    }
  }
  //If the day isn't today
  while (wd != weekday(allarm)) {
    weeklyAllarm.Day = weeklyAllarm.Day + 1;
    allarm = makeTime(weeklyAllarm);
  }
  return allarm;

}

//Get time_t format from a String format
time_t fromString(String s) {
  tmElements_t allarm;
  String date = s.substring(0, 10); //Date
  String tim = s.substring(11, 16); //Time

  int yr = date.substring(0, 4).toInt();  //year
  int mot = date.substring(5, 7).toInt(); //month
  int dy = date.substring(8, 10).toInt(); //day
  int hr = tim.substring(0, 2).toInt();   //hour
  int mit = tim.substring(3, 5).toInt();  //minute

  allarm.Year = yr - 1970;
  allarm.Month = mot;
  allarm.Day = dy;
  allarm.Hour = hr;
  allarm.Minute = mit;
  allarm.Second = 0;

  return makeTime(allarm);
}



//Return a String that rappresent the time_t date
String timeToString(time_t date) {
  struct tm *lt = localtime(&date);
  char str[32];
  strftime(str, sizeof str, "%d/%m/%Y %H:%M", lt);
  return str;
}


//Return a int that rappresent the time_t date
int timeToInt(time_t date) {
  int t=(int) date;
  Serial.println(t);
  return t;
}
