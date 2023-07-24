#include <INA.h>​
INA_Class INA;

int32_t rawCurrent = -666; //-666
uint16_t rawVoltage = 666;
int32_t rawWatts = -666;
String output = String(8);
String current = String(8);
String voltage = String(8);
String watts = String(8);
String temp = String(8);
uint8_t x = 0;
bool negativeNumber = 0;

void setup() {
  Serial.begin(115200);
  INA.begin(1,100000);
  INA.setAveraging(10);
  INA.setBusConversion(10000);
  INA.setShuntConversion(10000); //10000
  INA.setMode(INA_MODE_CONTINUOUS_BOTH);
}

void loop() {
  rawCurrent = INA.getBusMicroAmps(100);
  rawVoltage = INA.getBusMilliVolts();
  rawWatts = INA.getBusMicroWatts(); 
  current = divideBy1000( String(rawCurrent , DEC) );        // Ex: -23.452
  voltage = divideBy1000( String(rawVoltage , DEC) );        // Ex:    .125
  temp = String(rawWatts , DEC);                             // Divide raw microwatts figure by 1000 and discard decimals
  x = temp.length();
  temp.remove(x-3);
  watts = divideBy1000(temp);                                // Divide milliwatts by 1000 and keep decimals
  
  Serial.print("Corrente = ");
  Serial.print(current);
  Serial.println(" mA     ");
  
  Serial.print("Tensao = ");
  Serial.print(voltage);
  Serial.println(" V     ");
  
  Serial.print("Potencia = ");
  Serial.print(watts);
  Serial.println(" W     ");
  
  delay(1000);
}

String divideBy1000(String input){ 
  output = "";
  if(input.startsWith("-")){
    output.concat("-");
    input.remove(0,1);
  }
  switch(input.length()){
    case 0: {
      return "NaN";
    }
    break;
    case 1: {
      output.concat(".00");
      output.concat(input);
    }break;
    case 2: {
      output.concat("0");
      output.concat(input);
    }break;
    case 3: {
      output.concat(".");
      output.concat(input);
    }break;
    case 4: {
      output.concat(input.substring(0,1));
      output.concat(".");
      output.concat(input.substring(1));
    }break;
    case 5: {
      output.concat(input.substring(0,2));
      output.concat(".");
      output.concat(input.substring(2));
    }break;
    case 6: {
      output.concat(input.substring(0,3));
      output.concat(".");
      output.concat(input.substring(3));
    }break;
}
  return output;
}
