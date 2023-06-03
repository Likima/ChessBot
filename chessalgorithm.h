#ifndef CHESSALGORITHM_H
#define CHESSALGORITHM_H

#include "piececlass.h"
#include "boardclass.h"


void moveChoice(ChessBoard &board, int color) {
    ChessBoard tempboard = board;
    RowType movablePieces;
    RowType possiblePiece;
    std::vector<std::string> legalMoves;
    std::random_device rd;
    std::shared_ptr<Piece> movingPiece;
    std::mt19937 eng(rd());
    std::string move;

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

    possiblePiece = getPieces(board, move, color);

    doMove(move, board, color, possiblePiece[0]);
    board.setMoves(possiblePiece[0]);
}

#endif