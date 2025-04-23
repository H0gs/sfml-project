#include <iostream>
#include <vector>
#include <memory> // For std::unique_ptr
#include "BaseClass.h"
#include "DerivedClass.h"

//g++ main.cpp BaseClass.cpp DerivedClass.cpp -o program
class Superclass {
    public:
        virtual void showMessage() {
            std::cout << "Message from Superclass" << std::endl;
            testMethod();
        }
        virtual void testMethod(){
            std::cout << "Superclass Test Method!";
        }
    };
    
class Subclass : public Superclass {
    public:
        void showMessage() override {
            std::cout << "Message from Subclass" << std::endl;
            Superclass::showMessage();  // Calling the superclass method
        }
        void testMethod() override {
            std::cout << "Subclass Test Method";
        }
};
    
    


//----- Final version

bool contains(std::vector<int> nums, int num){
    for(int n : nums){
        if(n == num){
            return true;
        }
    }
    return false;
}

void scramblerHelper(std::vector<int> original, std::vector<int> vec, std::vector<std::vector<int>>* storage){
    storage->push_back(vec);
    if(vec.size() != original.size()){ //Base case, this array is done and should be added back to the main storage
        for(int n : original){
            if(!contains(vec, n)){
                std::vector<int> temp = vec; //Makes a copy
                temp.push_back(n);
                scramblerHelper(original, temp, storage);
            }
        }
    }
}

std::vector<std::vector<int>> scambler(std::vector<int> nums){
    int x = 10;
    std::vector<std::vector<int>> storage;

    std::vector<int> empty;

    scramblerHelper(nums, empty, &storage);
    for(std::vector<int> vec : storage){
        std::cout << "-> ";
        for(int n : vec){
            std::cout << n << ", ";
        }
        std::cout << std::endl;
    }
    std::cout << "X: " << x << std::endl;
    return storage;
}

//------------------- Final version end

int main(){
    std::cout << "Hello World!" << std::endl;


    std:: vector<int> nums;
    for(int i = 0; i < 3; i++){
        nums.push_back(i);
    }
    // // printFunction(nums);

    // std::vector<int> temp;

    // numFunction(temp, nums);

    scambler(nums);

    for(int i = 0; i < 3; i++){
        std::cout << std::endl;
    }

    std::vector<int> nums2;
    for(int i = 0; i < 6; i++){
        nums2.push_back(i);
    }

    for(int i = 0; i < nums2.size(); i++){
        std::cout << nums2.at(i);
    }

    Subclass obj;
    obj.showMessage();
    // obj.testMethod();












    // int x = 5;

    // int* x_ptr = &x;
    // int& x_ref = x;

    // std::cout << x << std::endl;
    // std::cout << x_ptr << std::endl;
    // std::cout << x_ref << std::endl;

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