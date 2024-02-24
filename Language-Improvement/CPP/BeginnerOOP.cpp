#include <iostream>
struct dummy{
    void func(int a){
    std::cout << a << std::endl;
    }
    void func(int a, int b){
        std::cout << a + b << std::endl;
    }
};


int main() {
    dummy a;
    a.func(1);
    a.func(1, 2);
}
