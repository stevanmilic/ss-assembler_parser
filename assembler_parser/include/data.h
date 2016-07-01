#ifndef _DATA_H
#define _DATA_H
#include <string>

class Data
{
public:
	int offset;
	Data(int offset)
	{
		this->offset = offset;
	}
	virtual std::ostream& dump(std::ostream& o) const = 0;
	friend std::ostream& operator<<(std::ostream& o, const Data& d)
	{
		return d.dump(o);
	}
};

#endif //_DATA_H
