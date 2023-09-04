#ifndef printStatements_h
#define printStatements_h
#include "piececlass.h"
#include "boardclass.h"


void printvector(const std::vector<piecePair> vec){
    for(int x = 0; x<vec.size(); x++){
        std::cout<<vec[x].first<<", ";
    }
    std::cout<<std::endl;
}

void printvector(const std::vector<std::string> vec)
{
    for (int x = 0; x < vec.size(); x++)
    {
        std::cout << vec[x] << " ";
    }
    std::cout << std::endl;
}

void printBoard(const ChessBoard &board)
{
    int rowNum = 8;
    
    const auto &b = board.getBoard();
    for (const auto &row : b)
    {
        std::cout << "\033[1;32m" << rowNum << " \033[0m";
        for (const auto &piece : row)
        {
            if (piece->getColor() == Black)
                std::cout << "\033[0;35m" << piece->getSymbol() << " \033[0m";
            else
                std::cout << "\033[0;36m" << piece->getSymbol() << " \033[0m";
        }
        rowNum--;
        
        std::cout << std::endl;
    }
    std::cout<<"  ";
    for(const auto& col : coords){
        std::cout << "\033[1;32m" << col << " \033[0m";
    }
    std::cout<<std::endl;
}

void printX(const ChessBoard& board){
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

void printY(const ChessBoard& board){
    const auto &b = board.getBoard();
    for (const auto &row : b)
    {
        for (const auto &piece : row)
        {
            if (piece->getColor() == Black)
                std::cout << "\033[0;30m" << piece->getY() << " \033[0m";
            else
                std::cout << "\033[0;37m" << piece->getY() << " \033[0m";
        }
        
        std::cout << std::endl;
    }
    printvector(board.getMoves());
}



#endif