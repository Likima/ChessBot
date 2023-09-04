#ifndef CHESSALGORITHM_H
#define CHESSALGORITHM_H

#include "piececlass.h"
#include "boardclass.h"

class ChessAlgorithm{
    public:
        ChessAlgorithm(int color) : color(color){};

    int getColor() const {return color;}
    void setDepth(int depth){this->INITIAL_DEPTH = depth;}
    
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
                    if(piece->getSymbol() == 'Q'){
                        if(!piece->getFirstMove()){
                            piece->getColor() == White ? retEval-=50 : retEval+=50;
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
                        totalEval+=piece->positionalAdvantage(board.getBoard());
                    }
                    else{
                        totalEval-=piece->positionalAdvantage(board.getBoard());
                    }
                }
                else if(board.getKing(Black)->inCheck(board.kingString(Black), board.getBoard())){
                    if(piece->getColor() == Black){
                        totalEval+=piece->positionalAdvantage(board.getBoard());
                    }
                    else{
                        totalEval-=piece->positionalAdvantage(board.getBoard());
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
        if(board.isEndgame()) INITIAL_DEPTH = 8;
        for(auto& row : board.getBoard()){
            for(auto& piece : row){
                if(piece->getColor() == White){
                    positionalEval += piece->positionalAdvantage(board.getBoard());
                }
                else if(piece->getColor() == Black){
                    positionalEval -= piece->positionalAdvantage(board.getBoard());
                }
            }
        }
            
        return whiteMaterial-blackMaterial+(positionalEval+miscEval)/2;
    }

    piecePair getBestMove(){
        return bestMove;
    }

    int alphaBeta(ChessBoard& board, int depth, int alpha, int beta, bool maximizingPlayer)
    {
        int color = maximizingPlayer ? White : Black;

        if(mated(board, color, board.getKing(color))){
            return maximizingPlayer ? INT_MAX : INT_MIN;
        }

        
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
        //if(board.getKing(color)->canCastle("O-O", board.getBoard())){
        //    legalMoves.emplace_back(std::make_pair("O-O", board.getKing(color)));
        //}
        //if(board.getKing(color)->canCastle("O-O-O", board.getBoard())){
        //    legalMoves.emplace_back(std::make_pair("O-O-O", board.getKing(color)));
        //}

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
                else board.setPiece(prevX-1, prevY, move.second, prevPiece);

                if (beta <= alpha){
                    break;
                }
                alpha = std::max(alpha, eval);

            }
            //bestLine.emplace_back(bestMove);
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
                    if(depth == DEPTH){
                        bestMove = move;
                    }
                }                    

                if(move.first == "O-O" || move.first == "O-O-O"){
                    board.revertCastle(color, move.first);
                }
                else board.setPiece(prevX-1, prevY, move.second, prevPiece);
                //board.setPiece(move.second->getPos().first-1, move.second->getPos().second, move.second, prevPiece);

                if (beta <= alpha){
                    break;
                }
                
                beta = std::min(beta, eval);

            }
            //bestLine.emplace_back(bestMove);
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
        if(board.isEndgame()) DEPTH = 7;
        if(moveNum > 20) DEPTH = 4;
        std::cout<<"Depth: "<<DEPTH<<std::endl;

        std::cout<<"ALPHABETA: "<<alphaBeta(board, DEPTH, INT_MIN, INT_MAX, color == White)<<std::endl;
        move = bestMove;
        std::cout<<"Best Line: ";
        for(auto& x : bestLine){
            std::cout<<x.first<<", ";
        }

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
        for(auto& move : bestLine){
            std::cout<<move.first<<", ";
        }

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
        int INITIAL_DEPTH = 4;
        int color;
        int DEPTH = 3;
        int moveNum;
        int analyzedPositions = 0;
        piecePair bestMove;
        std::vector<piecePair> bestLine;
        std::vector<piecePair> bestLine;
};

#endif