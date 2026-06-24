#include <Arduino.h>

#define SDA 33
#define SCL 32
#define left 16
#define right 17
#define rotate 18
#define drop 19

// Pre-define I2C functions
void IIC_start();
void IIC_send(unsigned char send_data);
void IIC_end();

// LED grid data 
unsigned char data[16] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
bool matrix[16][8];

// Button flags
bool left_flag = 0, right_flag = 0, rotate_flag = 0, drop_flag = 0;

void setup() 
{
    for (int i = 0; i < 16; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            matrix[i][j] = 0;
        }
    }

    // Setup I2C
    pinMode(SCL, OUTPUT);
    pinMode(SDA, OUTPUT);
    digitalWrite(SCL, LOW);
    digitalWrite(SDA, LOW);

    // Setup buttons
    pinMode(left, INPUT_PULLUP);
    pinMode(right, INPUT_PULLUP);
    pinMode(rotate, INPUT_PULLUP);
    pinMode(drop, INPUT_PULLUP);

    Serial.begin(115200);
}

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

    bool left_button = !digitalRead(left);
    Serial.print("left button: "); Serial.println(left_button);

    delay(100);
}

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
    for (char i = 0; i < 8; i++)
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