#ifndef SPECIALMOVES_H
#define SPECIALMOVES_H

#include "piececlass.h"
#include "boardclass.h"

void Promote(ChessBoard& board, std::shared_ptr<Piece> piece, bool botmove = false){
    int color = piece->getColor();
    char promote;
    while (true)
        {
            std::cout << "Promote to: [Q/R/B/N]" << std::endl;
            if(botmove == false) std::cin >> promote;
            else promote = 'Q';

            if (promote == 'Q')
            {
                board.setPiece(piece->getX()-1, piece->getY(), std::make_shared<Queen>(color, 'Q', piece->getX(), piece->getY(), 8));
            }
            else if (promote == 'R')
            {
                piece->setSymbol('R');
                board.setPiece(piece->getX(), piece->getY(), std::dynamic_pointer_cast<Rook>(piece));
            }
            else if (promote == 'B')
            {
                piece->setSymbol('B');
                board.setPiece(piece->getX(), piece->getY(), std::dynamic_pointer_cast<Bishop>(piece));
            }
            else if (promote == 'N')
            {
                piece->setSymbol('N');
                board.setPiece(piece->getX(), piece->getY(), std::dynamic_pointer_cast<Knight>(piece));
            }
            else
            {
                std::cout << "Invalid Input" << std::endl;
                continue;
            }
            break;
        }
}

void shortCastle(ChessBoard &board, int moveNumber)
{
    int n = 1;
    if (moveNumber == 0)
        n = 8;
    std::shared_ptr<King> king = std::dynamic_pointer_cast<King>(board.findPiece("-Ke" + std::to_string(n)));
    std::shared_ptr<Rook> rook = std::dynamic_pointer_cast<Rook>(board.findPiece("-Rh" + std::to_string(n)));
    board.setPiece(6, n, king);
    board.setPiece(5, n, rook);
}

void longCastle(ChessBoard &board, int moveNumber)
{
    int n = 1;
    if (moveNumber == 0)
        n = 8;
    std::shared_ptr<Piece> king = std::dynamic_pointer_cast<King>(board.findPiece("-Ke" + std::to_string(n)));
    std::shared_ptr<Piece> rook = std::dynamic_pointer_cast<Rook>(board.findPiece("-Ra" + std::to_string(n)));
    board.setPiece(2, n, king);
    board.setPiece(3, n, rook);
}

void castle(std::string move, ChessBoard &board, int moveNumber)
{
    if (move == "O-O")
        shortCastle(board, moveNumber);
    else if (move == "O-O-O")
        longCastle(board, moveNumber);
}

#endif