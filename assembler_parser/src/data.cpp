#include "data.h"

/**
 * Data implementation
 */


Data::Data(int offset, int line_number, int position){
	this->offset = offset;
	this->line_number = line_number;
	this->position = position;
}
