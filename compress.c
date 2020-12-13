#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#define COMPRESS_FAILED		0
#define MAX_BYTE_SIZE		255
#define MSB_BIT				7		// Bit count index starts from 0(LSB) and hence MSB location 7

uint16_t byte_compress( uint8_t *data_ptr, uint16_t size)
{
	int count1, count2;	
	uint8_t current_byte, next_byte, duplicate_counter;
	static int idx = 0; //Declared as static, since we are returning idx to indicate the byte count

	
	/* Step 1 : Check Data pointer is valid */
	if (data_ptr == NULL)
		return COMPRESS_FAILED;
	
	/* Step 2 : Check if the size of array is greater than zero */
	if (size == 0)
		return COMPRESS_FAILED;
		
	/* Step 3 : Ensure none of the array member is greater than 127 */
	for (count1 = 0; count1 < size; count1++)
	{
		if (data_ptr[count1] & (1 << MSB_BIT))
			return COMPRESS_FAILED;
	}
	
	idx = 0;
	/* Step 4 : Interate the entire array to find any repeated */	
	for (count1 = 0; count1 < size; count1++)
	{
		current_byte = data_ptr[count1];
		next_byte = data_ptr[count1+1];
	
		/* Check for adjacent byte is same */	
		if (current_byte == next_byte)
		{
			duplicate_counter = 0;
			
			/* Set MSB bit to indicate repeated sequence */ 
			data_ptr[idx] = current_byte | (1 << MSB_BIT);
			idx++;
			
			/* Set repeated count to 0 */
			data_ptr[idx] = duplicate_counter;
			
			/* Check for further repeated sequence */
			for (count2 = count1 + 2; count2 < size; count2++)
			{
				if (data_ptr[count2] == current_byte)
				{
					/* Update no of repeated byte */
					data_ptr[idx] = ++duplicate_counter;
					
					/* If repeated byte count is greater than 255, than use next two bytes to represent further repeated bytes */
					if (duplicate_counter == MAX_BYTE_SIZE)
					{
						duplicate_counter = 0;
						idx++;
						data_ptr[idx] = current_byte | (1 << MSB_BIT);
						idx++;						
						data_ptr[idx] = duplicate_counter;
					}						
				}
				else
				{
					break;
				}
			}
			
			idx++;			
			count1 = count2 - 1;
		}
		
		/* If not repeated, just store it */
		else
		{
			data_ptr[idx] = current_byte;
			idx++;
		}
	}
	
	/* Step 5 : Return the compressed array size */
	return idx;
}