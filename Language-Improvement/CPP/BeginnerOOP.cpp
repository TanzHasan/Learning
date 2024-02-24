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
    Deck newdeck;
    std::cout << newdeck << std::endl;
    Casino vegas;
    Poker losing;
    BlackJack money;
    // losing.play(); //will throw errors
    // money.play();
    // Game(); //Abstract class cannot be instantiated
    vegas.startgame(losing);
    vegas.startgame(money);
    losing.play();
    money.play();

    return 0;
}
