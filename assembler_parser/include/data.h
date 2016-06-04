#ifndef _DATA_H
#define _DATA_H
#include <string>

class Data
{
public:
	Data(int offset, int line_number, int position);
	int offset;
	int line_number;
	int position;
	virtual std::ostream& dump(std::ostream& o) const = 0;
	friend std::ostream& operator<<(std::ostream& o, const Data& d)
	{
		return d.dump(o);
	}
};

#endif //_DATA_H
