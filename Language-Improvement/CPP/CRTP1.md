# CRTP

Many people use CRTP instead of OOP for static polymorphism.
CRTP can be used in cases where you know the type of the object at runtime.
Here is an example:

```c++
#include <iostream>
 
template <class T>
struct Base
{
    void impl(){std::cout << "Spooky";}
};
struct D1 : public Base<D1>
{
    void impl(){std::cout << "D1\n";}
};
struct D2 : public Base<D2>
{
    void impl(){std::cout << "D2\n";}
};
 
int main()
{
    D1 d1; d1.impl();
    D2 d2; d2.impl();
}
```

In this case, we have a Base function and we can generate two more structs using D1 and D2 as the templates.