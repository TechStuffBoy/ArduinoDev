#define START_TRNSMTED 0x08
#define REPEATED_START_TRNSMTD 0x10
#define ARBITN_LOST 0x38
#define SLAVE_ADDRESS_AND_READ_BIT_TXD_ACK_RECVD 0x40
#define SLAVE_ADDRESS_AND_READ_BIT_TXD_NACK_RECVD 0x48
#define DATA_RCVD_ACK_RTRND 0x50
#define DATA_RCVD_NACK_RTRND 0x58
#define DATA_TXD_ACK_RECVD 0x28
#define DATA_BYTE_RECVD_ACK_RETURNED 0x50
#define DATA_BYTE_RECVD_NACK_RETURNED 0x58
#define SLAVE_ADDRESS_AND_WRITE_BIT_SEND_ACK_RECVD 0x18
#define SLAVE_ADDRESS_AND_WRITE_BIT_SEND_NACK_RECVD 0x20

void setup()
{
    Serial.begin(9600);
}

void loop()
{

}

void TWI_init_master(void)
{
    Serial.println("Initializing Master at 100Khz");
    //set SCL to 400kHz
    TWSR = 0x00;
    //TWBR = 0x0C;
    TWBR = 0x48;
    //enable TWI
    TWCR = (1<<TWEN);
}

/*
 *void TWI_init_master(void) // Function to initialize master
{
    TWBR=0x01;    // Bit rate
    TWSR=(0<<TWPS1)|(0<<TWPS0);    // Setting prescalar bits
    // SCL freq= F_CPU/(16+2(TWBR).4^TWPS)
}
 */

 void TWI_start(void)
{
    Serial.println("Start Condition Issued");
    // Clear TWI interrupt flag, Put start condition on SDA, Enable TWI
    TWCR= (1<<TWINT)|(1<<TWSTA)|(1<<TWEN);    
    while(!(TWCR & (1<<TWINT))); // Wait till start condition is transmitted
    while((TWSR & 0xF8)!= START_TRNSMTED); // Check for the acknowledgement
}

void TWI_repeated_start(void)
{
    // Clear TWI interrupt flag, Put start condition on SDA, Enable TWI
    TWCR= (1<<TWINT)|(1<<TWSTA)|(1<<TWEN); 
    while(!(TWCR & (1<<TWINT))); // wait till restart condition is transmitted
    while((TWSR & 0xF8)!= 0x10); // Check for the acknoledgement
}

void TWI_send_read_bit_plus_reciever_address(unsigned char data)
{
    Serial.println("TWI_send_read_bit_plus_reciever_address");
    //Here data is &bit Address + Data Direction bit(Write = 0)
    TWDR=data;    // Address and read instruction
    TWCR=(1<<TWINT)|(1<<TWEN);    // Clear TWI interrupt flag,Enable TWI
    while (!(TWCR & (1<<TWINT))); // Wait till complete TWDR byte received
    while((TWSR & 0xF8)!= SLAVE_ADDRESS_AND_READ_BIT_TXD_ACK_RECVD);  // Check for the acknoledgement
}

void TWI_send_write_bit_plus_reciever_address(unsigned char data)
{
    Serial.println("TWI_send_write_bit_plus_reciever_address");
    TWDR=data;    // put data in TWDR
    TWCR=(1<<TWINT)|(1<<TWEN);    // Clear TWI interrupt flag,Enable TWI
    while (!(TWCR & (1<<TWINT))); // Wait till complete TWDR byte transmitted
    while((TWSR & 0xF8) != SLAVE_ADDRESS_AND_WRITE_BIT_SEND_ACK_RECVD); // Check for the acknoledgement
}

void TWI_write_data_to_reciever(unsigned char data)
{
    Serial.println("TWI_write_data_to_reciever");
    TWDR=data;    // put data in TWDR
    TWCR=(1<<TWINT)|(1<<TWEN);    // Clear TWI interrupt flag,Enable TWI
    while (!(TWCR & (1<<TWINT))); // Wait till complete TWDR byte transmitted
    while((TWSR & 0xF8) != DATA_TXD_ACK_RECVD); // Check for the acknoledgement
}


byte TWI_read_single_byte_from_slave(void)
{
    Serial.println("Reading Single byte");
    TWCR=(1<<TWINT)|(1<<TWEN);    // Clear TWI interrupt flag,Enable TWI
    while (!(TWCR & (1<<TWINT))); // Wait till complete TWDR byte transmitted
    while((TWSR & 0xF8) != DATA_BYTE_RECVD_ACK_RETURNED); // Check for the acknoledgement
    return TWDR;
}

byte TWI_read_multiple_byte_from_slave(void)
{
    Serial.println("Read Multiple Byte");
    TWCR=(1<<TWINT)|(1<<TWEN);    // Clear TWI interrupt flag,Enable TWI
    while (!(TWCR & (1<<TWINT))); // Wait till complete TWDR byte transmitted
    while((TWSR & 0xF8) != DATA_BYTE_RECVD_NACK_RETURNED); // Check for the acknoledgement
    return TWDR;
}


void TWI_stop(void)
{
    Serial.println("TWI_stop");
    // Clear TWI interrupt flag, Put stop condition on SDA, Enable TWI
    TWCR= (1<<TWINT)|(1<<TWEN)|(1<<TWSTO);    
    while(!(TWCR & (1<<TWSTO)));  // Wait till stop condition is transmitted
}
