#include "polling.h"

char* polling() 
{
	short bufferIndex;
	short currentBufferSize;
	char* buffer = sys_alloc_mem(BUFFERSIZE + 1); //Buffer size is presently 25 chars
	char* value = sys_alloc_mem(MAXVALUESIZE); //Max size is presently three chars
	char currentSymbol;

  	//Initializes the command buffer to blank spaces
	for (bufferIndex = 0; bufferIndex < BUFFERSIZE + 1; bufferIndex++) {
		buffer[bufferIndex] = 32;
	}

	bufferIndex = 0;
	currentBufferSize = 0;
	value = " ";

	while (1) {
		if (inb(COM1 + 5) & 1) {
			*value = inb(COM1);

			//check for buffer underflow
			if (bufferIndex <= 0) {
				//serial_println("Buffer < 0");
				bufferIndex = 0;
			}
			//check for buffer overflow
			else if (bufferIndex >= BUFFERSIZE) {
				bufferIndex = BUFFERSIZE - 1;
			}

      		//BACKSPACE
			if (*value == 127 && bufferIndex != 0) {
				bufferIndex--;
				int tmp;
				value[0] = 8;
				serial_print(value);
				for (tmp = bufferIndex; tmp < currentBufferSize; tmp++) {
					buffer[tmp] = buffer[tmp + 1];
					value[0] = buffer[tmp + 1];
					serial_print(value);
				}
				for ( ; tmp > bufferIndex; tmp--) {
					value[0] = 8;
					serial_print(value);
				}
				currentBufferSize--;
			}
      		//DELETE / ARROWS
			else if (*value == 27) {
				*value = inb(COM1);

				if (*value == 91) {
					*value = inb(COM1);
					int tmp;
					switch(*value) {
						case 'A': //Up Arrow Key
							break;
						case 'B': //Down Arrow Key
							break;
						case 'C': //Right Arrow Key
							if (bufferIndex != currentBufferSize) {
								bufferIndex++;
								value[0] = currentSymbol;
								currentSymbol = buffer[bufferIndex];
								serial_print(value);
							}
							break;
						case 'D': //Left Arrow Key
							if (bufferIndex != 0) {
								bufferIndex--;
								currentSymbol = buffer[bufferIndex];
								value[0] = 8;
								serial_print(value);
							}
							break;
						default: //Delete Key
							if (currentBufferSize != 0) {
								for (tmp = bufferIndex; tmp < currentBufferSize; tmp++) {
									buffer[tmp] = buffer[tmp + 1];
									value[0] = buffer[tmp + 1];
									serial_print(value);
								}
								for ( ; tmp > bufferIndex; tmp--) {
									value[0] = 8;
									serial_print(value);
								}
								currentSymbol = buffer[bufferIndex];
								currentBufferSize--;
							}
							break;
					}
				} 
			}
      		//CARRIAGE RETURN
			else if (*value == 13) {
				buffer[currentBufferSize] = '\0';
				break;
			}
      		//NEW LINE FEED
			else if (*value == 10) {
				serial_println("NL");
				break;
			}
			else {
				if ((isspace(value) || 
	    			//0-9
					(*value >= 48 && *value <= 57) ||
	    			//A-Z
					(*value >= 65 && *value <= 90) ||
	    			//a-z
					(*value >= 97 && *value <= 122)) &&
					currentBufferSize < BUFFERSIZE) 
				{
					buffer[bufferIndex] = *value;
					bufferIndex++;
					currentBufferSize++;
					int tmp;
					serial_print(value);
					for (tmp = bufferIndex; tmp < currentBufferSize; tmp++) {
						char holder = buffer[tmp];
						buffer[tmp] = currentSymbol;
						value[0] = currentSymbol;
						serial_print(value);
						currentSymbol = holder;
					}
					for ( ; tmp > bufferIndex; tmp--) {
						value[0] = 8;
						serial_print(value);
					}
					currentSymbol = buffer[bufferIndex];
				}
			}
		}
	}
	serial_println("");
	sys_free_mem(value);
	while (isspace(buffer)) {
    	buffer++;
  	}
	return buffer;
}
