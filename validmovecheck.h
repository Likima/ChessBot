#ifndef VALIDMOVECHECK_H
#define VALIDMOVECHECK_H

#include "piececlass.h"
#include "boardclass.h"

bool moveIsValid(std::string move, ChessBoard &board, int moveNumber, std::shared_ptr<King> kingPtr)
{
    int prevX, prevY;
    RowType possiblePiece;
    std::vector<RowType> b = board.getBoard();
    std::shared_ptr<Piece> prevPiece = board.findPiece(move);

    if (move.length() < 2)
    {
        std::cout << "Move is not possible!" << std::endl;
        return false;
    }
    else if (move == "O-O" || move == "O-O-O")
    {
        if (kingPtr->canCastle(move, board.getBoard()))
        {
            std::cout << "Castled" << std::endl;
            castle(move, board, moveNumber % 2);
            moveNumber++;
            return true;
        }
        std::cout<<"You cannot castle"<<std::endl;
        return false;
    }
    else possiblePiece = getPieces(board.getBoard(), move, moveNumber%2);
    std::cout<<move<<std::endl;
    std::cout<<possiblePiece.size()<<std::endl;

    if (possiblePiece.size() > 1 && move.size() == 4)
    {
        for (int x = possiblePiece.size() - 1; x > -1; x--)
        {
            if (isdigit(move[1]))
            {
                if (possiblePiece[x]->getY() == move[1])
                    return true;
                possiblePiece.erase(possiblePiece.begin() + x);
            }
            else
            {
                if (std::to_string(possiblePiece[x]->getX()) == std::string(1, move[1] - 96))
                    return true;
                possiblePiece.erase(possiblePiece.begin() + x);
            }
        }
    }
    if (possiblePiece.empty() || possiblePiece.size() > 1)
    {
        for(auto& piece : possiblePiece){
            piece->printInfo();
        }
        std::cout << "Move is not possible!" << std::endl;
        return false;
    }

    prevX = (possiblePiece[0]->getX());
    prevY = (possiblePiece[0]->getY());

    doMove(move, board, moveNumber, possiblePiece[0]);

    if (movingToCheck(board, move, moveNumber % 2, possiblePiece[0]))
    {
        std::cout << "You are in check!" << std::endl;
        board.setPiece(prevX - 1, prevY, possiblePiece[0], prevPiece);
        return false;
    }
    if(possiblePiece[0]->getSymbol() == 'P' && (possiblePiece[0]->getY() == 1 || possiblePiece[0]->getY() == 8)){
        Promote(board, possiblePiece[0], false);
        board.setMoves(move+"="+std::string(1,board[8-possiblePiece[0]->getY()][possiblePiece[0]->getX()-1]->getSymbol()));
        return true;
    }

    if(possiblePiece[0]->getFirstMove() == true) possiblePiece[0]->setFirstMove();

    board.setMoves(move);
    return true;
}
bool mated(ChessBoard &board, int color, std::shared_ptr<King> kingPtr)
{
    std::shared_ptr<Piece> prevPiece;
    std::vector<int> kingPos;
    std::shared_ptr<Piece> piecePtr;
    std::shared_ptr<King> kingPtrTwo;
    int prevX, prevY;
    //std::cout<<kingPtr->getLegal(board.getBoard()).empty()<<std::endl;
    for(auto &piece : kingPtr->getLegal(board.getBoard())){
        std::cout<<piece.first<<std::endl;
        if(!movingToCheck(board, piece.first, color, kingPtr)){
            return false;
        }
    }

    std::cout<<"here"<<std::endl;
    const auto &b = board.getBoard();
    for (const auto &row : b)
    {
        for (const auto &piece : row)
        {
            if (piece && piece->getColor() == kingPtr->getColor())
            {
                for (const auto &move : piece->getLegal(board.getBoard()))
                {
                    if (movingToCheck(board, move.first, color, piece))
                    {
                        continue;
                    } else {
                        return false;
                    }
                }
            }
        }
    }

    return true; // King is in checkmate
}


bool movingToCheck(ChessBoard& board, std::string move, int color, std::shared_ptr<Piece> passedPiece)
{
    int prevX = passedPiece->getX();
    int prevY = passedPiece->getY();

    std::shared_ptr<Piece> prevPiece = board.findPiece(move);
    
    doMove(move, board, color, passedPiece);

    std::vector<int> kingPos = board.findKing(color);
    std::shared_ptr<Piece> piecePtr = board[kingPos[1]][kingPos[0]];
    std::shared_ptr<King> kingPtr = std::dynamic_pointer_cast<King>(piecePtr);

    std::string kingPosString = "K"+std::string(1, static_cast<char>(kingPos[0] + 97)) + std::to_string(8 - kingPos[1]);
    if(kingPtr->inCheck(kingPosString, board.getBoard())){
        board.setPiece(prevX-1, prevY, passedPiece, prevPiece);
        return false;
    }
    board.setPiece(prevX-1, prevY, passedPiece, prevPiece);
    return true;

}
#endif