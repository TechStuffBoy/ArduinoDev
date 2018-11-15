
// Program for Slave mode
#include<avr/io.h>
#include<util/delay.h>
#include "Wire.h"

void TWI_init_slave(void);
void TWI_match_read_slave(void);
void TWI_read_slave(void);
void TWI_match_write_slave(void);
void TWI_write_slave(void);
 
unsigned char write_data,recv_data;
 
void setup(){
    Serial.begin(9600);
    //Wire.begin(0x20);
    TWI_init_slave(); // Function to initilaize slave
}
void loop()
{
    TWI_match_read_slave(); //Function to match the slave address and slave dirction bit(read) 
    TWI_read_slave(); // Function to read data
     
    write_data=~recv_data; // Togglem the receive data
     
    TWI_match_write_slave(); //Function to match the slave address and slave dirction bit(write) 
    TWI_write_slave();       // Function to write data
}


void TWI_init_slave(void) // Function to initilaize slave
{
    //slave is initialized at 100Khz speed
    TWSR = 0x00;
    TWBR = 0x48;
    TWCR = (1<<TWEN);
    TWCR = (1<<TWINT);
    Serial.println("TWI_init_slave");
    TWAR= ( 0x20<<1 | 0); // Fill slave address to TWAR
}
 
void TWI_write_slave(void) // Function to write data
{
    Serial.println("TWI_write_slave");
    TWDR= write_data;           // Fill TWDR register whith the data to be sent 
    TWCR= (1<<TWEN)|(1<<TWINT);// Enable TWI, Clear TWI interrupt flag
    Serial.println(TWSR & 0xF8,HEX); 
    while((TWSR & 0xF8) != 0xC0); // Wait for the acknowledgement
}
 
void TWI_match_write_slave(void) //Function to match the slave address and slave dirction bit(write) 
{
    Serial.println("TWI_match_write_slave");
    while((TWSR & 0xF8)!= 0xA8) // Loop till correct acknoledgement have been received
    {
    Serial.println(TWSR & 0xF8,HEX);
    // Get acknowlegement, Enable TWI, Clear TWI interrupt flag
    TWCR=(1<<TWEA)|(1<<TWEN)|(1<<TWINT); 
    while (!(TWCR & (1<<TWINT)));  // Wait for TWINT flag
    }
}
 
void TWI_read_slave(void)
{
    Serial.println("TWI_read_slave");
    // Clear TWI interrupt flag,Get acknowlegement, Enable TWI
    TWCR|=(1<<TWINT);
    TWCR|=(1<<TWEN)|(1<<TWEA)|(0<<TWSTA)|(0<<TWSTO); 
    while (!(TWCR & (1<<TWINT))); // Wait for TWINT flag
    Serial.println(TWSR & 0xF8,HEX);
    while((TWSR & 0xF8)!=0x80); // Wait for acknowledgement
    recv_data=TWDR; // Get value from TWDR
}
 
void TWI_match_read_slave(void) //Function to match the slave address and slave dirction bit(read)
{
    Serial.println("TWI_match_read_slave");
    while((TWSR & 0xF8)!= 0x60)  // Loop till correct acknoledgement have been received
    {
    Serial.println(TWSR & 0xF8,HEX);
    // Get acknowlegement, Enable TWI, Clear TWI interrupt flag
    TWCR=(1<<TWEA)|(1<<TWEN)|(1<<TWINT); 
    while (!(TWCR & (1<<TWINT)));  // Wait for TWINT flag
    }
}
