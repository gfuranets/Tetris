#include <Arduino.h>

#define SDA_PIN 33
#define SCL_PIN 32
#define L_PIN 16
#define R_PIN 17
#define C_PIN 18
#define D_PIN 19

void IIC_start();
void IIC_send(unsigned char send_data);
void IIC_end();

// Data buffer
uint8_t data[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

bool grid[16][8];

void setup() 
{
	Serial.begin(115200);

	pinMode(SCL_PIN, OUTPUT);
	pinMode(SDA_PIN, OUTPUT);
	digitalWrite(SCL_PIN, LOW);
	digitalWrite(SDA_PIN, LOW);
	
	pinMode(L_PIN, INPUT_PULLUP);
	pinMode(R_PIN, INPUT_PULLUP);
	pinMode(C_PIN, INPUT_PULLUP);
	pinMode(D_PIN, INPUT_PULLUP);

	for (int i = 0; i < 16; ++i)
	{
		for (int j = 0; j < 8; ++j)
		{
			grid[i][j] = 0;
		}
	}
}

// Button variables
bool r, l, c, d, r_flag, l_flag, c_flag, d_flag;

// Test cursos - x, y
int x = 0, y = 15;

void loop() 
{
    /**************set the address plus 1***************/
    IIC_start();
    IIC_send(0x40);// set the address plus 1 automatically
    IIC_end();
    /************end the process of address plus 1 *****************/
    /************set the data display*****************/ 
    IIC_start();
    IIC_send(0xc0);// set the initial address as 0

	// Manage button pressing
	r = !digitalRead(R_PIN);
	l = !digitalRead(L_PIN);
	c = !digitalRead(C_PIN);
	d = !digitalRead(D_PIN);

	// Right
	if (r == 1 && r_flag == 0)
	{
		Serial.println("right");
		r_flag = 1;
		++x;
		if (x == 8) x = 0;
		delay(5);

	}
	else if (r == 0 && r_flag == 1)
		r_flag = 0;

	// Left
	if (l == 1 && l_flag == 0)
	{
		Serial.println("left");
		l_flag = 1;
		--x;
		if (x < 0) x = 7;
		delay(5);
	}
	else if (r == 0 && l_flag == 1)
		l_flag = 0;

	// Rotate	
	if (c == 1 && c_flag == 0)
	{
		Serial.println("rotate");
		c_flag = 1;
		// Rotate
		delay(5);
	}
	else if (c == 0 && c_flag == 1)
		c_flag = 0;

	// Drop
	if (d == 1 && d_flag == 0)
	{
		Serial.println("drop");
		d_flag = 1;
		y = 15;
		delay(5);
	}
	else if (d == 0 && d_flag == 1)
		d_flag = 0;
	
	// Sync data with grid
	for (int i = 0; i < 16; ++i) 
	{
		// Clear a row
		data[i] = 0;
		for (int j = 0; j < 8; ++j)
		{
			if (grid[i][j]) data[i] |= 1 << j;
		}
	}

	data[y] |= (1 << x);

	for (uint8_t i = 0; i < 16; ++i) 
		IIC_send(data[i]);

    IIC_end();
    /************end the data display*****************/
    /*************set the brightness display***************/ 
    IIC_start();
    IIC_send(0x8A);// set the brightness display
    IIC_end(); 
    /*************end the brightness display***************/ 
    delay(100);
}

void IIC_start()
{
	digitalWrite(SCL_PIN, LOW);
	delayMicroseconds(3);
	digitalWrite(SDA_PIN, HIGH);
	delayMicroseconds(3);
	digitalWrite(SCL_PIN, HIGH);
	delayMicroseconds(3);
	digitalWrite(SDA_PIN, LOW);
	delayMicroseconds(3);
}

void IIC_send(uint8_t send_data)
{
	for(char i = 0;i < 8;i++)
	{
		digitalWrite(SCL_PIN, LOW);
		delayMicroseconds(3); 

		if (send_data & 0x01)
			digitalWrite(SDA_PIN, HIGH);
		else
			digitalWrite(SDA_PIN, LOW);

		delayMicroseconds(3);
		digitalWrite(SCL_PIN, HIGH); 
		delayMicroseconds(3);
		send_data = send_data >> 1;
	}
}

void IIC_end()
{
	digitalWrite(SCL_PIN, LOW);
	delayMicroseconds(3);
	digitalWrite(SDA_PIN, LOW);
	delayMicroseconds(3);
	digitalWrite(SCL_PIN, HIGH);
	delayMicroseconds(3);
	digitalWrite(SDA_PIN, HIGH);
	delayMicroseconds(3);
}