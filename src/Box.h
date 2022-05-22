#pragma once

struct Box {
	int x;
	int y;
	Box& operator= (const Box& box)
	{
	    this->x=box.x;
	    this->y=box.y;
	    return *this;
	}
	const bool operator== (const Box& box)
	{
	    if(this->x==box.x)
        {
            if(this->y==box.y)
            {
                return true;
            }
        }
        return false;
	}
};
