#include "Game.h"
#include "Player.h"
#include "Board.h"
#include "Side.h"
#include <iostream>

int main()
{
    Board b(3,2);
    BadPlayer bp1("Marge");
    BadPlayer bp2("Homer");
    
    Game g(b, &bp1, &bp2);
    g.play();
}
