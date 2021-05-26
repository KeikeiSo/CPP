#include <iostream>
#include <map>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <time.h>
using namespace std;

// random seed
static int seed = time(nullptr);
// random generator function
int myrandom (int i) { return rand()%i;}

enum Rank {ACE = 1, TWO = 2, THREE = 3, FOUR = 4, FIVE = 5, 
SIX = 6, SEVEN = 7, EIGHT = 8, NINE = 9, TEN = 10, 
JACK = 11, QUEEN = 12, KING = 13};
enum Type {CLUBS = 'C', DIAMONDS = 'D', HEARTS = 'H', SPADES = 'S'};

class Card
{
public:
    //constructor
    Card() = default;
    Card(Rank r, Type t);
    ~Card() = default;

    //methods
    int getValue();
    void displayCard();
private:
    Rank rank;
    Type type;
};

// constructor of card
Card::Card(Rank r, Type t)
{
    this->rank = r;
    this->type = t;
}
// methods of card
int Card::getValue()
{
    // if the rank is Jack, Queen or King, return 10
    if (this->rank > 10) 
    {
        return 10;
    }
    return this->rank;
}

void Card::displayCard()
{
    // create a map that correspond rank to its string representation
    // I don't want to use switch because it is too long
    // even when using the conversion of int to string directly
    map<int, string> mr {{ACE, "1"}, {TWO, "2"}, {THREE, "3"}, {FOUR, "4"}, 
    {FIVE, "5"}, {SIX, "6"}, {SEVEN, "7"}, {EIGHT, "8"}, {NINE, "9"}, 
    {TEN, "10"}, {JACK, "J"}, {QUEEN, "Q"}, {KING, "K"}};

    // concatenate the rank's string with the type's char
    string s = mr[this->rank];
    s.push_back(this->type);
    cout << s << " ";
}


class Hand
{
public:
    // constructor and distructor
    Hand();
    ~Hand();

    // methods
    void add(Card c);
    void clear();
    int getTotal() const;
    void display();
protected:
    vector<Card> set;
};

Hand::Hand()
{
    this->set;
}

Hand::~Hand()
{
    // this line makes no different I keep it just in case.
    this->set.clear();
    this->set.~vector();
}

void Hand::add(Card c)
{
    // the push_back() is constant time
    this->set.push_back(c);
}

void Hand::clear()
{
    // I notice that they said clear change the vector capacity
    // so I use swap to force reallocation just to be safe
    vector<Card> empty;
    this->set.swap(empty);
    empty.clear();
    empty.~vector();
}

int Hand::getTotal() const
{
    // I used a boolean variable ace to see if ace in the set
    // If there is one and adding 10 to total would not bust,
    // I count the ace to be 11.
    int total = 0;
    bool ace = false;
    for (auto card : this->set)
    {
        if (card.getValue() == 1) ace = true;
        total += card.getValue();
    }
    if (total < 12 && ace) return total+10;
    return total;
}

void Hand::display()
{
    // I used an iterator to iterate through set
    for (std::vector<Card>::iterator it=this->set.begin(); it!=this->set.end(); ++it)
        (*it).displayCard();
    // also display the total value
    cout << "[" << this->getTotal() << "]" << endl;
}


class Deck: public Hand
{
public:
    //methods
    void Populate();
    void shuffle();
    void deal(Hand& h);
};

void Deck::Populate()
{
    Rank rs[13]= {Rank::ACE, Rank::TWO, Rank::THREE, Rank::FOUR, Rank::FIVE, 
    Rank::SIX, Rank::SEVEN, Rank::EIGHT, Rank::NINE, Rank::TEN, 
    Rank::JACK, Rank::QUEEN, Rank::KING};
    Type ts[4] = {Type::CLUBS, Type::DIAMONDS, Type::HEARTS, Type::SPADES};
    // empty the deck first
    this->clear();
    // make 52-cards deck that contain a card of each rank and type
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 13; j++)
        {
            Card c (rs[j], ts[i]);
            this->set.push_back(c);
        }
    }
}

void Deck::shuffle()
{
    
    // make sure the seed is different everytime
    seed ++;
    srand(seed);
    // use a function in the library
    std::random_shuffle (this->set.begin(), this->set.end(), myrandom);
}

void Deck::deal(Hand& h)
{
    if (this->set.empty()) {
        // display a warning message
        cout << "The deck is empty!" << endl;
        // populate a new deck
        cout << "A new deck is populated." << endl;
        this->Populate();
        this->shuffle();
    } 

    // get the card in the end of vector
    h.add(this->set.back());
    // delete the card in this deck
    this->set.pop_back();
    
}


class AbstractPlayer: public Hand
{
public:
    //methods
    virtual bool isDrawing() const = 0;
    bool isBusted();
};

bool AbstractPlayer::isBusted()
{
    // the player is busted if hand's total is over 21
    if (this->getTotal() > 21) return true;
    return false;
}

class ComputerPlayer:public AbstractPlayer
{
public:
    // methods
    bool isDrawing() const;
};

bool ComputerPlayer::isDrawing() const
{
    // computer will draw until its total is over 16
    return this->getTotal() > 16 ? false : true;
}


class HumanPlayer:public AbstractPlayer
{
public:
    //methods
    bool isDrawing() const;
    void announce(ComputerPlayer& casino);
};

bool HumanPlayer::isDrawing() const
{
    // if the total is already over 21, ignore the answer
    if (this->getTotal() >= 21) return false;
    // ask the player whether they want to draw
    char answer;
    cout << "Do you want to draw? (y/n): ";
    cin >> answer;
    // return the answer
    return answer == 'y'? true : false;
}

void HumanPlayer::announce(ComputerPlayer& casino)
{
    // first check if any player busted
    // if not the one with total closer to 21 wins
    if ((this->isBusted()) && (casino.isBusted())) 
        cout << "Push: No one wins." << endl;
    else if (this->isBusted())
    {
        cout << "Player busts." << endl;
        cout << "Casino wins." << endl;
    }
    else if (casino.isBusted())
    {
        cout << "Casino busts." << endl;
        cout << "Player wins." << endl;
    } else {
        if (casino.getTotal() > this->getTotal())
            cout << "Casino wins." << endl;
        else if (casino.getTotal() < this->getTotal())
            cout << "Player wins." << endl;
        else cout << "Push: No one wins." << endl;
    }
}


class BlackJackGame
{
public:
    BlackJackGame();
    //methods
    void play();
private:
    Deck m_deck;
    ComputerPlayer m_casino;
};

BlackJackGame::BlackJackGame()
{
    // I chose to populate and shuffle the deck when game is created
    // so that when the game continue, player may be able to guess
    // what cards are in the deck to increase win rate
    m_deck;
    m_deck.Populate();
    m_deck.shuffle();
    m_casino;
}

void BlackJackGame::play()
{
    // first deal one card to casino 
    // and two cards to the player
    // then display their cards
    HumanPlayer player;
    m_deck.deal(m_casino);
    cout << "Casino: ";
    m_casino.display();
    m_deck.deal(player);
    m_deck.deal(player);
    cout << "Player: ";
    player.display();
    // ask the player if they like to draw
    // drawing would continue as long as
    // their hand is not over 21
    while (player.isDrawing()) 
    {
        m_deck.deal(player);
        cout << "Player: ";
        player.display();
    }
    // casino would keep drawing until total is over 16
    while (m_casino.isDrawing())
    {
        m_deck.deal(m_casino);
        cout << "Casino: ";
        m_casino.display();
    }
    // announce whether it is a win, lose or draw
    player.announce(m_casino);
    // clear the casino because it is a state in game
    m_casino.clear();
}

int main()
{   
    /*
    // tester for card
    Card c (Rank::QUEEN, Type::SPADES);
    c.displayCard();
    cout << c.getValue() << endl;
    // tester for hand
    Hand h;
    h.add(c);
    cout << h.getTotal() << endl;
    h.add(c);
    cout << h.getTotal() << endl;
    h.clear();
    cout << h.getTotal() << endl;
    // tester for deck
    Deck d;
    d.Populate();
    cout << d.getTotal() << endl;
    d.shuffle();
    cout << d.getTotal() << endl;
    d.deal(h);
    cout << d.getTotal() << endl;
    cout << h.getTotal() << endl;
    */
    cout << "\tWelcome to the Comp322 Blackjack game!" << endl << endl;
    BlackJackGame game;
    // The main loop of the game
    bool playAgain = true;
    char answer = 'y';
    while (playAgain)
    {
        game.play();
        // Check whether the player would like to play another round
        cout << endl << endl;
        cout << "Would you like another round? (y/n): ";
        cin >> answer;
        playAgain = (answer == 'y' ? true : false);
    }
    cout <<"Gave over!";
    return 0;
}

