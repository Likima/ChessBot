#ifndef VALIDMOVECHECK_H
#define VALIDMOVECHECK

#include "piececlass.h"
#include "boardclass.h"

bool moveIsValid(std::string move, ChessBoard &board, int moveNumber, std::shared_ptr<King> kingPtr)
{
    int prevX, prevY;
    RowType possiblePiece;
    std::vector<RowType> b = board.getBoard();

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
        else
        {
            std::cout << "You Cannot Castle! " << std::endl;
            return true;
        }
        return false;
    }
    else possiblePiece = getPieces(board, move, moveNumber);
    if (possiblePiece.size() > 1 && move.size() == 4)
    {
        for (int x = possiblePiece.size() - 1; x > -1; x--)
        {
            if (isdigit(move[1]))
            {
                if (possiblePiece[x]->getY() == move[1])
                    return false;
                possiblePiece.erase(possiblePiece.begin() + x);
            }
            else
            {
                if (possiblePiece[x]->getX() == move[1] - 96)
                    return false;
                possiblePiece.erase(possiblePiece.begin() + x);
            }
        }
    }
    if (possiblePiece.empty() || possiblePiece.size() > 1)
    {
        std::cout << "Move is not possible!" << std::endl;
        return false;
    }

    prevX = (possiblePiece[0]->getX());
    prevY = (possiblePiece[0]->getY());

    doMove(move, board, moveNumber, possiblePiece[0]);

    if (movingToCheck(board, move, moveNumber % 2, possiblePiece[0]))
    {
        std::cout << "You are in check!" << std::endl;
        board.setPiece(prevX - 1, prevY, possiblePiece[0]);
        return false;
    }
    if(possiblePiece[0]->getSymbol() == 'P' && (possiblePiece[0]->getY() == 1 || possiblePiece[0]->getY() == 8)){
        Promote(board, possiblePiece[0]);
    }
    if(possiblePiece[0]->getFirstMove() == true){
        possiblePiece[0]->setFirstMove();
    }
    board.setMoves(possiblePiece[0]);
    return true;
}
bool mated(ChessBoard &board, int color)
{
    std::vector<int> kingPos = board.findKing(color);
    std::shared_ptr<Piece> piecePtr = board[kingPos[1]][kingPos[0]];
    std::shared_ptr<King> kingPtr = std::dynamic_pointer_cast<King>(piecePtr);
    int prevX, prevY;

    if (!kingPtr->getLegal(board.getBoard()).empty())
    {
        return false; 
    }

    const auto &b = board.getBoard();
    for (const auto &row : b)
    {
        for (const auto &piece : row)
        {
            if (piece && piece->getColor() == color)
            {
                for (const std::string &move : piece->getLegal(board.getBoard()))
                {
                    if (movingToCheck(board, move, color, piece) && !piece->legalMove(move, board.getBoard()))
                    {
                        continue;
                    }

                    prevX = piece->getX();
                    prevY = piece->getY();
                    doMove(move, board, color, piece);

                    std::vector<int> tempKingPos = board.findKing(color);
                    std::shared_ptr<Piece> tempPiecePtr = board[tempKingPos[1]][tempKingPos[0]];
                    std::shared_ptr<King> tempKingPtr = std::dynamic_pointer_cast<King>(tempPiecePtr);
                    if (!tempKingPtr->inCheck(move, board.getBoard()))
                    {
                        board.setPiece(prevX-1, prevY, piece);
                        return false; // King is not in checkmate
                    }
                    board.setPiece(prevX-1, prevY, piece);
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
    

    doMove(move, board, color, passedPiece);

    std::vector<int> kingPos = board.findKing(color);
    std::shared_ptr<Piece> piecePtr = board.getBoard()[kingPos[1]][kingPos[0]];
    std::shared_ptr<King> kingPtr = std::dynamic_pointer_cast<King>(piecePtr);

    std::string kingPosString = std::string(1, static_cast<char>(kingPos[0] + 97)) + std::to_string(8 - kingPos[1]);

    if(kingPtr->inCheck(kingPosString, board.getBoard())){
        board.setPiece(prevX-1, prevY, passedPiece);
    }
    board.setPiece(prevX-1,prevY,passedPiece);

    return !(kingPtr->inCheck(kingPosString, board.getBoard()));

}
#endif