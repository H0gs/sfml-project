#ifndef BASECLASS_H
#define BASECLASS_H
#include <iostream>

class BaseClass{
    private:
        int value;
    public:    
        virtual void testMethod();
        void setValue(int val);
        
};

#endif