/*
 * File: I2C_LCD.h
 * Driver Name: [[ I2C_LCD Display ]]
 * SW Layer:   ECUAL
 * Created on: Jan 28, 2024
 * Author:     Khaled Magdy
 * -------------------------------------------
 * For More Information, Tutorials, etc.
 * Visit Website: www.DeepBlueMbedded.com
 *
 */

#ifndef I2C_LCD_H_
#define I2C_LCD_H_

#include "stdint.h"

#define I2C_LCD_MAX	1	// Maximum Number of I2C_LCD Modules in Your Project
#define I2C_LCD	0	// I2C_LCD Instance Number 1 (Add more if you need)

//-----[ Prototypes For All User External Functions ]-----

void I2C_LCD_Init(uint8_t I2C_LCD_InstanceIndex);                                  //initialization
void I2C_LCD_Clear(uint8_t I2C_LCD_InstanceIndex);                                 //clear display
void I2C_LCD_Home(uint8_t I2C_LCD_InstanceIndex);                                  //set cursor in row 0 col 0
void I2C_LCD_SetCursor(uint8_t I2C_LCD_InstanceIndex, uint8_t Col, uint8_t Row);   //set cursor in col row
void I2C_LCD_WriteChar(uint8_t I2C_LCD_InstanceIndex, char Ch);                    //write char
void I2C_LCD_WriteString(uint8_t I2C_LCD_InstanceIndex, char* Str);                //write string

void I2C_LCD_ShiftLeft(uint8_t I2C_LCD_InstanceIndex);                             //shift every col of one space sx
void I2C_LCD_ShiftRight(uint8_t I2C_LCD_InstanceIndex);                            //shift every col of one space dx

void I2C_LCD_Backlight(uint8_t I2C_LCD_InstanceIndex);                             //light up backlight
void I2C_LCD_NoBacklight(uint8_t I2C_LCD_InstanceIndex);                           //shut off backlight
void I2C_LCD_Display(uint8_t I2C_LCD_InstanceIndex);                               //activate display control
void I2C_LCD_NoDisplay(uint8_t I2C_LCD_InstanceIndex);                             //disactivate display and clear it
void I2C_LCD_Cursor(uint8_t I2C_LCD_InstanceIndex);                                //show cursor to indicate actual cursor point
void I2C_LCD_NoCursor(uint8_t I2C_LCD_InstanceIndex);                              //hide cursor that indicates actual cursor point
void I2C_LCD_Blink(uint8_t I2C_LCD_InstanceIndex);                                 //show blinking cursor 
void I2C_LCD_NoBlink(uint8_t I2C_LCD_InstanceIndex);                               //shut off blinking cursor

void I2C_LCD_CreateCustomChar(uint8_t I2C_LCD_InstanceIndex, uint8_t CharIndex, const uint8_t* CharMap);
void I2C_LCD_PrintCustomChar(uint8_t I2C_LCD_InstanceIndex, uint8_t CharIndex);
void I2C_LCD_ACapo(uint8_t I2C_LCD_InstanceIndex);


#endif /* I2C_LCD_H_ */
