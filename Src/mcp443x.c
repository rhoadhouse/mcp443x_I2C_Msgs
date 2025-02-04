/*************************************************************
 
This driver is used to generate the messages for the family of
digital potentiometers mcp443x I2C communication.
 
  
************************************************************/




/********************Includes********************************
*************************************************************/
#include "../Inc/mcp443x.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


/********************Globals********************************
*************************************************************/
uint8_t read_bit = 1;
uint8_t write_bit = 0;



/**********************wiper_chan_to_dev_address**************
This function is used to generate the byte that corresponds
to the device memory address for the desired channel.  This
byte is to be used in tandem with other command bytes.  The
command byte has format 0bAAAAxxxx, where the bits marked
with 'A' are the bits associated with the memory address.

Inputs:
channel -> uint8_t -> the desired channel (0 - 5)

Returns:
dev_address_byte -> uint8_t 
***************************************************************/
static uint8_t wiper_chan_to_dev_address(uint8_t channel){
  
  // initialize the address_byte variable
  uint8_t address_byte = 0b00000000;
  
  // create array to retrieve the value of the channel address
  uint8_t chan_address_decode[] = {0x0, 0x1, 0x6, 0x7};
  
  // get address byte and bit shift into position
  address_byte = chan_address_decode[channel] << 4;

  // Add the bit shifted address to the command
  return address_byte;

}


/**********************wiper_chan_to_dev_address**************
This function is used to generate the byte that corresponds
to the TCON device memory address for the desired wiper channel.
This byte is to be used in tandem with other command bytes.  The
command byte has format 0bAAAAxxxx, where the bits marked
with 'A' are the bits associated with the memory address.

Inputs:
channel -> uint8_t -> the desired tcon register (0 - 3)

Returns:
dev_address_byte -> uint8_t 
***************************************************************/
static uint8_t tcon_num_to_dev_address(uint8_t channel){
  
  // initialize the address_byte variable
  uint8_t address_byte = 0b00000000;
  
  // create array to retrieve the value of the channel address
  uint8_t tcon_address_decode[] = {0x4, 0x4, 0xA, 0xA};
  
  // get address byte and bit shift into position
  address_byte = tcon_address_decode[channel] << 4;

  // Add the bit shifted address to the command
  return address_byte;

}


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
  // XX is the position of the address 
  uint8_t address_byte = 0x58;
  
  // The address for the desired device starts of the second bit
  // Left shift the address by one position to put it in the correct spot
  address = address << 1;

  // Add the bit shifted address to the command
  return address_byte | address;

  

}

/**********************increment_wiper***********************
This function is used to generate the byte messages needed to
increment the value of the wiper for the specified channel.
Acceptable channel values are between 0 and 3.

NOTE: the cp44xx_address_byte must be sent BEFORE this byte 
      to access the appropriate device address

Inputs:
channel -> uint8_t -> the desired channel (0 - 3)

Returns:
command_byte -> uint8_t
***************************************************************/

uint8_t increment_wiper(uint8_t channel) {
  
  //00000100 is the base for the increment command
  uint8_t command_byte = 0b00000100;
  
  //Generate command byte by OR'ing command with the channel address
  command_byte = command_byte | wiper_chan_to_dev_address(channel);
  
  return command_byte;
}



/**********************decrement_wiper***********************
This function is used to generate the byte command message
needed to decrement the value of the wiper for the specified
channel.  Acceptable channel values are between 0 and 3.

NOTE: the cp44xx_address_byte must be sent BEFORE this byte 
      to access the appropriate device address

Inputs:
channel -> uint8_t -> the desired channel (0 - 3)

Returns:
command_byte -> uint8_t
***************************************************************/

uint8_t decrement_wiper(uint8_t channel) {

  //00001000 is the base for the decrement command
  uint8_t command_byte = 0b00001000;  

  //Generate command byte
  command_byte = command_byte | wiper_chan_to_dev_address(channel);

  return command_byte;
}



/**********************set_pot_terminal_cons_cmd***********************
This function is used to generate the byte messages needed to
set the pot's internal terminal connections.  This command REQUIRES
for the set_pot_terminal_cons_data to follow.

Inputs:
channel -> uint8_t -> the desired channel (0 - 3)
switch_state -> uint8_t -> the state of the switch, on or off

Returns:
ret_data -> uint8_t pointer array -> an array of bytes to be sent over I2C to the pot
***************************************************************/
uint8_t set_pot_terminal_cons_cmd(uint8_t channel) {
  
  //00000000 is the base for the write command
  uint8_t command_byte = 0b00000000;  

  //Generate command byte
  command_byte = command_byte | tcon_num_to_dev_address(channel);

  return command_byte;
}


/**********************set_pot_terminal_cons_data***********************
This function is used to generate the byte messages needed to
set the pot's internal terminal connections.  This function REQUIRES
for the set_pot_terminal_cons_data_cmd to preceed it to inform the 
mcp443x chip to accept data.

Inputs:
channel -> uint8_t -> the desired channel (0 - 3)
hw_config_switch_state

switch_state -> uint8_t -> the state of the switch, on or off

Returns:
ret_data -> uint8_t pointer array -> an array of bytes to be sent over I2C to the pot
***************************************************************/
uint8_t set_pot_terminal_cons_data(uint8_t channel, bool hw_config_switch_state, bool term_a_switch_state, bool term_w_switch_state, bool term_b_switch_state) {
  
  //00000000 is the base for the write command
  uint8_t data_byte = 0b00000000;  

  //Generate command byte
  data_byte = data_byte | tcon_num_to_dev_address(channel);

  return 0x00;
}



/**********************set_pot_wiper_val***********************
This function is used to generate the byte message needed to
set the value of the pot wiper at a given channel to a specific
value.

Inputs:
channel -> uint8_t -> the desired channel (0 - 3)

Returns:
ret_data -> uint8_t pointer array -> an array of bytes to be sent over I2C to the pot
***************************************************************/

uint8_t set_pot_wiper_val(uint8_t channel) {
  
  //00000000 is the base for the write command
  uint8_t command_byte = 0b00000000;  

  //Generate command byte
  command_byte = command_byte | wiper_chan_to_dev_address(channel);

  return command_byte;
}



/**********************read_pot_data***********************
This function is used to setup the byte commands needed for
ask the pot for data.

Inputs:
channel -> uint8_t -> the channel of the potentiometer (0 - 3)

Returns:
ret_data -> uint8_t pointer array -> an array of bytes to be sent over I2C to the pot
***************************************************************/

uint8_t read_pot_data(uint8_t channel) {

//00001100 is the base for the read command
  uint8_t command_byte = 0b00001100;  

  //Generate command byte
  command_byte = command_byte | wiper_chan_to_dev_address(channel);

  return command_byte;
}

/**********************read_tcon_data***********************
This function is used to setup the byte commands needed for
the TCON register data request command.

Inputs:
channel -> uint8_t -> the channel of the potentiometer (0 - 3)

Returns:
ret_data -> uint8_t pointer array -> an array of bytes to be sent over I2C to the pot
***************************************************************/

uint8_t read_tcon_data(uint8_t channel) {

//00001100 is the base for the read command
  uint8_t command_byte = 0b00001100;  

  //Generate command byte
  command_byte = command_byte | tcon_num_to_dev_address(channel);

  return command_byte;
}


