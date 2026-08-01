#include <Arduino.h>
#include "Game.h"

#define SDA_PIN 33
#define SCL_PIN 32
#define L_PIN 16
#define R_PIN 17
#define C_PIN 18
#define D_PIN 19

void IIC_start();
void IIC_send(uint8_t data);
void IIC_end();
void sendFrame(uint8_t data[]);

// Data buffer
uint8_t data[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

// Define a game object
Game game;

// Loop variable
uint32_t last_time;

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
}

// Button variables
bool r, l, c, d, r_flag, l_flag, c_flag, d_flag;

// Test cursos - x, y
// int x = 0, y = 15;

void loop() 
{
	// MANAGE USER INPUT

	r = !digitalRead(R_PIN);
	l = !digitalRead(L_PIN);
	c = !digitalRead(C_PIN);
	d = !digitalRead(D_PIN);

	// Right
	if (r == 1 && r_flag == 0)
	{
		game.handleInput('R');
		r_flag = 1;
		delay(5);

	}
	else if (r == 0 && r_flag == 1)
		r_flag = 0;

	// Left
	if (l == 1 && l_flag == 0)
	{
		game.handleInput('L');
		l_flag = 1;
		delay(5);
	}
	else if (l == 0 && l_flag == 1)
		l_flag = 0;

	// Rotate	
	if (c == 1 && c_flag == 0)
	{
		game.handleInput('C');
		c_flag = 1;
		delay(5);
	}
	else if (c == 0 && c_flag == 1)
		c_flag = 0;

	// Drop
	if (d == 1 && d_flag == 0)
	{
		game.handleInput('D');
		d_flag = 1;
		delay(5);
	}
	else if (d == 0 && d_flag == 1)
		d_flag = 0;

	/* --------------------------------------------------------- */

	// TICK GRAVITY

	if (millis() - last_time > 1000)
	{
		// Handle falling (default movement)
		game.handleInput('F');
		last_time = millis();
	}

	/* --------------------------------------------------------- */

	// RENDER 
	game.clear(data);
	game.display(data);

	/* --------------------------------------------------------- */
	
	// SEND
	sendFrame(data);
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

void IIC_send(uint8_t data)
{
	for (uint8_t i = 0; i < 8; ++i)
	{
		digitalWrite(SCL_PIN, LOW);
		delayMicroseconds(3); 

		if (data & 0x01)
			digitalWrite(SDA_PIN, HIGH);
		else
			digitalWrite(SDA_PIN, LOW);

		delayMicroseconds(3);
		digitalWrite(SCL_PIN, HIGH); 
		delayMicroseconds(3);
		data = data >> 1;
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

void sendFrame(uint8_t data[])
{
    IIC_start();
    IIC_send(0x40); // auto-increment adress mode
    IIC_end();
 
    IIC_start();
    IIC_send(0xc0); // start at adress 0
	for (uint8_t i = 0; i < 16; ++i) 
		IIC_send(data[i]);
    IIC_end();

    IIC_start();
    IIC_send(0x8A); // brightness
    IIC_end(); 

    delay(10);
}
