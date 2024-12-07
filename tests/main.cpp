#include <iostream>
#include <vector>
#include <memory> // For std::unique_ptr
#include "BaseClass.h"
#include "DerivedClass.h"

//g++ main.cpp BaseClass.cpp DerivedClass.cpp -o program

int main(){
    std::cout << "Hello World!" << std::endl;

    int x = 5;

    int* x_ptr = &x;
    int& x_ref = x;

    std::cout << x << std::endl;
    std::cout << x_ptr << std::endl;
    std::cout << x_ref << std::endl;

    // int x = 5;
    // int* x_ptr = &x;
    // std::cout << x << std::endl;
    // std::cout << x_ptr << std::endl;
    // std::cout << *x_ptr << std::endl;

    // std::unique_ptr<int> uniquePtr(x_ptr);
    // std::unique_ptr<int> x2 = std::make_unique(x_ptr);

    // delete x_ptr;

    // BaseClass* baseClass = new DerivedClass(); // --> this becomes a dangling pointer when the next line is run and a smart pointer is created
    // std::unique_ptr<BaseClass> uniqueBaseClass(baseClass);
    
    // std::unique_ptr<BaseClass> bc = std::make_unique<BaseClass>();
    // std::unique_ptr<BaseClass> dc = std::make_unique<DerivedClass>();



    // DerivedClass* derivedClass;

    // std::vector<std::unique_ptr<BaseClass>> vectorThing;
    

    // baseClass->testMethod();
    // derivedClass->testMethod();
    
    // vectorThing.push_back(uniqueBaseClass);
    // vectorThing.push_back(std::make_unique<DerivedClass>());

    // std::cout << vectorThing.size() << std::endl;

    // vectorThing.at(0)->testMethod();
    // vectorThing.at(1)->testMethod();
    // for(auto& b : vectorThing){
    //     b->testMethod();
    // }





        // UNIQUE POINTER STUFF
    /*
    std::vector<std::unique_ptr<BaseClass>> vect;

    vect.push_back(std::make_unique<BaseClass>());
    vect.push_back(std::make_unique<DerivedClass>());

    vect.at(0)->testMethod();
    vect.at(1)->testMethod();

    for(std::unique_ptr<BaseClass>& obj : vect){
        obj->testMethod();
    }
    */

    return 0;
}