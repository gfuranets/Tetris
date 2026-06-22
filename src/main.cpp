#include <Arduino.h>

#define SDA 33
#define SCL 32

unsigned char data_line = 0;
unsigned char delay_count = 0;
unsigned char data_display1 = 0;
unsigned char data_display2 = 0;
unsigned char data_display3 = 0;
unsigned char data_display4 = 0;
unsigned char data_display5 = 0;
unsigned char data_display6 = 0;
unsigned char data_display7 = 0;
unsigned char data_display8 = 0;
unsigned char data_display9 = 0;
unsigned char data_display10 = 0;
unsigned char data_display11 = 0;
unsigned char data_display12 = 0;
unsigned char data_display13 = 0;
unsigned char data_display14 = 0;
unsigned char data_display15 = 0;
unsigned char data_display16 = 0;
void IIC_start();
void IIC_send(unsigned char send_data);
void IIC_end();

unsigned char data[16] = {0x00,0x00,0x00,0x00,0x26,0x41,0x86,0x80,0x80,0x80,0x86,0x41,0x26,0x00,0x00,0x00};
bool matrix[16][8];

void setup() 
{
    for (int i = 0; i < 16; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            matrix[i][j] = 0;
        }
    }

    pinMode(SCL, OUTPUT);
    pinMode(SDA, OUTPUT);
    digitalWrite(SCL, LOW);
    digitalWrite(SDA, LOW);
}

/*----------------------------------------------------------------*/
void loop() 
{
    /**************set the address plus 1***************/
    IIC_start();
    IIC_send(0x40);// set the address plus 1 automatically
    IIC_end();
    /************end the process of address plus 1 *****************/
    /************set the data display*****************/ 
    IIC_start();
    IIC_send(0xc0);// set the initial address as 

    // Translate game matrix to data array
    for (int i = 0; i < 16; i++)
    {
        data[i] = 0;
        for (int j = 0; j < 8; j++) 
        {
            data[i] |= (matrix[i][j] << j); // Shift 1/0 accordingly + add to data[i], get binary number
        }
    }
    
    // Send display data
    for (char i = 0; i < 16; i++) IIC_send(data[i]);

    IIC_end();
    /************end the data display*****************/
    /*************set the brightness display***************/ 
    IIC_start();
    IIC_send(0x8A);// set the brightness display
    IIC_end(); 
    /*************end the brightness display***************/ 
    delay(100);
}

/*----------------------------------------------------------------*/
void IIC_start()
{
    digitalWrite(SCL, LOW);
    delayMicroseconds(3);
    digitalWrite(SDA, HIGH);
    delayMicroseconds(3);
    digitalWrite(SCL, HIGH);
    delayMicroseconds(3);
    digitalWrite(SDA, LOW);
    delayMicroseconds(3);
}

void IIC_send(unsigned char send_data)
{
    for(char i = 0; i < 8; i++)
    {
        digitalWrite(SCL, LOW);
        delayMicroseconds(3); 

        if(send_data & 0x01) digitalWrite(SDA, HIGH);
        else digitalWrite(SDA, LOW);

        delayMicroseconds(3);
        digitalWrite(SCL, HIGH); 
        delayMicroseconds(3);
        send_data = send_data >> 1;
    }
}

void IIC_end()
{
    digitalWrite(SCL, LOW);
    delayMicroseconds(3);
    digitalWrite(SDA, LOW);
    delayMicroseconds(3);
    digitalWrite(SCL,HIGH);
    delayMicroseconds(3);
    digitalWrite(SDA, HIGH);
    delayMicroseconds(3);
}