//  Board.cpp
//  Project 3
//
//  Created by Delina Yetbarek on 5/22/23.

// Place your Board member function implementations here.

#include "Board.h"

// initialize a board with the number of holes and beans specified, or 1 hole and 0 beans otherwise
Board::Board(int nHoles, int nInitialBeansPerHole)
{
    if( nHoles < 0 )
        nHoles = 1;
    
    if( nInitialBeansPerHole < 0 )
        nInitialBeansPerHole = 0;
    
    m_nHoles = nHoles;
    m_nBeans = nInitialBeansPerHole;
    northVec.push_back(0); // pots are empty
    southVec.push_back(0);
    
    
    // constructing north side
    for(int i = 1; i <= m_nHoles; i++)
        northVec.push_back(m_nBeans);

    //construct south side
    for(int i = 1; i <= m_nHoles; i++)
        southVec.push_back(m_nBeans);
}


int Board::holes() const
{
    return m_nHoles;
}


// return the number of beans in a side and hole
int Board::beans(Side s, int hole) const
{
    
    if(s == SOUTH && hole <= m_nHoles) 
    {
        return southVec.at(hole);
    }
    
    else if(s == NORTH && hole <= m_nHoles)
    {
        return northVec.at(hole);
    }
    
    else
        return -1; //if side and hole aren invalid, return -1
}


// return the total number of beans in all the holes not including the pot
int Board::beansInPlay(Side s) const
{
    if( !validSide(s) )
        return -1;
    
    int count = 0;
    
    if( s == NORTH )
    {
        for(int i = 1; i <= m_nHoles; i++) // count the number of beans in all the holes EXCLUDING the pot so start at 1
            count += northVec[i];
    }
    
    else // it's the south side
    {
        for(int i = 1; i <= m_nHoles; i++)
            count += southVec[i];
    }
    
    return count;
}

// return the total number of beans
int Board::totalBeans() const
{
    int total = 0;
    
    for(int i = 0; i <= m_nHoles; i++) // now we include the pot, so vector starts at "hole" 0
    {
        total += northVec[i] + southVec[i];
    }
    
    return total;
}


// sow the beans from a particular side and hole, and set the endSide to the final side the last bean was placed and endHole to the final hole the last bean was placed in
bool Board::sow(Side s, int hole, Side& endSide, int& endHole)
{
    if( !validSide(s) || !validHole(hole) || hole == POT || beans(s,hole) == 0 ) // if side or hole is invalid, hole is a pot, or hole is empty
        return false;
    
    int beans = northVec[hole];
    
    
    if( s == NORTH ) // if we start on north side
    {
        northVec[hole] = 0; // set beans at hole to zero
        int pos = hole - 1; // PLACE BEANS IN HOLE TO LEFT WHEN ON NORTH SIDE
        
        while(beans != 0) // while loop for looping through the board
        {
            for(int i = pos; i >= 0 && beans != 0; i--) // backwards while loop for north
            {
                northVec.at(i)++;
                beans--;
                if(beans == 0)
                {
                    endSide = NORTH;
                    endHole = i;
                    return true;
                }
            }
            pos = m_nHoles; // set the start for next loop to be the last hole
            for(int i = 1; i < southVec.size() ; i++) // loop through south from first hole to last
            {
                southVec.at(i)++;
                beans--;
                if(beans == 0)
                {
                    endSide = SOUTH;
                    endHole = i;
                    return true;
                }
            }
        }
        return true;
    }
    
    
    else // if start side is SOUTH
    {
        beans = southVec.at(hole); // keeping track of amount of beans
        
        southVec.at(hole) = 0; // make starting hole beans = 0
        
        int pos = hole + 1; // starting position for the first loop is one past the chosen hole bc CCW
        
        while(beans != 0) // while loop to loop through the board
        {
            for(int i = pos; i < southVec.size() && beans != 0; i++) // loop through south
            {
                southVec.at(i)++;
                beans--;
                if(beans == 0)
                {
                    endSide = SOUTH;
                    endHole = i;
                    return true;
                }
            }
            
            pos = 1; // set starting position for next loop to 1
            if(beans != 0) // add to south's pot
            {
                southVec.at(0)++;
                beans--;
            }
            
            if(beans == 0)
            {
                endSide = SOUTH;
                endHole = 0;
                return true;
            }
            
            for(int i = m_nHoles; i > 0; i--) // loop backwards through north
            {
                northVec.at(i)++;
                beans--;
                
                if(beans == 0)
                {
                    endSide = NORTH;
                    endHole = i;
                    return true;
                }
            }
        }
        
        return true;
    }
}



// move the beans to the correct pot
bool Board::moveToPot(Side s, int hole, Side potOwner)
{
    if( !validSide(s) || !validHole(hole) || hole == POT )
        return false;
    
    int beans;
    if( s == SOUTH )
    {
        beans = southVec.at(hole); // get number of beans at the hole
        southVec.at(hole) = 0;
    }
    
    else // it's the north side
    {
        beans = northVec.at(hole);
        northVec.at(hole) = 0;
    }
    
    if( potOwner == SOUTH )
    {
        southVec.at(0) += beans;
        return true;
    }
    
    else // it's north's pot
    {
        northVec.at(0) += beans;
        return true;
    }
}

// set the number of beans in a given side and hole, or return false if invalid number of beans, holes, or side
bool Board::setBeans(Side s, int hole, int beans)
{
    if( !validSide(s) || hole < 0 || hole > m_nHoles|| beans < 0 )
        return false;

    if( s == SOUTH )
        southVec.at(hole) = beans;

    if( s == NORTH )
        northVec.at(hole) = beans;

    return true;
}


// check if hole is valid (not including the pot), and return true if hole is within valid bounds, otherwise return false as it's out of bounds
bool Board::validHole(int hole) const // not include POT
{
    for (int i = 1; i <= m_nHoles; i++)
    {
        if ( hole == i )
            return true;
    }
    return false;
}


// check if the side is valid and return true, otherwise if invalid, return false
bool Board::validSide(Side s) const
{
    if( s == SOUTH || s == NORTH )
        return true;
    
    else {  return false; }
}












    











    
