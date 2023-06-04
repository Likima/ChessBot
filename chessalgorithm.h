#ifndef CHESSALGORITHM_H
#define CHESSALGORITHM_H

#include "piececlass.h"
#include "boardclass.h"


void moveChoice(ChessBoard &board, int color) {
    ChessBoard tempboard = board;
    RowType movablePieces;
    RowType possiblePiece;
    bool ambiguous = false;
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

    possiblePiece = getPieces(board, move, color);


    if (possiblePiece.size() > 1)
    {
        std::cout<<"Ambiguous Move"<<std::endl;
        for(auto &row : board.getBoard()){
            for(auto &piece : row){
                if(piece->getSymbol() == possiblePiece[0]->getSymbol() && piece->getColor() == possiblePiece[0]->getColor()){
                    if(piece->getX() == possiblePiece[0]->getX() && piece->getY() == possiblePiece[0]->getY()){
                        continue;
                    }
                    if(piece->legalMove(move, board.getBoard())){
                        if(piece->getX() == possiblePiece[0]->getX()){
                            move.insert(1, std::to_string(possiblePiece[0]->getX()+96));
                            break;
                        }
                        else if(piece->getY() == possiblePiece[0]->getY()){
                            move.insert(1, std::to_string(possiblePiece[0]->getY()));
                            break;
                        }
                    }
                }
            }
        }
        for (int x = possiblePiece.size() - 1; x > -1; x--)
        {
            if (isdigit(move[1]))
            {
                if (possiblePiece[x]->getY() == move[1])
                    move.insert(1, std::to_string(possiblePiece[x]->getX()+96));
                    continue;
                possiblePiece.erase(possiblePiece.begin() + x);
            }
            else
            {
                if (std::to_string(possiblePiece[x]->getX()) == std::string(1, move[1] - 96))
                    move.insert(1, std::to_string(possiblePiece[x]->getX()+96));
                    continue;
                possiblePiece.erase(possiblePiece.begin() + x);
            }
        }
    }
    std::cout<<move<<std::endl;

    if(possiblePiece.size() == 2) std::cout<<"Ambiguous Move ERROR"<<std::endl;
    if(possiblePiece[0]->getSymbol() == 'P'){
        if(possiblePiece[0]->getX() == 1 || possiblePiece[0]->getX() == 8){
            move = move+"=Q";
            Promote(board, possiblePiece[0], true);
        }
    }


    doMove(move, board, color, possiblePiece[0]);
    if(possiblePiece[0]->getFirstMove()){
        possiblePiece[0]->setFirstMove();
    }
    board.setMoves(possiblePiece[0]);
    board.playedMovePrint();
}

#endif