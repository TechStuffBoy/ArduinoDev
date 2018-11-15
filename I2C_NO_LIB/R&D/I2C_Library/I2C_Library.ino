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
    
}

void loop()
{

}

void TWIInit(void)
{
    //set SCL to 400kHz
    TWSR = 0x00;
    TWBR = 0x0C;
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
    // Clear TWI interrupt flag, Put start condition on SDA, Enable TWI
    TWCR= (1<<TWINT)|(1<<TWSTA)|(1<<TWEN);    
    while(!(TWCR & (1<<TWINT))); // Wait till start condition is transmitted
    while((TWSR & 0xF8)!= START_TRNSMTED); // Check for the acknowledgement
}

void TWI_send_read_bit_plus_reciever_address(unsigned char data)
{
    //Here data is &bit Address + Data Direction bit(Write = 0)
    TWDR=data;    // Address and read instruction
    TWCR=(1<<TWINT)|(1<<TWEN);    // Clear TWI interrupt flag,Enable TWI
    while (!(TWCR & (1<<TWINT))); // Wait till complete TWDR byte received
    while((TWSR & 0xF8)!= SLAVE_ADDRESS_AND_READ_BIT_TXD_ACK_RECVD);  // Check for the acknoledgement
}

void TWI_send_write_bit_plus_reciever_address(unsigned char data)
{
    TWDR=data;    // put data in TWDR
    TWCR=(1<<TWINT)|(1<<TWEN);    // Clear TWI interrupt flag,Enable TWI
    while (!(TWCR & (1<<TWINT))); // Wait till complete TWDR byte transmitted
    while((TWSR & 0xF8) != SLAVE_ADDRESS_AND_WRITE_BIT_SEND_ACK_RECVD); // Check for the acknoledgement
}

void TWI_write_data_to_reciever(unsigned char data)
{
    TWDR=data;    // put data in TWDR
    TWCR=(1<<TWINT)|(1<<TWEN);    // Clear TWI interrupt flag,Enable TWI
    while (!(TWCR & (1<<TWINT))); // Wait till complete TWDR byte transmitted
    while((TWSR & 0xF8) != DATA_TXD_ACK_RECVD); // Check for the acknoledgement
}

void TWI_stop(void)
{
    // Clear TWI interrupt flag, Put stop condition on SDA, Enable TWI
    TWCR= (1<<TWINT)|(1<<TWEN)|(1<<TWSTO);    
    while(!(TWCR & (1<<TWSTO)));  // Wait till stop condition is transmitted
}

void TWI_read_single_byte_from_slave(void)
{
    byte recv_data = 0;
    TWCR=(1<<TWINT)|(1<<TWEN);    // Clear TWI interrupt flag,Enable TWI
    while (!(TWCR & (1<<TWINT))); // Wait till complete TWDR byte transmitted
    while((TWSR & 0xF8) != DATA_BYTE_RECVD_ACK_RETURNED); // Check for the acknoledgement
    recv_data=TWDR;
}

void TWI_read_multiple_byte_from_slave(void)
{
    byte recv_data = 0;
    TWCR=(1<<TWINT)|(1<<TWEN);    // Clear TWI interrupt flag,Enable TWI
    while (!(TWCR & (1<<TWINT))); // Wait till complete TWDR byte transmitted
    while((TWSR & 0xF8) != DATA_BYTE_RECVD_NACK_RETURNED); // Check for the acknoledgement
    recv_data=TWDR;
}
 
