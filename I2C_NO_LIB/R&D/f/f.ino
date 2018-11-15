// Program for Slave mode

#define OWN_SLAVE_ADDRESS_PLUS_WRITE_CAME_FRM_MASTER_ACK_RETRND_BACK 0x60
#define ARBITRATION_LOST_IN_SLAVE_R_W_OWN_SL_ADDRS_RECVD_ACK_RETRND 0x68
#define GENERAL_CALL_ADDRESS_RECIEVED_ACK_RTRND 0x70
#define ARBITRATION_LOST_IN_SLAVE_R_W_GENERAL_CALL_ADDRESS_RECIEVED_ACK_RETRND 0x78
#define PREVIOUSLY_ADDRSD_WITH_OWN_SL_ADDR_AND_W_DATA_RECVD_ACK_RTRND 0x80
#define PREVIOUSLY_ADDRSD_WITH_OWN_SL_ADDR_AND_W_DATA_RECVD_NACK_RTRND 0x88
#define PREVIOUSLY_ADDRSED_WITH_GENERAL_CALL_DATA_RECVD_ACK_RTRND 0x90
#define PREVIOUSLY_ADDRSED_WITH_GENERAL_CALL_DATA_RECVD_NACK_RTRND 0x98
#define STOP_OR_REPEATED_START_RECVD_WHILE_STILL_ADDRSD_AS_SLAVE 0xA0

#define OWN_SL_ADDRS_PLUS_READ_RECVD_ACK_RTRND 0xA8
#define ARBTRN_LOST_IN_SL_ADDR_R_W_OWN_SL_ADDR_R_RECVD_ACK_RETRND 0xB0
#define DATA_BYTE_IN_TWDR_TXMTD_ACK_RECIVED 0xB8
#define DATA_BYTE_IN_TWDR_TXMTD_NACK_RECIVED 0xC0
#define LAST_DATA_BYTE_IN_TWDR_TXMTD_ACK_RECIVED 0xC8

#define NO_RELEVANT_STATE_INFO_AVAILABLE_TWINT_IS_ZERO 0xF8
#define BUS_ERROR_DUE_TO_ILLEGAL_START_STOP 0x00

#include<avr/io.h>
#include<util/delay.h>

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
    Serial.println("<------------------END--------------------->");
}


void TWI_init_slave(void) // Function to initilaize slave
{
    //slave is initialized at 100Khz speed
    TWSR = 0x00;
    TWBR = 0x48;
    TWCR = (1<<TWEN);
    TWCR = (1<<TWINT);
    Serial.println("TWI_init_slave");
    TWAR= ( 0x20<<1 | 1); // Fill slave address to TWAR
}
 
void TWI_write_slave(void) // Function to write data
{
    Serial.println("TWI_write_slave");
    TWDR= write_data;           // Fill TWDR register whith the data to be sent 
    TWCR= (1<<TWEN)|(1<<TWINT);// Enable TWI, Clear TWI interrupt flag
    //printStatus(TWSR & 0xF8);
    Serial.println(TWSR & 0xF8,HEX);
    while((TWSR & 0xF8) != 0xC0); // Wait for the acknowledgement
}
 
void TWI_match_write_slave(void) //Function to match the slave address and slave dirction bit(write) 
{
    Serial.println("TWI_match_write_slave");
    while((TWSR & 0xF8)!= 0xA8) // Loop till correct acknoledgement have been received
    {
        //printStatus(TWSR & 0xF8);
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
    //printStatus(TWSR & 0xF8);
    Serial.println(TWSR & 0xF8,HEX);
    while((TWSR & 0xF8)!=0x80); // Wait for acknowledgement
    recv_data=TWDR; // Get value from TWDR
}
 
void TWI_match_read_slave(void) //Function to match the slave address and slave dirction bit(read)
{
    Serial.println("TWI_match_read_slave");
    while((TWSR & 0xF8)!= 0x60)  // Loop till correct acknoledgement have been received
    {
        //printStatus(TWSR & 0xF8);
        Serial.println(TWSR & 0xF8,HEX);
        // Get acknowlegement, Enable TWI, Clear TWI interrupt flag
        TWCR=(1<<TWEA)|(1<<TWEN)|(1<<TWINT); 
        while (!(TWCR & (1<<TWINT)));  // Wait for TWINT flag
    }
}

void printStatus(int hexVal)
{
    switch(hexVal)
    {
        case 0x60 :
            Serial.println("OWN_SLAVE_ADDRESS_PLUS_WRITE_CAME_FRM_MASTER_ACK_RETRND_BACK");
            break;
        case 0x68:
            Serial.println("ARBITRATION_LOST_IN_SLAVE_R_W_OWN_SL_ADDRS_RECVD_ACK_RETRND");
            break;
        case 0x70 :
            Serial.println("GENERAL_CALL_ADDRESS_RECIEVED_ACK_RTRND");
            break;
        case 0x78 :
            Serial.println("ARBITRATION_LOST_IN_SLAVE_R_W_GENERAL_CALL_ADDRESS_RECIEVED_ACK_RETRND");
            break;
        case 0x80 :
            Serial.println("PREVIOUSLY_ADDRSD_WITH_OWN_SL_ADDR_AND_W_DATA_RECVD_ACK_RTRND");
            break;
        case 0x88 :
            Serial.println("PREVIOUSLY_ADDRSD_WITH_OWN_SL_ADDR_AND_W_DATA_RECVD_NACK_RTRND");
            break;
        case 0X90 :
            Serial.println("PREVIOUSLY_ADDRSED_WITH_GENERAL_CALL_DATA_RECVD_ACK_RTRND");
            break;
        case 0x98 :
            Serial.println("PREVIOUSLY_ADDRSED_WITH_GENERAL_CALL_DATA_RECVD_NACK_RTRND");
            break;
        case 0xA0 :
            Serial.println("STOP_OR_REPEATED_START_RECVD_WHILE_STILL_ADDRSD_AS_SLAVE");
            break;
        case 0xA8 :
            Serial.println("OWN_SL_ADDRS_PLUS_READ_RECVD_ACK_RTRND");
            break;
        case 0XB0 :
            Serial.println("ARBTRN_LOST_IN_SL_ADDR_R_W_OWN_SL_ADDR_R_RECVD_ACK_RETRND");
            break;
        case 0xB8 :
            Serial.println("DATA_BYTE_IN_TWDR_TXMTD_ACK_RECIVED");
            break;
        case 0xC0 :
            Serial.println("DATA_BYTE_IN_TWDR_TXMTD_NACK_RECIVED");
            break;
        case 0xC8 :
            Serial.println("LAST_DATA_BYTE_IN_TWDR_TXMTD_ACK_RECIVED");
            break;
        case 0xF8 :
            Serial.println("NO_RELEVANT_STATE_INFO_AVAILABLE_TWINT_IS_ZERO");
            break;
        case 0x00 :
            Serial.println("BUS_ERROR_DUE_TO_ILLEGAL_START_STOP");
            break;
    }
}

