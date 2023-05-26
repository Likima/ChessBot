#include "piececlass.h"
#include "boardclass.h"


//make the pawn class more efficient. Refer to bishop class, implement something similar

//iterate over all the attacking pieces & pass the kings position as the move

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
    //ESC[background_colour;Text_colourm output ESC[m”
}

void doMove(const std::string& move, ChessBoard& board, int moveNumber, std::shared_ptr<Piece> passedPiece = NULL) {//rn only works for three letter things like ke5...
    //cout<<move[2]<<endl;
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
            std::cout<<horizontalInt<<", "<<verticalCoord<<std::endl;
            std::cout<<moved->getX()<<", "<<moved->getY()<<std::endl;
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
    std::string kingPosString;
    std::vector<int> kingPos;
    std::shared_ptr<Piece> piecePtr;
    std::shared_ptr<King> kingPtr;

    auto& b = board.getBoard();

    while(true) {


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
        if(move.length()<2 || move.length()>4){
            std::cout<<"Move is not possible!"<<std::endl;
            continue;

        } else {
            for (const auto& row : board.getBoard()){
                //std::cout<<std::endl;
                for (const auto& piece : row) {
                    if(piece->getSymbol() == toupper(move[0]) && piece->getSymbol() != 
                    'P' && piece->getColor() == moveNumber%2 && move.length() == 3 &&
                    piece->legalMove(move, moveNumber%2, board.getBoard()) == 1){
                        possiblePiece.emplace_back(piece);
                    }
                    //if(piece->getSymbol() == 'P') std::cout<<piece->legalMove(move, moveNumber%2, b)<<" ";
                    else if(piece->getSymbol() == 'P' && piece->getColor() == moveNumber%2 
                    && piece->legalMove(move, moveNumber%2, board.getBoard()) == 1 && pawnMove(move)){
                        possiblePiece.emplace_back(piece);
                    }
                }
            }            
        }

        for(int x = 0; x<possiblePiece.size(); x++){
            std::cout<<possiblePiece[x]->getSymbol()<<" ";
        } std::cout<<std::endl;

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

        kingPos = board.findKing(moveNumber%2);
        for(int x = 0; x<kingPos.size(); x++){
            std::cout<<kingPos[x]<<" ";
        } piecePtr = board[kingPos[1]][kingPos[0]];

        kingPosString = std::string(1, static_cast<char>(kingPos[0]+97))+std::to_string(8-kingPos[1]);

        //if(board.isChecked(moveNumber%2, kingPosString)) std::cout<<"You are in check!"<<std::endl;

        kingPtr = std::dynamic_pointer_cast<King>(piecePtr);

        std::cout<<"KingPosString: "<<kingPosString<<std::endl;

        kingPtr->printInfo();
        //kingPtr->printMoves();

        if (kingPtr != NULL && !kingPtr->inCheck(kingPosString, board.getBoard())){
            std::cout << "You are in check!" << std::endl;
            board.setPiece(prevX-1, prevY, possiblePiece[0]);
            possiblePiece.clear();
            //kingPtr->printInfo();
            continue;
        }

        kingPosString = "";
        std::cout<<std::endl;;
        possiblePiece.clear();
        printBoard(board);
        moveNumber++;
    }
    return 0;
}