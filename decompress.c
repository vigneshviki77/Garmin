#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define DECOMPRESS_FAILED	0
#define MASK_MSB_BIT		0x7F
#define MSB_BIT				7		// Bit count index starts from 0(LSB) and hence MSB location 7

uint16_t byte_decompress( uint8_t *data_ptr, uint16_t size)
{
    int count1, count2;
    int byte_count = 0;
    uint8_t *data_str = NULL;
	static int idx = 0; //Declared as static, since we are returning idx to indicate the byte count
	
	/* Step 1 : Check Data pointer is valid */
	if (data_ptr == NULL)
		return DECOMPRESS_FAILED;
	
	/* Step 2 : Check if the size of array is greater than zero */
	if (size == 0)
		return DECOMPRESS_FAILED;
	
	/* Step 3 : Find the total number of bytes */
	for (count1 = 0; count1 < size; count1++)
	{
		if (data_ptr[count1] & (1 << MSB_BIT))
		{
			byte_count +=  (data_ptr[count1 + 1]) + 2;	
			count1++; 
		}
		
		else
		{
			byte_count++;
		}
	}
	
	/* Step 4 : Allocate temporary memory */
	data_str = (uint8_t*)malloc(sizeof(uint8_t) * byte_count);
	
	/* Step 5 : Interate complete compressed array to decompress */
	count2 = 0;
	for (count1 = 0; count1 < size; count1++)
	{
		if (data_ptr[count1] & (1 << MSB_BIT))
		{
			data_str[idx] = data_ptr[count1] & MASK_MSB_BIT;
			idx++;
			data_str[idx] = data_ptr[count1] & MASK_MSB_BIT;
			idx++;
			
			if (data_ptr[count1 + 1] != 0)
			{			
				for (count2 = 0; count2 < data_ptr[count1 + 1]; count2++)
				{
					data_str[idx] = data_ptr[count1] & MASK_MSB_BIT;
					idx++;
				}
			}
			count1++;
		}
		
		else
		{
			data_str[idx] = data_ptr[count1];
			idx++;
		}
	}
	
	/* Step 6 : Copy the decompress data to original array */
    memcpy(data_ptr, data_str, idx);
    
    /* Step 7 : Free/Delete the temporary memory */
    free(data_str);
    
    /* Step 8 : Return decompressed array size */
    return idx;
}