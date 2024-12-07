#ifndef DERIVEDCLASS_H
#define DERIVEDCLASS_H
#include <iostream>
#include "BaseClass.h"

class DerivedClass : public BaseClass{
    public:
        void testMethod() override;
};

#endif