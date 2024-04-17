+++
title = "Beginner OOP"
date = "2024-03-11"
+++

# Refreshing OOP Knowledge

Lots of people have opinions on OOP. I'm not experienced enough to have a good take on the matter. My goal is to change that, of course. I, too, want to declare that OOP is an anti-pattern.


Below is a super basic class.


```c++
class SuperBasic {};
```

It doesn't actually do anything, so I'll make a less basic class.


```c++
#include <iostream>

class Basic {

private:
    int a;
    int b;
public:
    //constructor

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


This shows how to create 4 basic functions (constructor, copy, assignment, destructor), and running it will show how each of them of them are used. The destructor will be particularly important for preventing memory leaks, but that's not important for now. In addition, there is a public method we can use to do something with the class.

Let's say I want an object that inherits from this Basic class. The way to do that would be the following:

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

This doesn't work because I declared items in Basic private. Let's fix that:

```c++
class Basic {
protected:
    int a;
    int b;
    ...
```

Protected means it can be accessed by classes that inherit it. Now when I try the below we I expect to get 2:

```c++
int main(){
    Basic* a = new Advanced(1, 2);
    std::cout << a->doSomething() << std::endl;
    delete a;
}
```

I get 3. This is because I didn't override. To do that, I have to set the function I'm overriding to be virtual in the base class and add override to the derived class. I also don't see deleting advanced, so I have to set the Basic destructor to be virtual, too, which will make it so the pointer calls the derived destructor first. The reasoning behind adding virtual will be covered later when I discuss vtables and vptrs.

```c++
class Basic{
    ...
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

Now everything works as intended. So far, this is has been a very cursory overview of how classes work and some basic inheritance. I'm now going to use this for a bigger example to show how this would be used in practice. Traditionally this is taught with Animals eating or making noises. I'm going to gamble.

```c++
#include <vector>
#include <algorithm>
#include <random>
#include <iostream>


class Card {
public:
    std::string suit;
    std::string rank;

    Card(std::string suit, std::string rank) : suit(suit), rank(rank) {}
};
```

First I'm going to include the necessary headers and then make a basic card class.
```c++
#include <vector>
#include <algorithm>
#include <random>
#include <iostream>

class Card {
public:
    enum Suit { H, D, C, S };
    enum Rank { Two, Three, Four, Five, Six, Seven, Eight, Nine, Ten, J, Q, K, A };

    Card(Suit suit, Rank rank) : suit(suit), rank(rank) {}

    Suit getSuit() const { return suit; }
    Rank getRank() const { return rank; }
    friend std::ostream& operator<<(std::ostream& os, const Card& card);
private:
    Suit suit;
    Rank rank;
};

std::ostream& operator<<(std::ostream& os, const Card& card) {
    const char* suitNames[] = {"Hearts", "Diamonds", "Clubs", "Spades"};
    const char* rankNames[] = {"2", "3", "4", "5", "6", "7", "8", "9", "10", "Jack", "Queen", "King", "Ace"};

    return os << rankNames[static_cast<int>(card.rank)] << " of " << suitNames[static_cast<int>(card.suit)];
}
```

The enums here can be strings, it doesn't really matter for the purpose of this demonstration. The friend operator allows us to access private and protected members of a class from certain methods. In this case we can now std::cout a card.

Using these cards I'll make a deck that I can shuffle, insert into, and deal from. Note that since I am using pointers to Card objects that are allocated on the heap with new, I need to clean them up with my destructor otherwise there will be a memory leak.

```c++
class Deck {
public:
    Deck() {
        for (int suit = 0; suit < 4; ++suit) {
            for (int rank = 0; rank < 13; ++rank) {
                cards.emplace_back(new Card(Card::Suit(suit), Card::Rank(rank)));
            }
        }
    }

    void shuffle() {
        std::shuffle(cards.begin(), cards.end(), std::default_random_engine {});
    }

    Card* dealCard() {
        if (cards.empty()) {
            throw std::out_of_range("Out of cards buddy...");
        }
        Card* dealtCard = cards.back();
        cards.pop_back();
        return dealtCard;
    }

    void insertCard(Card* newCard){
        cards.push_back(newCard);
        this->shuffle();
    }
    ~Deck(){
        for(Card* card: cards){
            delete card;
        }
    }
    friend std::ostream& operator<<(std::ostream& os, const Deck& deck);

private:
    std::vector<Card*> cards;
};

std::ostream& operator<<(std::ostream& os, const Deck& deck) {
    for (const Card* card : deck.cards) {
        os << *card << '\n';
    }
    return os;
}
```

Up till now I've just been using what I've covered. Now we're going to make a Game class. There are many types of card games that all have sort of similar rules. For that reason this Game class will be an abstract class. It will have a purely virtual play method because different games can be played in different ways.

```c++
class Game {
public:
    Game() : deck(nullptr) {};

    virtual ~Game() { delete deck; }

    virtual void play() = 0;

    void chooseDeck (Deck* deck){
        this->deck = deck;
        deck->shuffle();
    }

protected:
    Deck* deck;
};
```

No matter the card game we'll probably want some or all of these methods. This is an abstract class and can't be instantiated. The reason for this is the line ```virtual void play() = 0;``` which creates the purely virtual function play. Since there is no default way to play a game, we can't create this class directly; we must inherit and override. I'm going to inherit these methods with 2 different types of games, Blackjack and Poker.

```c++
class Poker : public Game {
public:
    Poker() : Game() {}

    void play() override {
        if (this->deck == nullptr) throw std::out_of_range("No Deck...");
        std::cout << "Playing Poker..." << std::endl;
    }

};

class BlackJack : public Game {
public:
    BlackJack() : Game() {}

    void play() override {
        if (this->deck == nullptr) throw std::out_of_range("No Deck...");
        std::cout << "Playing BlackJack..." << std::endl;
    }

};
```

These classes can use all the methods of the abstract class and override the purely virtual play method. Overriding play is a classic example of polymorphism, allowing objects of different classes to be treated as objects of a base class while still executing their own versions of the method. Polymorphism is very useful, especially when setting up something like the Casino class, which will take all types of Game objects. 

```c++
class Casino {
public:
    Casino(){
        for (int i = 0; i < 100; ++i) {
            decks.emplace_back(new Deck);
        }
    }

    void startgame(Game& game){
        game.chooseDeck(decks.back());
        decks.pop_back();
    };

    virtual ~Casino() {
        for (Deck* deck: decks) {
            delete deck;
        }
    }

private:
    std::vector<Deck*> decks;
};

int main() {
    Casino vegas;
    Poker losing;
    BlackJack money;
    // losing.play(); //will throw errors
    // money.play();
    // Game emptygame; //Abstract class cannot be instantiated
    vegas.startgame(losing);
    vegas.startgame(money);
    losing.play();
    money.play();
    return 0;
}
```

That covers the basic principles of OOP in C++. We could add more depth to the BlackJack and Poker classes, but that won't add anything OOP-wise. There are a few other topics to cover (vtables, vptrs, etc) which will be discussed in another Markdown file. Below are some of the terms used in this file and a little more depth about them:

**Protected**: Data members of a class that are labeled protected can be accessed by inherited classes and friends.

**Virtual**: Normally, C++ matches a function call with its definition at compile time. This is called static binding. Specifying virtual ensures that this is done in runtime instead. This is called dynamic binding. This is why doSomething printed 2 instead of 3 when we added the virtual keyword because it got dynamically linked Advanced::doSomething instead of Basic::doSomething even though it appeared to be a Basic pointer.

**Purely Virtual**: The presence of these functions make a class abstract. Inherited classes must override this method if they want to be instantiated.

**Abstract Classes**: These are classes that are designed specifically to be base classes and represent concepts (Game, Animal, Shape). They have one or more purely virtual functions. In our case, there is no way to play a Game. There are, however, ways to play Poker, Blackjack, and any other inherited classes.

**Polymorphism**: The examples shown were example of runtime polymorphism using method overriding. There are other forms of polymorphism but they aren't OOP related. These are function overloading and method overloading which can be seen below.

```c++
//function overloading
void func(int a){
    std::cout << a << std::endl;
}

void func(int a, int b){ //same name different parameters
    std::cout << a + b << std::endl;
}

int main() {
    func(1);
    func(1, 2);
}
```

```c++
//method overloading
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
```
