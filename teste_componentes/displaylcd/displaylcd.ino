#include <Wire.h>
#include <LiquidCrystal_I2C.h>
 
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  lcd.init();  
  lcd.backlight();
  lcd.clear();
}


void loop() {
  //Cria o primeiro arco-íris
  //VELOCIMETRO
  vel = 50;
  gasto = 200;
  vel_media = 25;
  
 
  //Cria o texto do GASTO
  //JOULÍMETRO - Gasto acumulado;
  
  lcd.setCursor (0,0);                              // units position relative to scale
  lcd.print ("Gasto: ");
  lcd.setCursor (0,1);                           // units position relative to scale
  lcd.print (gasto);  
  lcd.setCursor (0,2);                          // units position relative to scale
  lcd.print ("J");

  //Cria o texto do Vel Med
  //VEL MED - Velocidade media;
  
  lcd.setCursor (1,0);                              // units position relative to scale
  lcd.print ("Vel Med: ");
  lcd.setCursor (1,1);                           // units position relative to scale
  lcd.print (vel_media);  
  lcd.setCursor (1,2);                          // units position relative to scale
  lcd.print ("KM/H");  

}

