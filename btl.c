#include <16f877a.h>             // Thu vien cua PIC16F877A
#device ADC=10  
#fuses hs 
#use delay(clock=20000000)       // Dat tan so xung nhip la 20 Mhz

#define LCD_ENABLE_PIN PIN_D2
#define LCD_RS_PIN PIN_D0
#define LCD_RW_PIN PIN_D1

#define LCD_DATA4 PIN_D4
#define LCD_DATA5 PIN_D5
#define LCD_DATA6 PIN_D6
#define LCD_DATA7 PIN_D7

#include "lcd.c" // Thu vien lcd

float value_adc, previous_value_adc, t2, previous_t2;
int x;

void do_nhiet_do() {
   setup_adc(ADC_CLOCK_INTERNAL); // Cau hinh xung nhip cho ADC
   setup_adc_ports(AN0);          // Thiet lap AN0 lam ngo vao analog
   set_adc_channel(0);            // Chon kenh adc0
   value_adc = read_adc();        // Doc gia tri adc tu kenh 0 
   value_adc = (float)value_adc * 0.488; // Gan gia tri ADC vao bien x
   delay_ms(500); 
   lcd_gotoxy(1,1);            // Di chuyen con tro LCD
   printf(lcd_putc, "T1 = %3.2f ", value_adc); // In gia tri ADC len LCD
   lcd_putc(223);
   lcd_putc("C");
}

void phan_loai() {
   if (input(PIN_C1) == 0) {
      x = 1;
      delay_ms(100);
   }
   if (input(PIN_C2) == 0) {
      x = 2;
      delay_ms(100);
   }
   if (input(PIN_C1) == 1 && input(PIN_C2) == 1) {
      x = 0;
      delay_ms(100); 
   }
}

void chinh_nhiet_do(int x) {
   if (x == 1) {
      t2 = t2 + 0.5;
   }
   if (x == 2) {
      t2 = t2 - 0.5;
   }
   else {
      t2 = t2;
   }
   lcd_gotoxy(1,2);            // Di chuyen con tro LCD
   printf(lcd_putc, "T2 = %3.2f ", t2); // In gia tri len LCD
   lcd_putc(223);
   lcd_putc("C"); 
   
   if (t2 > previous_t2) {
      output_low(PIN_B2);
      output_high(PIN_B1);
      delay_ms(500); 
      output_low(PIN_B1);
   }
   else if (t2 < previous_t2) {
      output_low(PIN_B1);
      output_high(PIN_B2);
      delay_ms(500);
      output_low(PIN_B2);
   }
   previous_t2 = t2;
}

void main() {
   lcd_init(); // Khoi tao LCD
   t2 = 30;
   while (true) {
      if (input(PIN_C0) == 0) {
         value_adc = 30;     // Doc gia tri ADC tu kenh 0
         lcd_putc("\f");
         lcd_gotoxy(1,1);    // Di chuyen con tro LCD
         printf(lcd_putc, "T1 = %3.2f ", value_adc); // In gia tri len LCD
         lcd_putc(223);
         lcd_putc("C");   
         
         lcd_gotoxy(1,2);    // Di chuyen con tro LCD
         printf(lcd_putc, "T2 = %3.2f ", value_adc); // In gia tri len LCD
         lcd_putc(223);
         lcd_putc("C"); 
         t2 = 30;
         
         output_high(PIN_B1);
         delay_ms(150); 
         output_low(PIN_B1);
         output_high(PIN_B2);
         delay_ms(150);
         output_low(PIN_B2);
         output_high(PIN_B1);
         delay_ms(150); 
         output_low(PIN_B1);
      }
      else {
         do_nhiet_do();
         phan_loai();
         chinh_nhiet_do(x);
         if (value_adc > previous_value_adc) {
            output_low(PIN_B2);
            output_high(PIN_B1);
            delay_ms(500); 
            output_low(PIN_B1);
         }
         else if (value_adc < previous_value_adc) {
            output_low(PIN_B1);
            output_high(PIN_B2);
            delay_ms(500);
            output_low(PIN_B2);
         }
         previous_value_adc = value_adc;
      }
   }
}

