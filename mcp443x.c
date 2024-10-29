/*************************************************************
 *
 *  This driver is used to generate the messages for the
 *  family of digital potentiometers mcp443x
 *
 *  Mesage structure:
 *  start bit -> high speed mode command (if 3.4 Mbit comm desired) -> control
 *byte  -> Ackknowledge bit from slave device -> Command Byte -> aknowledge bit
 *from slave device -> data byte -> High Speed CMD: Start bit: when the serial
 *data line is pulled low while the serial clock line is high (normal operation
 *the serial data line is stable while the serial clock line is high) Control
 *Byte: always starts with 01011 followed by the two address bits then the R/W
 *bit (0 is write, 1 is read) Command Byte: used when the write bit is set in
 *the address byte Data Byte (third byte): the byte of data that is to be
 *written to either the volitile wiper memory, or TCON register
 *
 *
 ************************************************************/


/********************Includes********************************
*************************************************************/
#include "mcp443x.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>


/********************Globals********************************
*************************************************************/
uint8_t read_bit = 1;
uint8_t write_bit = 0;


/**********************mcp443x_address_byte***********************
This function is used to generate the address byte for the mcp443x
digital potentiometer.

Inputs:
address -> uint8_t -> the i2c address of the potentiometer (0 - 3)

Returns:
address_byte -> uint8_t 
***************************************************************/
uint8_t mcp44xx_address_byte(uint8_t address){
  // Address byte has a constant 0b01011XX0 (0x58)
  // XX is the poasition of the address 
  uint8_t address_byte = 0x58;
  
  // The address for the desired device starts of the second bit
  // Left shift the address by one position to put it in the correct spot
  address = address << 1;

  // Add the bit shifted address to the command
  return address_byte + address;

  

}

/**********************increment_wiper***********************
This function is used to generate the byte messages needed to
increment the value of the wiper for the specified channel at
a given I2C address.

Inputs:
address -> uint8_t -> the i2c address of the potentiometer (0 - 3)
channel -> uint8_t -> the desired channel (0 - 3)
iinc_num -> uint8_t -> the number of times to increment the wiper

Returns:
ret_data -> uint8_t pointer array -> an array of bytes to be sent over I2C to the pot
***************************************************************/

uint8_t increment_wiper(uint8_t address, uint8_t channel, uint8_t inc_num) {
  
  uint8_t command_byte = 0b00000100;

  // bit shift the intial command by channel num
  uint8_t channel_byte = 0b00000000;
  channel_byte = 0b00010000 << channel;
  
  //Generate command byte
  command_byte = command_byte + channel_byte;

  
  return command_byte;
}



/**********************decrement_wiper***********************
This function is used to generate the byte messages needed to
decrement the value of the wiper for the specified channel at
a given I2C address.

Inputs:
address -> uint8_t -> the i2c address of the potentiometer (0 - 3)
channel -> uint8_t -> the desired channel (0 - 3)
iinc_num -> uint8_t -> the number of times to decrement the wiper

Returns:
ret_data -> uint8_t pointer array -> an array of bytes to be sent over I2C to the pot
***************************************************************/

uint8_t decrement_wiper(uint8_t address, uint8_t channel, uint8_t inc_num) {
  
  uint8_t command_byte = 0b00001000;

  // bit shift the intial command by channel num
  uint8_t channel_byte = 0b00000000;
  channel_byte = 0b00010000 << channel;
  

  //Generate command byte
  command_byte = command_byte + channel_byte;

  return command_byte;
}



/**********************set_pot_terminal_cons***********************
This function is used to generate the byte messages needed to
set the pot's internal terminal connections.

Inputs:
address -> uint8_t -> the i2c address of the potentiometer (0 - 3)
channel -> uint8_t -> the desired channel (0 - 3)
switch_state -> uint8_t -> the state of the switch, on or off

Returns:
ret_data -> uint8_t pointer array -> an array of bytes to be sent over I2C to the pot
***************************************************************/
uint8_t *set_pot_terminal_cons(uint8_t address, uint8_t channel, uint8_t switch_state) {
  return 0x00;
}



/**********************set_pot_wiper_val***********************
This function is used to generate the byte messages needed to
set the value of the pot wiper at a given channgel to a specific
value.

Inputs:
address -> uint8_t -> the i2c address of the potentiometer (0 - 3)
channel -> uint8_t -> the desired channel (0 - 3)
wiper value -> uint8_t -> the value to set the wiper to

Returns:
ret_data -> uint8_t pointer array -> an array of bytes to be sent over I2C to the pot
***************************************************************/

uint8_t *set_pot_wiper_val(uint8_t address, uint8_t channel,uint8_t wiper_value) {
  uint8_t *ret_data = (uint8_t *)malloc(3);
  address = address << 1;
  uint8_t address_byte = 0b01011000 + address + write_bit;
  ret_data[0] = address_byte;
  
  // bit shift the intial command by channel num
  uint8_t command_byte = 0b00010000 << channel;
  
  // add the write command (00) to the command bytes bit 2 and 3 locatons
  command_byte = command_byte + 0b00000000;
                                 
  ret_data[1] = command_byte;
  ret_data[2] = wiper_value;
  return ret_data;
}



/**********************read_pot_data***********************
This function is used to setup the byte commands needed for
ask the pot for data.

Inputs:
address -> uint8_t -> the i2c address of the potentiometer (0 - 3)

Returns:
ret_data -> uint8_t pointer array -> an array of bytes to be sent over I2C to the pot
***************************************************************/

uint8_t *read_pot_data(uint8_t address) {
  uint8_t *ret_data = (uint8_t *)malloc(1);
  address = address << 1;
  uint8_t address_byte = 0b01011000 + address + read_bit;
  ret_data[0] = address_byte;
  return ret_data;
}


/**********************compilier switch for testing***********************/


#if MY_SWITCH
int main() {

  printf("Hello World from mcp443_message.c\n");
  uint8_t increment_val = 4;
  uint8_t *data = read_pot_data(1);
  for (int i = 0; i <= increment_val; i++) {
    printf("%x\n", data[i]);
  }

  return 0;
}
#else
#endif