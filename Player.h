//  Player.h
//  Project 3
//
//  Created by Delina Yetbarek on 5/22/23.

// Place your Player class definition here, as well as the definitions for the HumanPlayer, BadPlayer, and SmartPlayer classes.

#ifndef PLAYER_H
#define PLAYER_H

#include <string>
#include "Board.h"
#include "Game.h"




class Player
{
public:
    Player(std::string name);
    // Create a Player with the indicated name.
    
    std::string name() const;
    // Return the name of the player.
    
    virtual bool isInteractive() const;
    // Return false if the player is a computer player. Return true if the player is human. Most kinds of players will be computer players.
    
    virtual int chooseMove(const Board& b, Side s) const = 0;
    // Every concrete class derived from this class must implement this function so that if the player were to be playing side s and had to make a move given board b, the function returns the move the player would choose. If no move is possible, return −1.
    
    virtual ~Player();
    // Since this class is designed as a base class, it should have a virtual destructor.

private:
    std::string m_name;
};





class HumanPlayer : public Player
{
public:
    HumanPlayer(std::string name);
    virtual bool isInteractive() const;
    ~HumanPlayer();
    virtual int chooseMove(const Board& b, Side s) const;
};





class BadPlayer : public Player
{
public:
    BadPlayer(std::string name);
    ~BadPlayer();
    virtual int chooseMove(const Board& b, Side s) const;
};





class SmartPlayer : public Player
{
public:
    SmartPlayer(std::string name);
    ~SmartPlayer();
    virtual int chooseMove(const Board& b, Side s) const;

private:
    int evaluate(Side s, Board b) const; // turn is whose turn it is
    
    void chooseMove(Side s, Side turn, Board b, int depth, int& bestHole, int& value, JumpyTimer& time) const;
//    void chooseMove(Side s, Side turn, Board b, int depth, int& bestHole, int& value) const;


};


#endif /* PLAYER_H */
