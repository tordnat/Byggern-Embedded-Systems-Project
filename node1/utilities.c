#include "utilities.h"

uint8_t byte_to_percentage(uint8_t val){
	return (val*100)/255;
}

uint8_t array_avg(uint8_t* array, uint8_t size){
	float sum = 0;
	for (uint8_t i = 0; i <= size; i++){
		sum += array[i];
	}
	return (int) (sum/(float) size);
}