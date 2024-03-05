# How does Virtual Work?

## What are Vtables and how are they represented?
VTables (or virtual tables) are arrays of virtual functions.
They allow C++ to support dynamic dispatch (or run-time method binding).
Virtual functions are member functions of a C++ class that can be redefined in a child class.
A pointer to a vtable will exist as a datamember of all classes with virtual functions.
When I call a virtual method, I lookup the object's v-table and call the appropriate derived class method.

Essentially allows the below behavior:
```c++
#include <iostream>

struct Base{
    virtual int method(){
        return 1;
    }
};

struct Derived: Base{
    int method() override{
        return 2;
    }
};

int main(){
    Base* a = new Derived();
    // output is 2
    std::cout << a->method() << std::endl;
    // The original can still be called with Base::method
}
```

So what does this look like under the hood? This is all compiler dependant but let's say I have the below code:
```c++
struct Base{
    virtual int method();
    virtual int method2();
    virtual int method3();
};
```
The compiler builds a table containing 3 function-pointers. This is a relatively small space overhead. Every object of type base will have a pointer to this object.

Now lets say we have a Derived class.

```c++
struct Derived: Base {
    int method() override;
};
```

The compiler will build and store another static table in memory.
Since we don't add any more virtual functions the table is still of size 3.
Now, we can just use the same function pointers for method1 and method2, but we need to replace function pointer for method and any other things we might override.
The pointer for derived class objects will be to this new vtable.

When we try to call a virtual function we dereference the pointer and use the function pointer to call the function. This way we are able to figure out which function to use dynamically. A good explanation of what goes on under the hood and in the hardware can be found [here](http://www.dietmar-kuehl.de/mirror/c++-faq/virtual-functions.html#faq-20.4). Essentially there are three calls that go on in the hardware to fetch the location and load the function.

## Why are Virtual Functions slow?

Virtual functions need the extra steps to load the function from a vtable. This naturally takes a little extra time.

The real problem is that, since the compiler cannot know which functions are going to be called, it cannot inline or do many other optimizations.
This is the primary cause of slowness and why templates are often used in settings where performance is critical.
A stackexchange post going into more detail can be found [here](https://softwareengineering.stackexchange.com/questions/191637/in-c-why-and-how-are-virtual-functions-slower).

Some work has been done for runtime optimizations that can decrease the impact of the lack of compile time optimization: [Profile Guided Optimization](https://learn.microsoft.com/en-us/cpp/build/profile-guided-optimizations?view=msvc-170).
I am not very familiar with it but it can allow for optimizations where the optimizer uses data from test runs to determine areas that need to be optimized.
