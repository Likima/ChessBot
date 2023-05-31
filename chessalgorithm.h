#ifndef CHESSALGORITHM_H
#define CHESSALGORITHM_H

#include "piececlass.h"
#include "boardclass.h"

void moveChoice(ChessBoard &board, int color) {
    RowType movablePieces;
    std::random_device rd;
    std::mt19937 eng(rd());

    for (const auto& row : board.getBoard()) {
        for (auto& piece : row) {
            if (piece->getSymbol() != '.' && piece->getColor() == color) {
                if (!piece->getLegal(board.getBoard()).empty()){
                    //piece->printInfo();
                    movablePieces.emplace_back(piece);
                }
            }
        }
    }
    //std::cout<<movablePieces.size()<<std::endl;

    int minNumber = 1;
    int maxNumber = movablePieces.size();

    std::uniform_int_distribution<int> distr(minNumber, maxNumber);
    int randomNumber = distr(eng);

    std::uniform_int_distribution<int> distro(minNumber, movablePieces[randomNumber - 1]->getLegal(board.getBoard()).size());
    int randomMove = distro(eng);

    //std::cout << movablePieces[randomNumber - 1]->getLegal(board.getBoard())[randomMove - 1] << std::endl;

    doMove(movablePieces[randomNumber - 1]->getLegal(board.getBoard())[randomMove - 1], board, color, movablePieces[randomNumber - 1]);

    //return movablePieces[randomNumber - 1]->getLegal(board.getBoard())[randomMove - 1];
}


#endif