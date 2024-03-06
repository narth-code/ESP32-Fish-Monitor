#ifndef LCD_HANDLER_H
#define LCD_HANDLER_H

// Used for I2C or SPI
#define OLED_RESET 23

void setupLCD();
void toDisplay();

void testdrawbitmap(const uint8_t *bitmap, uint8_t w, uint8_t h);
void testdrawchar(void);
void testdrawcircle(void);
void testfillrect(void);
void testdrawtriangle(void);
void testfilltriangle(void);
void testdrawroundrect(void);
void testfillroundrect(void);
void testdrawrect(void);
void testdrawline();

#endif