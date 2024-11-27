#include <iostream>

class testClass{
    public:
        int num;
        int getNum(){
            std::cout << "Test?" << std::endl;
            return 9;
        }
};