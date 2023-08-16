/**
 * @file displaylcd.cpp
 * @author your name (you@domain.com)
 * @brief 
 * @details description
 * @version 0.1
 * @date 2023-08-08
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include "../includes/displaylcd.hpp"


/**
 * @brief 
 * @details
 * 
 */
void DisplayLCD::setupDisplayLCD() {
  this->lcd.init();  
  this->lcd.clear ();
}

void DisplayLCD::mostraConsumo(float consumo) {
                       // text position
  
  this->lcd.setCursor (0,0);      // units position relative to scale
  this->lcd.print ("Gasto: ");
  this->lcd.setCursor (0,1);   // units position relative to scale
  this->lcd.print (consumo);  
  this->lcd.setCursor (0,3);  // units position relative to scale
  this->lcd.print ("J");
}

void DisplayLCD::mostraVelocidadeMedia(double vel_media) {
  
  /**
   * @brief 
   * 
   */
                              //text position
 
  this->lcd.setCursor (1,0);   // units position relative to scale
  this->lcd.print ("Vel Med: ");
  this->lcd.setCursor (1,2);   // units position relative to scale
  this->lcd.print (vel_media);  
  this->lcd.setCursor (1,3);  // units position relative to scale
  this->lcd.print ("KM/H");  
}



