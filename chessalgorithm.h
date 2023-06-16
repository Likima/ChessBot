#ifndef CHESSALGORITHM_H
#define CHESSALGORITHM_H

#include "piececlass.h"
#include "boardclass.h"

class ChessAlgorithm{
    public:
        ChessAlgorithm(int color) : color(color){};

    int getColor() const {return color;}
    
    int evaluateOpening(ChessBoard &board){
        int retEval = 0;
        for(auto& row : board.getBoard()){
            for(auto& piece : row){
                if(piece->getSymbol() == 'P'){
                    if(piece->getX() == 4 || piece->getX() == 5){
                        if(piece->getY() != 2 && piece->getY() != 7){
                            piece->getColor() == White ? retEval+=20 : retEval-=20;
                            //retEval+=20;
                        }
                    }
                    //retEval-=((piece->getColor() == White ? 1 : -1)*piece->getY());
                }
            }
        }
        
        return retEval;
    }

    int evaluateEndgame(ChessBoard& board){
        int totalEval = 0;
        for(auto& row : board.getBoard()){
            for(auto & piece : row){
                if(board.getKing(White)->inCheck(board.kingString(White), board.getBoard())){
                    if(piece->getColor() == White){
                        totalEval+=piece->positionalAdvantage();
                    }
                    else{
                        totalEval-=piece->positionalAdvantage();
                    }
                }
                else if(board.getKing(Black)->inCheck(board.kingString(Black), board.getBoard())){
                    if(piece->getColor() == Black){
                        totalEval+=piece->positionalAdvantage();
                    }
                    else{
                        totalEval-=piece->positionalAdvantage();
                    }
                }
                if(piece->getSymbol() == 'N' && piece->getFirstMove()){
                    color == White ? totalEval+=10 : totalEval-=10;
                }
                if(piece->getSymbol() == 'B' && piece->getFirstMove()){
                    color == White ? totalEval+=10 : totalEval-=10;
                }

            }
        }
        return totalEval;
    }

    int evaluatePos(ChessBoard& board){
        int miscEval = 0;
        int positionalEval;
        std::pair<int,int> materialPair = board.getMaterial();
        int whiteMaterial = materialPair.first;
        int blackMaterial = materialPair.second;
        analyzedPositions++;
        if(moveNum < 12){
            miscEval+=evaluateOpening(board);
        }
        //if(board.isEndgame(board)) miscEval+=evaluateEndgame();
        for(auto& row : board.getBoard()){
            for(auto& piece : row){
                if(piece->getColor() == White){
                    positionalEval += piece->positionalAdvantage();
                }
                else if(piece->getColor() == Black){
                    positionalEval -= piece->positionalAdvantage();
                }
            }
        }
            
        return whiteMaterial-blackMaterial+positionalEval+miscEval;
    }

    piecePair getBestMove(){
        return bestMove;
    }

    int alphaBeta(ChessBoard& board, int depth, int alpha, int beta, bool maximizingPlayer)
    {
        int color = maximizingPlayer ? White : Black;
        std::vector<std::pair<std::pair<int, int>, std::shared_ptr<Piece>>> prevCoords;
        std::vector<piecePair> legalMoves;
        std::shared_ptr<Piece> prevPiece;
        int prevX, prevY;

        if (board.findKing(color) == std::vector<int>{-1,-1}) return maximizingPlayer ? INT_MAX : INT_MIN;
        else if(mated(board, color, board.getKing(color))) return maximizingPlayer ? INT_MAX : INT_MIN;
        if (depth == 0) {
            return evaluatePos(board);
        }

        for (const auto& row : board.getBoard()) {
            for (const auto& piece : row) {
                if (piece->getSymbol() != '.' && piece->getColor() == color) {
                    for(auto& moves : piece->getLegal(board.getBoard())){
                        if(!movingToCheck(board, moves.first, color, piece)){
                            legalMoves.emplace_back(moves);
                        }
                    }
                }
            }
        }
        if(board.getKing(color)->canCastle("O-O", board.getBoard())){
            legalMoves.emplace_back(std::make_pair("O-O", board.getKing(color)));
        }
        if(board.getKing(color)->canCastle("O-O-O", board.getBoard())){
            legalMoves.emplace_back(std::make_pair("O-O-O", board.getKing(color)));
        }

        if(legalMoves.empty()){
            if(board.getKing(color)->inCheck(board.kingString(color), board.getBoard())) return maximizingPlayer ? INT_MIN : INT_MAX;
            else return 0;
        }

        if (maximizingPlayer)
        {
            int maxEval = INT_MIN;
            for (auto& move : legalMoves)
            {
                if(movingToCheck(board, move.first, color, move.second)) continue;
                prevPiece = board.findPiece(move.first);
                prevX = move.second->getX();
                prevY = move.second->getY();
                doMove(move.first, board, color, move.second);

                int eval = alphaBeta(board, depth - 1, alpha, beta, false);
                if(eval>maxEval){
                    maxEval = std::max(maxEval, eval);
                    if(depth == INITIAL_DEPTH) bestMove = move;
                }

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
                if(movingToCheck(board, move.first, color, move.second)) continue;
                prevPiece = board.findPiece(move.first);
                prevX = move.second->getX();
                prevY = move.second->getY();
                doMove(move.first, board, color, move.second);
            
                int eval = alphaBeta(board, depth - 1, alpha, beta, true);
                if(eval<minEval){
                    minEval = std::min(minEval, eval);
                    if(depth == INITIAL_DEPTH) bestMove = move;
                }                    

                beta = std::min(beta, eval);

                board.setPiece(prevX-1, prevY, move.second, prevPiece);
                //board.setPiece(move.second->getPos().first-1, move.second->getPos().second, move.second, prevPiece);

                if (beta <= alpha)
                    break;
            }
            return minEval;
        }
    }   

    void moveChoice(ChessBoard &board, int color, int moveNumber) {
        moveNum = moveNumber/2;
        color == White ? std::cout<<"White's move"<<std::endl : std::cout<<"Black's move"<<std::endl;
        RowType possiblePiece;
        std::vector<piecePair> legalMoves;
        std::random_device rd;
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
        std::cout<<alphaBeta(board, INITIAL_DEPTH, INT_MIN, INT_MAX, color == White)<<std::endl;
        move = bestMove;

        if(move.first == "O-O" || move.first == "O-O-O"){
            castle(move.first, board, color);
            move.second->setFirstMove();
            board.setMoves(move.first);
            board.playedMovePrint();
            return;
        }

        std::cout<<move.first<<std::endl;
        move.second->printInfo();
        std::cout<<"Analyzed Positions: "<<analyzedPositions<<std::endl; 
        std::cout<<"BEST MOVE: "<<bestMove.first<<std::endl;  

        doMove(move.first, board, color, move.second);

        if(move.second->getSymbol() == 'P'){
            if(move.second->getY() == 1 || move.second->getY() == 8){
                move.first = move.first+"=Q";
                Promote(board, move.second, color == White);
            }
        }
        if(move.second->getFirstMove()) move.second->setFirstMove();
        board.setMoves(move.first);
        board.playedMovePrint();
    }   


    private:
        int color;
        int moveNum;
        int analyzedPositions = 0;
        piecePair bestMove;
};

#endif