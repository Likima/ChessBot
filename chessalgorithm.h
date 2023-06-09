#ifndef CHESSALGORITHM_H
#define CHESSALGORITHM_H

#include "piececlass.h"



void moveChoice(ChessBoard &board, int color) {
    color == White ? std::cout<<"White's move"<<std::endl : std::cout<<"Black's move"<<std::endl;
    ChessBoard tempboard = board;
    RowType movablePieces, possiblePiece;
    std::vector<piecePair> legalMoves;
    std::random_device rd;
    std::shared_ptr<Piece> movingPiece;
    std::mt19937 eng(rd());
    piecePair move;

    std::pair<std::shared_ptr<Piece>, std::string> bestMove = std::make_pair(nullptr, "");

    std::vector<int> kingPos = board.findKing(color);
    std::shared_ptr<Piece> piecePtr;
    std::shared_ptr<King> kingPtr;
    std::string kingPosString = "K" + std::string(1, static_cast<char>(kingPos[0] + 97)) + std::to_string(8 - kingPos[1]);

    piecePtr = board[kingPos[1]][kingPos[0]];
    kingPtr = std::dynamic_pointer_cast<King>(piecePtr);

    if(kingPtr->canCastle("O-O", board.getBoard())){
        legalMoves[legalMoves.size()-1].first = "O-O";
        legalMoves[legalMoves.size()-1].second = kingPtr;
    }
    if(kingPtr->canCastle("O-O-O",board.getBoard())){
        legalMoves[legalMoves.size()-1].first = "O-O-O";
        legalMoves[legalMoves.size()-1].second = kingPtr;
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
    std::cout<<"check 2.5 "<<movablePieces.size()<<std::endl;

    for(auto& piece : movablePieces){
        for(auto& moves : piece->getLegal(board.getBoard())){
            if(!movingToCheck(board, moves.first, color, piece)){
                legalMoves.emplace_back(std::make_pair(moves.first, piece));
            }
        }
    }

    std::uniform_int_distribution<int> distr(1, legalMoves.size());
    int randomNumber = distr(eng);
    move = legalMoves[randomNumber-1];

    std::cout<<"check 3"<<std::endl;

    if(move.first == "O-O" || move.first == "O-O-O"){
        castle(move.first, board, color);
        return;
    }
    
    std::cout<<move.first<<std::endl;

    doMove(move.first, board, color, move.second);

    if(move.second->getSymbol() == 'P'){
        if(move.second->getY() == 1 || move.second->getY() == 8){
            move.first = move.first+"=Q";
            Promote(board, move.second, true);
        }
    }
    if(move.second->getFirstMove()){
        move.second->setFirstMove();
    }
    board.setMoves(move.first);
    board.playedMovePrint();
}

#endif