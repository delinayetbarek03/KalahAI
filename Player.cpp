//  Player.cpp
//  Project 3
//
//  Created by Delina Yetbarek on 5/22/23.

// Place your Player member function implementations here, as well as those for HumanPlayer, BadPlayer, and SmartPlayer.

#include "Player.h"
#include <iostream>




Player::Player(std::string name) : m_name(name) {}


std::string Player::name() const
{
    return m_name;
}


bool Player::isInteractive() const
{
    return false; // most kinds of players are computers (OVERRIDE in derived classes)
}

Player::~Player() {}




// HUMAN PLAYER

HumanPlayer::HumanPlayer(std::string name) : Player(name) {}
// A HumanPlayer chooses its move by prompting a person running the program for a move (reprompting if necessary until the person enters a valid hole number), and returning that choice. We will never test for a situation where the user doesn't enter an integer when prompted for a hole number. (The techniques for dealing with the issue completely correctly are a distraction to this project, and involve either a function like stoi or strtol, or the type istringstream.)

bool HumanPlayer::isInteractive() const
{
    return true;
}



int HumanPlayer::chooseMove(const Board& b, Side s) const
{
    if(b.beansInPlay(s) == 0) // NO BEANS IN EITHER
        return -1;
    
    int inp = 0;
    while( inp > b.holes() || inp < 1 || b.beans(s, inp) == 0 )
    {
        std::cout << "Select a hole, " + name() + ": ";
        std::cin >> inp;
        
        if( inp > 0 && inp <= b.holes() && b.beans(s,inp) != 0 )
            break;
    }

    return inp;
}


HumanPlayer::~HumanPlayer() {}



// BAD PLAYER

BadPlayer::BadPlayer(std::string name) : Player(name) {}
// A BadPlayer is a computer player that chooses an arbitrary valid move and returns that choice. "Arbitrary" can be what you like: leftmost, nearest to pot, fewest beans, random, etc.. The point of this class is to have an easy-to-implement class that at least plays legally.

int BadPlayer::chooseMove(const Board& b, Side s) const
{
    if(b.beansInPlay(s) == 0)
        return -1;

    int holes = b.holes();
    int move = -1;

    for(int i = 1; i <= holes; i++)
    {
        if( b.beans(s, i) > 0 ) // chose the first hole that has beans
        {
            move = i;
            break;
        }
    }

//    std::cerr << name() + " chooses hole " << move << std::endl;
    return move;

    
}

BadPlayer::~BadPlayer() {}




// SMART PLAYER

SmartPlayer::SmartPlayer(std::string name) : Player(name) {}

// calls the recurive chooseMove function to determine the best hole, and return the result of the best holee
int SmartPlayer::chooseMove(const Board& b, Side s) const
{
    JumpyTimer time(1000);
    
    int bestHole;
    int value;
    chooseMove(s, s, b, 0, bestHole, value, time);
//    chooseMove(s, b, 0, bestHole, value);
//    std::cerr << name() + " chooses hole " << bestHole << std::endl;
    return bestHole;
}




SmartPlayer::~SmartPlayer() {}





// this a heuristic function that evaluates how good a certain board state is in order to determine the best possible move a smart player should make. It checks once the game is over, if our side has more beans in our pot and in play on our side, then it's a winning state so should be rated with a very high number,  if  our side has less beans in our pot and in play on our side, then it's a losing state so should be rated with a very low number, and otherwise, it's a tie so return 0.
//  When the game is not over, it checks if you have a majority of the beans in your pot compared to the total number of beans in play, and rates this high since our side would be winning, low if our side has less than a majority of the total beans, and returns the difference from our pot and the opponents pot if neither is about to win to detemine how good that board state is
int SmartPlayer::evaluate(Side s, Board b) const
{

Side otherSide = opponent(s);

// GAME IS OVER WHEN NO MORE BEANS IN PLAY ON EITHER SIDE
if( ( b.beansInPlay(s) == 0 ) || ( b.beansInPlay(otherSide) == 0) ) // if the game is over bc no more beans on your turn
{
    // IF OUR SIDE HAS MORE BEANS
    if( b.beansInPlay(s) + b.beans(s, POT) > b.beansInPlay(otherSide) + b.beans(otherSide, POT) )
    {
        return 999999; // our side is the max
    }
    else if( b.beansInPlay(s) + b.beans(s, POT) < b.beansInPlay(otherSide) + b.beans(otherSide, POT) )    // IF OUR SIDE HAS LESS BEANS
    {
        return -999999;
    }
    else // it's a tie
    {
        return 0;
    }
}


// GAME IS NOT OVER, YOU HAVE A MAJORITY OF BEANS

if( b.beans(s, POT) > ( b.totalBeans() ) / 2  ) // you have the majority of the beans in your pot
{
    return 999999;
}
else if( b.beans(s, POT) < ( b.totalBeans() ) / 2  )
{
    return -999999;
}
else
{
    // else, return the difference between the pots, want to maximize difference between the beans for side s

    int difference = b.beans(s, POT) - b.beans(otherSide, POT);
    return difference;
}
    
}





// this is a helper function that returns the best possible move to make by utilizing recursion to determine the possible moves it can make, and consider the possible countermoves the opponent can make in order to determine the best hole to chose. The best hole will be rated the highest value from our evaluation function. It first checks if the hole and number of beans is valid and returns -1 if not. The depth of the recursion will be limited to 4 in order to return a choice in a reasonable time, so when we call this function we will initiialze the depth to 0 and increase it for subsequent calls for the countermoves. If the depth hits the base case, 4, then we will return and bestHole will be set to the hole with the greatest possible value out of all the moves we've seen thus far in the game tree.

void SmartPlayer::chooseMove(Side s, Side turn, Board b, int depth, int& bestHole, int& value, JumpyTimer& time) const // set depth to 0 beginning
{
    
if(b.beansInPlay(s) == 0 || b.holes() > 6 || b.holes() < 0 ) // NO BEANS IN EITHER
{
    bestHole = -1;
    value = evaluate(s, b);
    return;
}

if( depth >= 10 )
{
    bestHole = -1;
    value = evaluate(s, b);
    return;
}

if( time.elapsed() >= 4900 )
{
    bestHole = -1;
    value = evaluate(s, b);
    return;
}
    

int holes = b.holes();
Side endSide;
int endHole;
int h2;
int v2;

Side otherSide = opponent(s);
    
    
    if( s == turn ) // ON OUR SIDE
    {
        value = -999999;
        v2 = -999999;
    }
    else
    {
        value = 999999; // ON OPPONENTS SIDE
        v2 = 999999;
    }
    

Board cpyB = b; // going to alter the copy of the board

for( int i = 1; i <= holes ; i++ )
{
    if( b.beans(s,i) > 0 )
    {
        cpyB.sow(s, i, endSide, endHole); // if endhole is 0 the get another turn

//         while you keep landing in pot
        if( endHole == 0 )
        {
            chooseMove(s, turn, cpyB, depth, h2, v2, time); // if endhole is 0 the get another turn
        }

//         if ended in a capture
        if( cpyB.beans(s, endHole) == 1 &&  cpyB.beans(otherSide, endHole) > 0 )
        {
            cpyB.moveToPot(s, endHole, s); // move beans from souths hole to souths pot
            cpyB.moveToPot(otherSide, endHole, s); // move beans from norths hole to souths pot
        }

        chooseMove(otherSide, otherSide, cpyB, depth+1, h2, v2, time); // choose move for opponent with the hole
        
        
        
        // minimax
        if( s == turn ) // maximizer
        {
            if( v2 >= value )
            {
                value = v2;
                bestHole = i;
            }
        }

        else // minimizer
        {
            if( v2 <= value )
            {
                value = v2;
                bestHole = i;
            }

        }
    }
    
    cpyB = b;
    }
    
    return;
}











