#ifndef CHESSALGORITHM_H
#define CHESSALGORITHM_H

#include "piececlass.h"
#include "boardclass.h"

int evaluatePos(ChessBoard& board){
    std::pair<int,int> materialPair = board.getMaterial();
    int whiteMaterial = materialPair.first;
    int blackMaterial = materialPair.second;
    return whiteMaterial-blackMaterial;
}

int alphaBeta(ChessBoard& board, int depth, int alpha, int beta, bool maximizingPlayer)
{
    std::vector<piecePair> legalMoves;
    std::shared_ptr<Piece> prevPiece;
    int prevX, prevY;
    if (depth == 0) return evaluatePos(board);

    for (const auto& row : board.getBoard()) {
        for (const auto& piece : row) {
            if (piece->getSymbol() != '.' && piece->getColor() == maximizingPlayer ? 1 : 0) {
                for(auto& moves : piece->getLegal(board.getBoard())){
                    if(!movingToCheck(board, moves.first, maximizingPlayer ? 1 : 0, piece)){
                        legalMoves.emplace_back(std::make_pair(moves.first, moves.second));
                    }
                }
            }
        }
    }

    if (maximizingPlayer)
    {
        int maxEval = INT_MIN;
        for (auto& move : legalMoves)
        {
            prevPiece = board.findPiece(move.first);
            prevX = board.findPiece(move.first)->getX();
            prevY = board.findPiece(move.first)->getY();
            doMove(move.first, board, maximizingPlayer ? 1 : 0, move.second);

            int eval = alphaBeta(board, depth - 1, alpha, beta, false);
            maxEval = std::max(maxEval, eval);
            alpha = std::max(alpha, eval);
            board.setPiece(prevX-1, prevY, move.second, prevPiece);
            
            if (beta <= alpha)
                break;
        }
        return maxEval;
    }
    else
    {
        int minEval = INT_MAX;
        for (auto& move : legalMoves)
        {
            prevPiece = board.findPiece(move.first);
            prevX = board.findPiece(move.first)->getX();
            prevY = board.findPiece(move.first)->getY();
            doMove(move.first, board, maximizingPlayer ? 1 : 0, move.second);

            int eval = alphaBeta(board, depth - 1, alpha, beta, true);
            minEval = std::min(minEval, eval);
            beta = std::min(beta, eval);

            board.setPiece(prevX-1, prevY, move.second, prevPiece);

            if (beta <= alpha)
                break;
        }
        return minEval;
    }
}

void moveChoice(ChessBoard &board, int color) {
    color == White ? std::cout<<"White's move"<<std::endl : std::cout<<"Black's move"<<std::endl;
    RowType movablePieces, possiblePiece;
    std::vector<piecePair> legalMoves;
    std::random_device rd;
    std::shared_ptr<Piece> movingPiece;
    std::mt19937 eng(rd());
    piecePair move;

    std::vector<int> kingPos = board.findKing(color);
    std::shared_ptr<Piece> piecePtr;
    std::shared_ptr<King> kingPtr;
    std::string kingPosString = "K" + std::string(1, static_cast<char>(kingPos[0] + 97)) + std::to_string(8 - kingPos[1]);

    piecePtr = board[kingPos[1]][kingPos[0]];

    kingPtr = std::dynamic_pointer_cast<King>(piecePtr);

    if(kingPtr == nullptr){
        std::cout<<"kingPtr is null"<<std::endl;
        return;
    }

    if(kingPtr->canCastle("O-O", board.getBoard())){
        legalMoves.emplace_back(std::make_pair("O-O", kingPtr));
    }

    if(kingPtr->canCastle("O-O-O",board.getBoard())){
        legalMoves.emplace_back(std::make_pair("O-O-O", kingPtr));
    }

    for (const auto& row : board.getBoard()) {
        for (const auto& piece : row) {
            if (piece->getSymbol() != '.' && piece->getColor() == color) {
                for(auto& moves : piece->getLegal(board.getBoard())){
                    if(!movingToCheck(board, moves.first, color, piece)){
                        legalMoves.emplace_back(std::make_pair(moves.first, moves.second));
                    }
                }
            }
        }
    }

    std::uniform_int_distribution<int> distr(1, legalMoves.size());
    int randomNumber = distr(eng);
    move = legalMoves[randomNumber-1];
    std::cout<<alphaBeta(board, 3, INT_MIN, INT_MAX, color == White)<<std::endl;

    if(move.first == "O-O" || move.first == "O-O-O"){
        castle(move.first, board, color);
        move.second->setFirstMove();
        board.setMoves(move.first);
        board.playedMovePrint();
        return;
    }
    
    std::cout<<move.first<<std::endl;
    move.second->printInfo();

    doMove(move.first, board, color, move.second);

    if(move.second->getSymbol() == 'P'){
        if(move.second->getY() == 1 || move.second->getY() == 8){
            move.first = move.first+"=Q";
            Promote(board, move.second, color == White);
        }
    }
    if(move.second->getFirstMove()){//may cause problems. not sure
        move.second->setFirstMove();
    }
    board.setMoves(move.first);
    board.playedMovePrint();
}

#endif