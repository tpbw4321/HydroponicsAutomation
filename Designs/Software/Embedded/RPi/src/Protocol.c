/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdint.h>
#include "Protocol.h"

/**
 * @function Protocol_DecodeInput(char * input)
 * @param None
 * @return Takes an incoming data transmission and decodes it
 * @brief Converts the current frequency and returns flow rate
 * @author Barron Wong 02/08/19
*/
message_t Protocol_DecodeInput(char * input){
    char * ptr;
    message_t message;
    uint8_t count = 0;
    
    ptr = strtok(input,":");
    
    while(ptr != NULL){
        switch(count){
            case 0:{
				message.key = atoi(ptr);
            break;
            }
            case 1:{
				message.value = atof(ptr);
            break;
            }
            default:{
                message.key = invalid_key;
				message.value = -1;
            }
        }
        count++;
        ptr = strtok(NULL,":");
    }
    
    return message;
    
}
/**
 * @function Protocol_EncodeOutput(char * input)
 * @param key, value, and buffer
 * @return String pointer ready for output
 * @brief Takes a key and a parameter and get it read for output
 * @author Barron Wong 02/08/19
*/
char * Protocol_EncodeOutput(pkey_t key, float value, char * buffer){
    int size = strlen(buffer);
 
    //clear buffer
    for(int i = 0; i < size; i++){
        buffer[0] = 0;
    }
    if(key > invalid_key && key <= flow_measured){
        sprintf(buffer,"%d:%f",(int)key,value);
    }

    return buffer;
}

/**
 * @function Protocol_PrintMessage(char * input)
 * @param message_t
 * @return none
 * @brief Prints the contents of a message
 * @author Barron Wong 02/08/19
*/
void Protocol_PrintMessage(message_t * msg) {
	if (msg->key > invalid_key && msg->key <= flow_measured) {
		printf("%s:%f\r\n", pkeys[msg->key], msg->value);
	}

}

#ifdef PROTOCOL_TEST
#include "USBCom.h"
    
int main(void)
{

    
    
}
#endif
/* [] END OF FILE */