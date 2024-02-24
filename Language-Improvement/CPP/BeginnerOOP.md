# Refreshing OOP Knowledge

Lots of people have opinions on OOP. I'm not experienced enough to have a good take on the matter. My goal is to change that, of course. I, too, want to declare that OOP is an anti-pattern.

Below is a Super Basic class.

```c++
class SuperBasic {};
```
It doesn't actually do anything so let's make a less basic class.

```c++
#include <iostream>

class Basic {
    private:
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
    ~Basic() {
        std::cout << "Deleting" << std::endl;
    }
    int doSomething(){
        return a + b;
    }
};

int main(){
    Basic a = Basic(1, 2);
    Basic b = Basic(a);
    Basic c = Basic(3, 2);
    b = c;
    std::cout << b.doSomething() << std::endl;
}
```

This shows how to create 4 basic functions and running it will show how each of them of them are used. The destructor will be particularly important for getting rid of memory leaks but that's not super important for now.

Let's say we want an object that inherits from this Basic class. The way to do that would be the following:

```c++
class Advanced : public Basic {
    public:
    Advanced(int a, int b) : Basic(a,b){};
    int doSomething(){
        return a * b;
    }
    ~Advanced(){
        std::cout << "Deleting Advanced" << std::endl;
    }
};
```

This doesn't work because we declared items in Basic private. Let's fix that:

```c++
class Basic {
    protected:
    int a;
    int b;
    ...
```

Now when we try the following we should get 2.
```c++
int main(){
    Basic* a = new Advanced(1, 2);
    std::cout << a->doSomething() << std::endl;
    delete a;
}
```

We get 3. This is because we didn't override. To do that we have to set the function we are overriding to be virtual. We also don't see deleting advanced so we have to set the Basic destructor to be virtual too.

```c++
class Basic{
    ...
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
    ...
```

Now all is right in the world.
