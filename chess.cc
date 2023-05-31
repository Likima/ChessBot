#include "piececlass.h"
#include "boardclass.h"
#include "chessalgorithm.h"

void printvector(std::vector<std::string> vec){
    for(int x = 0; x<vec.size(); x++){
        std::cout<<vec[x]<<" ";
    }
    std::cout<<std::endl;
}

bool mated(ChessBoard& board, int color){
    std::vector<int> kingPos = board.findKing(color);
    std::shared_ptr<Piece> piecePtr = board[kingPos[1]][kingPos[0]];
    std::shared_ptr<King> kingPtr = std::dynamic_pointer_cast<King>(piecePtr);

    if(kingPtr->getLegal(board.getBoard()).empty()) return true;

    return false;
}

bool pawnMove(std::string move){
    //std::cout<<"HERE!!!!!!!!"<<std::endl;
    if(move.length() == 2) return true;
    else if(move.length() == 4 && move.find('x') != std::string::npos) return true;
    return false;
}

void printBoard(const ChessBoard& board) {
    const auto& b = board.getBoard();
    for (const auto& row : b) {
        for (const auto& piece : row) {
            if(piece->getColor() == Black) std::cout<< "\033[0;01;02"<<"m"<< piece->getSymbol() << " "<<"\033[m";
            else std::cout << "\033[0;01"<<"m"<<piece->getSymbol() << " "<<"\033[m";
        }

        std::cout << std::endl;
    }
    printvector(board.getMoves());
}

void shortCastle(ChessBoard& board, int moveNumber){
    int n = 1;
    if(moveNumber == 0) n = 8;
    std::shared_ptr<King> king = std::dynamic_pointer_cast<King>(board.findPiece("-Ke"+std::to_string(n)));
    std::shared_ptr<Rook> rook = std::dynamic_pointer_cast<Rook>(board.findPiece("-Rh"+std::to_string(n)));
    board.setPiece(6,n,king);
    board.setPiece(5,n,rook);
}

void longCastle(ChessBoard& board, int moveNumber){
    int n = 1;
    if(moveNumber == 0) n = 8;
    std::shared_ptr<Piece> king = std::dynamic_pointer_cast<King>(board.findPiece("-Ke"+std::to_string(n)));
    std::shared_ptr<Piece> rook = std::dynamic_pointer_cast<Rook>(board.findPiece("-Ra"+std::to_string(n)));
    board.setPiece(2,n,king);
    board.setPiece(3,n,rook);
}

void castle(std::string move, ChessBoard& board, int moveNumber){
    if(move == "O-O") shortCastle(board, moveNumber);
    else if(move == "O-O-O") longCastle(board, moveNumber);
}

void doMove(const std::string& move, ChessBoard& board, int moveNumber, std::shared_ptr<Piece> passedPiece = NULL) {//clean up if have time
    
    int moveSize = move.length()-2;
    char m = move[0];
    char horizontalCoord = move[moveSize];
    int verticalCoord = (move[moveSize+1] - '0');
    int horizontalInt = 0;

    std::shared_ptr<Piece> moved;
    const auto& b = board.getBoard();

    if(passedPiece == NULL){
        for (const auto& row : b) {
            for (auto& piece : row) {
                if(piece && piece->getSymbol() == toupper(m) && piece->getColor() == moveNumber%2){
                    moved = piece;
                    break;
                }
            }
        }
    }else{
        moved = passedPiece;
        if(move.length() == 2){
            horizontalCoord = move[0];
            verticalCoord = move[1] - '0';
        }
    }
    

    for (std::vector<char>::iterator t = coords.begin(); t != coords.end(); ++t) {
        if(*t == horizontalCoord){
            board.setPiece(horizontalInt, verticalCoord, std::move(moved));
            break;
        }
        horizontalInt++;            
    }
}


int main() {
    std::string move;
    RowType possiblePiece;
    int moveNumber = 1;
    ChessBoard board;
    printBoard(board);
    int prevX, prevY;
    char BotSide;
    char promote;
    int BotMove;
    std::string kingPosString;
    std::vector<std::string> playedMoves;
    std::vector<int> kingPos;
    std::shared_ptr<Piece> piecePtr;
    std::shared_ptr<King> kingPtr;
    std::shared_ptr<Piece> nullpiece = std::make_shared<Piece>('.');

    std::vector<std::string> legalMoves;
    auto& b = board.getBoard();

    std::cout<<"What Side Are You Playing? [B/W] ";
    if(std::cin>>BotSide){
        if(BotSide == 'B') BotMove = 1;
        else if(BotSide == 'W') BotMove = 0;
        else {
            std::cout<<"Invalid Input"<<std::endl;
            return 0;
        }
    } else {
        std::cout<<"Invalid Input"<<std::endl;
        return 0;
    }

    while(true) {
        if(!playedMoves.empty()){
            //printvector(playedMoves);
            //std::cout<<", "<<std::endl;
        }
        kingPos = board.findKing(moveNumber%2);
        piecePtr = board[kingPos[1]][kingPos[0]];
        kingPtr = std::dynamic_pointer_cast<King>(piecePtr);
        kingPosString = "K"+ std::string(1, +static_cast<char>(kingPos[0]+97))+std::to_string(8-kingPos[1]);
        legalMoves = kingPtr->getLegal(board.getBoard());

        if(kingPtr != NULL && legalMoves.size() == 0 && !kingPtr->inCheck(kingPosString, board.getBoard())){
            std::cout<<"Checkmate!"<<std::endl;
            if(moveNumber%2 == 1){
                std::cout<<"White Wins!"<<std::endl;
            } else {
                std::cout<<"Black Wins!"<<std::endl;
            }
            break;
        }

        if(BotMove == moveNumber%2){
            //doMove(moveChoice(board, moveNumber%2), board, moveNumber%2);
            moveChoice(board, moveNumber%2);
            moveNumber++;
            printBoard(board);
            continue;
        }
        //else{
        //    moveChoice(board, moveNumber%2);
        //    moveNumber++;
        //    printBoard(board);
        //    continue;
        //}

        if(moveNumber%2 == 1){
            std::cout<<"White's Move"<<std::endl;
        } else {
            std::cout<< "\033[0;01;02"<<"m"<< "Black's Move" << " "<<"\033[m"<<std::endl;
        }
        std::cout<<std::endl;
        std::cout << "Enter move: ";
        std::cin >> move;
        if(move == "display"){
            printBoard(board);
            continue;
        }

        if(move[0] == '?' && move.length() == 4 && board.findPiece(move)){
            printvector(board.findPiece(move)->getLegal(board.getBoard()));
            if(board.findPiece(move)->getLegal(board.getBoard()).empty()) std::cout<<"No Legal Moves "<<std::endl;
            continue;
        } if(move[0] == '?'){
            std::cout<<"Not a valid piece "<<std::endl;
            continue;
        }
        if(move[0] == '!' && move.length() == 4 && board.findPiece(move)){
            board.setPiece((board.findPiece(move)->getX()-1),(board.findPiece(move)->getY()),nullpiece, 1);
            continue;
        }

        std::cout<<move.length()<<std::endl;

        if(move.length()<2){
            std::cout<<"Move is not possible!"<<std::endl;
            continue;

        } else if(move == "O-O" || move == "O-O-O"){
            if(kingPtr->canCastle(move, board.getBoard())){
                std::cout<<"Castled"<<std::endl;
                castle(move, board, moveNumber%2);
                moveNumber++;
                printBoard(board);
                continue;
            }
            else{
                std::cout<<"You Cannot Castle! "<<std::endl;
                continue;
            } 
        } else {
            for (const auto& row : board.getBoard()){
                //std::cout<<std::endl;
                for (const auto& piece : row) {
                    if(piece->getSymbol() == toupper(move[0]) && piece->getSymbol() != 
                    'P' && piece->getColor() == moveNumber%2 && move.length() == 3 &&
                    piece->legalMove(move, board.getBoard()) == 1){
                        possiblePiece.emplace_back(piece);
                    }
                    else if(piece->getSymbol() == 'P' && piece->getColor() == moveNumber%2 
                    && piece->legalMove(move, board.getBoard()) == 1 && pawnMove(move)){
                        possiblePiece.emplace_back(piece);
                        std::cout<<"Pawn X "<<piece->getX()<<std::endl;
                        if(moveNumber%2 == 1 && piece->getX() == 7){
                            while(true){
                                std::cout<<"Promote to: [Q/R/B/N]"<<std::endl;
                                std::cin>>promote;
                                if(promote == 'Q') {
                                    piece->setSymbol('Q');
                                    board.setPiece(piece->getX(), piece->getY(), std::dynamic_pointer_cast<Queen>(piece), 1);
                                }
                                else if(promote == 'R') {
                                    piece->setSymbol('R');
                                    board.setPiece(piece->getX(), piece->getY(), std::dynamic_pointer_cast<Rook>(piece), 1);
                                }
                                else if(promote == 'B') {
                                    piece->setSymbol('B');
                                    board.setPiece(piece->getX(), piece->getY(), std::dynamic_pointer_cast<Bishop>(piece), 1);
                                }
                                else if(promote == 'N') {
                                    piece->setSymbol('N');
                                    board.setPiece(piece->getX(), piece->getY(), std::dynamic_pointer_cast<Knight>(piece), 1);
                                }
                                else {
                                    std::cout<<"Invalid Input"<<std::endl;
                                    continue;
                                }
                                break;
                            }
                        }
                        else if(moveNumber%2 == 0 && piece->getX() == 2){
                            while(true){
                                std::cout<<"Promote to: [Q/R/B/N]"<<std::endl;
                                std::cin>>promote;
                                if(promote == 'Q') {
                                    piece->setSymbol('Q');
                                    board.setPiece(piece->getX(), piece->getY(), std::dynamic_pointer_cast<Queen>(piece), 1);
                                }
                                else if(promote == 'R') {
                                    piece->setSymbol('r');
                                    board.setPiece(piece->getX(), piece->getY(), std::dynamic_pointer_cast<Rook>(piece), 1);
                                }
                                else if(promote == 'B') {
                                    piece->setSymbol('b');
                                    board.setPiece(piece->getX(), piece->getY(), std::dynamic_pointer_cast<Bishop>(piece), 1);
                                }
                                else if(promote == 'N') {
                                    piece->setSymbol('n');
                                    board.setPiece(piece->getX(), piece->getY(), std::dynamic_pointer_cast<Knight>(piece), 1);
                                }
                                else {
                                    std::cout<<"Invalid Input"<<std::endl;
                                    continue;
                                }
                                break;
                            }
                        }
                    }
                }
            }            
        }


        if(possiblePiece.size() > 1 && move.size() == 4){
            for(int x = possiblePiece.size()-1; x>-1; x--){
                if(isdigit(move[1])){
                    std::cout<<"here! "<<std::endl;
                    if(possiblePiece[x]->getY() == move[1]) continue;
                    possiblePiece.erase(possiblePiece.begin()+x);
                }
                else{
                    std::cout<<possiblePiece[x]->getX()<<", "<<possiblePiece[x]->getY()<<std::endl;
                    if(possiblePiece[x]->getX() == move[1] - 96) continue;
                    possiblePiece.erase(possiblePiece.begin()+x);                
                }
                
            }
        }

        if(possiblePiece.empty() || possiblePiece.size()>1){
            std::cout<<"Move is not possible!"<<std::endl;
            possiblePiece.clear();
            continue;
        }
        prevX = (possiblePiece[0]->getX());
        prevY = (possiblePiece[0]->getY());


        doMove(move, board, moveNumber, possiblePiece[0]);

        if(possiblePiece[0]->getFirstMove()){possiblePiece[0]->setFirstMove();}

        kingPos = board.findKing(moveNumber%2);
        for(int x = 0; x<kingPos.size(); x++){
            std::cout<<kingPos[x]<<" ";
        } piecePtr = board[kingPos[1]][kingPos[0]];

        kingPosString = std::string(1, static_cast<char>(kingPos[0]+97))+std::to_string(8-kingPos[1]);

        kingPtr = std::dynamic_pointer_cast<King>(piecePtr);

        if (kingPtr != NULL && !kingPtr->inCheck(kingPosString, board.getBoard())){
            std::cout << "You are in check!" << std::endl;
            board.setPiece(prevX-1, prevY, possiblePiece[0]);
            possiblePiece.clear();
            continue;
        }

        //kingPosString = "";
        std::cout<<std::endl;;
        possiblePiece.clear();
        playedMoves.emplace_back(move);
        printBoard(board);
        moveNumber++;
    }
    return 0;
}