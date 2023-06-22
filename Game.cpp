//  Game.cpp
//  Project 3
//
//  Created by Delina Yetbarek on 5/22/23.

// Place your Game member function implementations here.

#include "Game.h"
#include "Side.h"
#include <iostream> // becasue Game::display, Game::move, Game::play write to COUT, Game::play uses CIN


Game::Game(const Board& b, Player* south, Player* north) : m_board(b), m_south(south), m_north(north), m_turn(SOUTH) {}





// display the game board such that it appears like:
/*
    Homer
 2  2  2
0         0
 2  2  2
    Marge
 */
void Game::display() const
{
    std::cout << "\t" +  m_north->name() << std::endl << "  ";
    
    for( int i = 1; i <= m_board.holes(); i++ ) // NORTHS HOLES
        std::cout << m_board.beans(NORTH, i) << "  ";
    
    std::cout << std::endl << m_board.beans(NORTH, 0); // NORTHS POT
    
    for( int i = 0; i < m_board.holes(); i++ )
    {
        std::cout << "   "; // right amount of spaces in between the pots
    }
    std::cout << " " << m_board.beans(SOUTH, 0) << std::endl << "  "; // SOUTHS POT

    
    for( int i = 1; i <= m_board.holes(); i++ ) // SOUTHS HOLES
        std::cout << m_board.beans(SOUTH, i) << "  ";
    
    std::cout << std::endl << "\t" + m_south->name() << std::endl;
}


// check the status of the game and set the variable over to true if the game is over once there's no more beans on your side becasue the game is over, if the number of beans in your pot is larger once there's no more beans in play, or once there's a tie. Also, set hasWinner to true accordingly once we determined if the game is over or not, and set the winner to the side that won if hasWinner is true
void Game::status(bool& over, bool& hasWinner, Side& winner) const
{
    
        if( m_board.beansInPlay(SOUTH) != 0 && m_board.beansInPlay(NORTH) != 0 )  // can still make a move
        {
            over = false;
            return;
        }
    
        if( m_turn == SOUTH && m_board.beansInPlay(SOUTH) == 0 )  // souths turn and south DOESNT have beans
        {
            over = true;
        }
    
        if( m_turn == SOUTH && m_board.beansInPlay(SOUTH) != 0 )  // souths turn and south HAS beans
        {
            over = false;
            return;
        }
    
        if( m_turn == NORTH && m_board.beansInPlay(NORTH) == 0 )  // norths turn and north DOESNT have beans
        {
            over = true;
        }
    
        if( m_turn == NORTH && m_board.beansInPlay(NORTH) != 0 )  // norths turn and north HAS beans
        {
            over = false;
            return;
        }
    
        if( m_board.beans(NORTH, POT) > m_board.beans(SOUTH, POT) ) // game is over, north wins
        {
            over = true;
            hasWinner = true;
            winner = NORTH;
            return;
        }
    
        if( m_board.beans(SOUTH, POT) > m_board.beans(NORTH, POT) ) // game is over, south wins
        {
            over = true;
            hasWinner = true;
            winner = SOUTH;
            return;
        }
    
        if( m_board.beans(NORTH, POT) ==  m_board.beans(SOUTH, POT) ) // TIE
        {
            over = true;
            hasWinner = false;
            return;
        }
    

    
}


// if you're on the north side and you cant make a move, then sweep all the beans from souths side into souths POT and return false. If you can make a move, sow it and keep calling move if you end up in your pot, only do captures if you land in a hole that didn't have beans prior and the opponents side HAS beans, so the same for the opposite side and return false. Do the same for the other side
bool Game::move(Side s)
{
    // Attempt to make a complete move for the player playing side s. "Complete" means that the player sows the seeds from a hole and takes any additional turns required or completes a capture. Whenever the player gets an additional turn, you should display the board so someone looking at the screen can follow what's happening. If the move can be completed, return true; if not, because the move is not yet completed but side s has no holes with beans to pick up and sow, sweep any beans in s's opponent's holes into that opponent's pot and return false.
    // call choosemove function for each side's player, might get -1 return false, sow according to move. Call respective choose move on north and south player
    // if no more beans in north, then game over so move all beans to opponents pot

    int holes = m_board.holes();
    Side endSide;
    int endHole;
    
    if( s == NORTH ) // side is NORTH
    {
        int moveHole = m_north->chooseMove(m_board, s);
        
        if( moveHole == -1 ) // CANT make a move
        {
            for( int i = 1; i <= holes; i++ ) // sweep all the beans from souths side into souths POT when north can't make a move
            {
                m_board.moveToPot(SOUTH, i, SOUTH);
            }
            return false;
        }
        
        else // CAN make a move
        {
            
            if( m_board.sow(s, moveHole, endSide, endHole) )  // sow, if land in pot, get another move
            {
                if( endHole == POT ) // get another move
                {
                    display();
                    std::cout << m_north->name() + " gets another turn." << std::endl;
                    move(s);
                }
            }
            
            // CAPTURING, only if the opponents side has beans
            
            if( beans(NORTH, endHole) == 1 &&  beans(SOUTH, endHole) > 0 )
            {
                m_board.moveToPot(NORTH, endHole, NORTH); // move beans from norths hole to norths pot
                m_board.moveToPot(SOUTH, endHole, NORTH); // move beans from souths hole to norths pot
            }
            
            return true;
        }
        
    }

    else // side is SOUTH
    {
        int moveHole = m_south->chooseMove(m_board, s); // make the move and store the hole
        if( moveHole == -1 ) // CANT make a move
        {
            for( int i = 1; i <= holes; i++ ) // sweep all the beans from norths side into norths POT when south can't make a move
            {
                m_board.moveToPot(NORTH, i, NORTH);
            }
            return false;
        }
        
        else // CAN make a move
        {
            if( m_board.sow(s, moveHole, endSide, endHole) )  // sow, if land in pot, get another move
            {
                if( endHole == POT ) // get another move
                {
                    display();
                    std::cout << m_south->name() + " gets another turn." << std::endl;
                    move(s);
                }
            }
            
            
            // CAPTURING, only if the opponents side has beans
            
            if( beans(SOUTH, endHole) == 1 &&  beans(NORTH, endHole) > 0 )
            {
                m_board.moveToPot(SOUTH, endHole, SOUTH); // move beans from souths hole to souths pot
                m_board.moveToPot(NORTH, endHole, SOUTH); // move beans from norths hole to souths pot
            }
            
            return true;
        }
    }

}



// play the game and if both players are not interactive, make it so that you must press enter in order to see the next move. Once the game is over, if there is a winner then sweep the beans into their respective pots, if there's a winner print the winner, and if it's a tie then print there's a tie
void Game::play()
{
    
    bool over = false;
    bool hasWinner = false;
    Side winner;
    status(over, hasWinner, winner);
    bool pressEnter = false;
    
    display(); // display the game
    

    // neither players are interactive
    if( !m_north->isInteractive() && !m_south->isInteractive() )
    {
        pressEnter = true; // you want to prompt the user to press enter
    }


    while(!over)
    {
        if(pressEnter)
        {
            std::cout << "Press ENTER to continue";
            std::cin.ignore(10000, '\n');
        }
        
        move(SOUTH);
        display();
        status(over, hasWinner, winner);

        
        if(pressEnter)
        {
            std::cout << "Press ENTER to continue";
            std::cin.ignore();
        }
        
        move(NORTH);
        display();
        status(over, hasWinner, winner);
    
    }
    
    if( m_board.beansInPlay(NORTH) == 0 || m_board.beansInPlay(SOUTH) == 0 ) // if there is a winner then sweep
    {
        for(int i = 1; i <= m_board.holes(); i++)
        {
            m_board.moveToPot(NORTH, i, NORTH);
            m_board.moveToPot(SOUTH, i, SOUTH);
        }
        display();
    }


    if(hasWinner)
    {
        if(winner == NORTH)
        {
            std::cout << "The winner is " + m_north->name() + "." << std::endl;
        }

        else // winner is SOUTH
        {
            std::cout << "The winner is " + m_south->name() + "." << std::endl;
        }
    }

    else if (!hasWinner)
    {
        std::cout << "The game resulted in a tie." << std::endl;
    }
    
    
}
   


int Game::beans(Side s, int hole) const
{
    return m_board.beans(s,hole);
}


