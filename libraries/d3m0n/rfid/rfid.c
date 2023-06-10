#include "i2c-display-lib.h"
#include rfid.h

//setup buttons
int button_up = 20;
int button_ok = 19;
int button_down = 18;




void start_rfid()
{
	pinMode(button_up, INPUT_PULLUP); //up
	pinMode(button_ok, INPUT_PULLUP); //ok
	pinMode(button_down, INPUT_PULLUP); //down
	
	
	lcd_setAddr(0x3f);
	lcd_init(16, 17);
	
	lcd_clear();
	
	display("Module", 0);
	display("rfid", 0);
	
	delay(2000);
	return;
}
void display(String text, int line)
{
	lcd_setCursor(line, 0);
	lcd_print(text.c_str());
}