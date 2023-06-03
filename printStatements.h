#ifndef printStatements_h
#define printStatements_h
#include "piececlass.h"
#include "boardclass.h"


void printvector(std::vector<std::string> vec)
{
    for (int x = 0; x < vec.size(); x++)
    {
        std::cout << vec[x] << " ";
    }
    std::cout << std::endl;
}

void printBoard(const ChessBoard &board)
{
    const auto &b = board.getBoard();
    for (const auto &row : b)
    {
        for (const auto &piece : row)
        {
            if (piece->getColor() == Black)
                std::cout << "\033[0;01;02"
                          << "m" << piece->getSymbol() << " "
                          << "\033[m";
            else
                std::cout << "\033[0;01"
                          << "m" << piece->getSymbol() << " "
                          << "\033[m";
        }
        
        std::cout << std::endl;
    }
    printvector(board.getMoves());
}

void printX(ChessBoard& board){
    const auto &b = board.getBoard();
    for (const auto &row : b)
    {
        for (const auto &piece : row)
        {
            if (piece->getColor() == Black)
                std::cout << "\033[0;01;02"
                          << "m" << piece->getX() << " "
                          << "\033[m";
            else
                std::cout << "\033[0;01"
                          << "m" << piece->getX() << " "
                          << "\033[m";
        }
        
        std::cout << std::endl;
    }
}

void printY(ChessBoard& board){
    const auto &b = board.getBoard();
    for (const auto &row : b)
    {
        for (const auto &piece : row)
        {
            if (piece->getColor() == Black)
                std::cout << "\033[0;01;02"
                          << "m" << piece->getY() << " "
                          << "\033[m";
            else
                std::cout << "\033[0;01"
                          << "m" << piece->getY() << " "
                          << "\033[m";
        }
        
        std::cout << std::endl;
    }
    printvector(board.getMoves());
}



#endif