#include <iostream>

class Basic {
    protected:
    int a;
    int b;
    //constructor
    public:
    Basic(int a, int b) : a(a), b(b) {
        std::cout << "Creating" << std::endl;
    }
    //copy constructor
    Basic(const Basic& other): a(other.a), b(other.b){
        std::cout << "Copying" << std::endl;
    }
    //assignment operator
    Basic& operator=(const Basic& other){
        if (this == &other) return *this;
        this->a = other.a;
        this->b = other.b;
        std::cout << "assigning" << std::endl;
        return *this;
    }
    //destructor
    virtual ~Basic() {
        std::cerr << "Deleting" << std::endl;
    }
    virtual int doSomething(){
        return a + b;
    }
};

class Advanced : public Basic {
    public:
    Advanced(int a, int b) : Basic(a,b){};
    int doSomething() override {
        return a * b;
    }
    ~Advanced(){
        std::cout << "Deleting Advanced" << std::endl;
    }
};

int main(){
    Basic* a = new Advanced(1, 2);
    std::cout << a->doSomething() << std::endl;
    delete a;
}