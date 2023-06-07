#ifndef CHESSALGORITHM_H
#define CHESSALGORITHM_H

#include "piececlass.h"



void moveChoice(ChessBoard &board, int color) {
    color == White ? std::cout<<"White's move"<<std::endl : std::cout<<"Black's move"<<std::endl;
    ChessBoard tempboard = board;
    RowType movablePieces, possiblePiece;
    std::vector<std::string> legalMoves;
    std::random_device rd;
    std::shared_ptr<Piece> movingPiece;
    std::mt19937 eng(rd());
    std::string move;

    std::vector<int> kingPos = board.findKing(color);
    std::shared_ptr<Piece> piecePtr;
    std::shared_ptr<King> kingPtr;
    std::string kingPosString = "K" + std::string(1, static_cast<char>(kingPos[0] + 97)) + std::to_string(8 - kingPos[1]);

    piecePtr = board[kingPos[1]][kingPos[0]];
    kingPtr = std::dynamic_pointer_cast<King>(piecePtr);

    if(kingPtr->canCastle("O-O", board.getBoard())){
        legalMoves.emplace_back("O-O");
    }
    if(kingPtr->canCastle("O-O-O",board.getBoard())){
        legalMoves.emplace_back("O-O-O");
    }

    for (const auto& row : board.getBoard()) {
        for (auto& piece : row) {
            if (piece->getSymbol() != '.' && piece->getColor() == color) {
                if (!piece->getLegal(board.getBoard()).empty()){
                    movablePieces.emplace_back(piece);
                }
            }
        }
    }

    for(auto& piece : movablePieces){
        for(auto& moves : piece->getLegal(board.getBoard())){
            if(!movingToCheck(board, moves, color, piece)){
                legalMoves.emplace_back(moves);
            }
        }
    }

    std::uniform_int_distribution<int> distr(1, legalMoves.size());
    int randomNumber = distr(eng);
    move = legalMoves[randomNumber-1];
    std::cout<<move<<std::endl;

    possiblePiece = getPieces(board.getBoard(), move, color);

    std::cout<<"check 4: "<<possiblePiece.size()<<std::endl;

    if (possiblePiece.size() > 1)
    {
        for (int x = possiblePiece.size() - 1; x > -1; x--)
        {
            if (isdigit(move[1]))
            {
                if (possiblePiece[x]->getY() == move[1]){
                    movingPiece = possiblePiece[x];
                    break;
                }
                possiblePiece.erase(possiblePiece.begin() + x);
            }
            else
            {
                if (std::to_string(possiblePiece[x]->getX()) == std::string(1, move[1] - 96)){
                    movingPiece = possiblePiece[x];
                    break;
                }
                possiblePiece.erase(possiblePiece.begin() + x);
            }
        }
    } else movingPiece = possiblePiece[0];
    
    std::cout<<move<<std::endl;
    std::cout<<possiblePiece.size()<<std::endl;

    doMove(move, board, color, movingPiece);
    if(possiblePiece[0]->getSymbol() == 'P'){
        if(movingPiece->getY() == 1 || movingPiece->getY() == 8){
            move = move+"=Q";
            Promote(board, movingPiece, true);
        }
    }
    if(movingPiece->getFirstMove()){
        movingPiece->setFirstMove();
    }
    board.setMoves(move);
    board.playedMovePrint();
}

#endif