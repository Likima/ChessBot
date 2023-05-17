#include <iostream>
#include <vector>
#include <string>
#include <cstdio>
#include <cstring>
#include <ctype.h>
#include <algorithm>
#include <memory>

//make the pawn class more efficient. Refer to bishop class, implement something similar

//iterate over all the attacking pieces & pass the kings position as the move

class Piece;

std::vector<char> coords = {'a','b','c','d','e','f','g','h'};

using RowType = std::vector<std::shared_ptr<Piece>>;
int Black = 0;
int White = 1;

class Piece {
public:
    const static int BLACK = 0;
    const static int WHITE = 1;

    Piece(const Piece& other) : color(other.color), symbol(other.symbol), x(other.x), y(other.y){}

    Piece() {}
    Piece(int symbol) : symbol(symbol), color(-1){}
    Piece(int color, char symbol) : symbol(symbol), color(color){}
    Piece(int color, char symbol, int x, int y) : symbol(symbol), color(color), x(x), y(y){}

    virtual bool legalMove(std::string move, int color, std::vector<RowType> Board){return false;}
    virtual void movePiece(std::string move){};
    virtual ~Piece() {}
    
    int getColor() const {return color;}
    char getSymbol() const {return symbol;}
    int getX() const {return x;}
    int getY() const {return y;}

    void printInfo(){
        std::cout<<"x: "<<x<<", "<<"y: "<<y<<", "
        "COLOR: "<<color<<", "<<"SYMBOL: "<<symbol;
    }

    bool checkRook(std::string move, int color, std::vector<RowType> Board){
        int moveSize = move.length()-2;
        std::cout<<"("<<getX()<<", "<<(move[moveSize]-96)<<", "<<getY()<<", "<<(move[moveSize+1] - '0')<<")"<<std::endl;
        if(getX() == (move[moveSize]-96) && getY() == (move[moveSize+1]- '0')){//checking if moving to same square
            return false;
        }
        else if(getX() == (move[moveSize]-96)){//if the rook is travelling vertically
            movingY = move[moveSize+1]-'0';
            if(getY()<movingY){
                for(int i = getY()+1; i<movingY; i++){
                    std::cout<<"    ["<<8-i<<", "<<getX()-1<<"]"<<std::endl;
                    if(Board[8-i][getX()-1]->getSymbol() != '.') return false;
                }
                if(Board[8-movingY][getX()-1]->getColor() == getColor()) return false;
                return true;
            }
            else if(getY()>movingY){
                for(int i = movingY+1; i<getY(); i++){
                    if(Board[8-i][getX()-1]->getSymbol() != '.') return false;
                }
                if(Board[8-movingY][getX()-1]->getColor() == getColor()) return false;
                return true;
            }
        }
        else if(getY() == (move[moveSize+1]- '0')){//if the rook is travelling horizontally
            movingX = move[moveSize]-96;
            if(getX()<movingX){
                for(int i = getX()+1; i<movingX; i++){
                    if(Board[8-getY()][i-1]->getSymbol() != '.') return false;
                }
                if(Board[8-getY()][movingX-1]->getColor() == getColor()) return false;
                return true;                
            }
            else if(getX()>movingX){
                for(int i = movingX+1; i<getX(); i++){
                    if(Board[8-getY()][i-1]->getSymbol() != '.'){
                        return false;
                    }
                }
                if(Board[8-getY()][movingX-1]->getColor() == getColor()) return false;
                std::cout<<"HERE"<<std::endl;
                return true;                
            }
        }
        return false;
    }

    bool checkBishop(std::string move, int color, std::vector<RowType> Board){
        if(getX() == (move[1]-96) && getY() == (move[2]- '0')){//checking if moving to same square
            return false;
        }

        distX = abs(getX()-(move[1]-96));
        distY = abs(getY()-(move[2]-'0'));

        if(distX != distY) return false;
        
        for(int x = 1; x<distX; x++){
            incX = x;
            incY = x;
            
            if((getY())<(move[2]-'0')) incY=-incY;
            if(getX()>(move[1]-96)) incX=-incX;

            std::cout<<"["<<(8-getY()+incY)<<", "<<(getX()-1+incX)<<std::endl;
            if(7-getY()+incY == 8-getY() && getX()+incX == getX()-1) continue;
            if(Board[8-getY()+incY][getX()-1+incX]->getSymbol() != '.') return false;
        }
        if(Board[8-(move[2]-'0')][move[1]-97]->getColor() == getColor()) return false;
        return true;
    }
    
    void setColor(int color) {this->color = color;}
    void setSymbol(char symbol) {this->symbol = symbol;}
    void setX(int x){this->x = x;}
    void setY(int y){this->y = y;}

private:
    int color;
    int x;
    int y;
    char symbol;
    int distY;
    int distX;
    int incX;
    int incY;
    int movingX;
    int movingY;
};

class Pawn: public Piece{
    public:
    Pawn(char color, int symbol, int x, int y) : Piece(color, 'P', x, y){}

    bool legalMove(std::string move, int color, std::vector<RowType> Board) override{

        if(getX() == (move[0]-96) && getY() == (move[1]- '0')){//checking if moving to same square
            return false;
        }
        std::cout<<"("<<getX()<<", "<<(move[0]-97)<<", "<<getY()<<", "<<(move[1] - '0')<<")"<<std::endl;
        if(color == White){
            if(getX() == (move[0]-96) && getY()+1 == (move[1] - '0') && Board[7-getY()][getX()-1]->getSymbol() == '.'){
                this->firstMove = false;
                return true;
            }
            else if(firstMove && getX() == (move[0]-96) && getY()+2 == (move[1] - '0')&& Board[7-getY()][getX()-1]->getSymbol() == '.' && Board[6-getY()][getX()-1]->getSymbol() == '.'){
                this->firstMove = false;
                return true;
            }
            else if(getX() != 1 && getX()-1 == move[0]-96 && Board[7-getY()][getX()-2]->getSymbol() != '.'){
                if(Board[7-getY()][getX()-2]->getColor() == getColor()) return false;
                this->firstMove = false;
                return true;
            }
            else if(getX() != 8 && getX() == move[0]-96 && Board[7-getY()][getX()]->getSymbol() != '.'){
                if(Board[7-getY()][getX()]->getColor() == getColor()) return false;
                this->firstMove = false;
                return true;
            }
        }
        else if(color == Black){
            if(getX() == (move[0]-96) && getY()-1 == (move[1] - '0') && Board[9-getY()][getX()-1]->getSymbol() == '.'){
                this->firstMove = false;
                return true;
            }
            else if(firstMove && getX()-1 == (move[0]-97) && getY()-2 == (move[1] - '0')
            && Board[9-getY()][getX()-1]->getSymbol() == '.' && Board[10-getY()][getX()-1]->getSymbol() == '.'){
                this->firstMove = false;
                return true;
            }
            else if(getX() != 1 && getX()-1 == move[0]-96 && Board[9-getY()][getX()-2]->getSymbol() != '.'){
                if(Board[9-getY()][getX()-2]->getColor() == getColor()) return false;
                this->firstMove = false;
                return true;
            }
            else if(getX() != 8 && getX() == move[0]-96 && Board[9-getY()][getX()]->getSymbol() != '.'){
                if(Board[9-getY()][getX()]->getColor() == getColor()) return false;
                this->firstMove = false;
                return true;
            }
        }
        return false;
    }
    private:
        bool firstMove = true;
        bool enPassantable = false;
};

class Rook: public Piece{
    public:
    Rook(char color, int symbol, int x, int y) : Piece(color, 'R', x, y){}

    bool legalMove(std::string move, int color, std::vector<RowType> Board) override{
        return checkRook(move, color, Board);
    }
};

class Bishop: public Piece{
    public:
    Bishop(char color, int symbol, int x, int y) : Piece(color, 'B', x, y){}

    bool legalMove(std::string move, int color, std::vector<RowType> Board) override{
        return checkBishop(move, color, Board);
    }
};

class Queen:public Piece{
    public:
    Queen(char color, int symbol, int x, int y) : Piece(color, 'Q', x, y){}
    bool legalMove(std::string move, int color, std::vector<RowType> Board) override{
        return(checkRook(move,color,Board) || checkBishop(move,color,Board));
    }
};

class Knight:public Piece{
    public:
    Knight(char color, int symbol, int x, int y) : Piece(color, 'N', x, y){}
    bool legalMove(std::string move, int color, std::vector<RowType> Board) override{
        if(getX() == (move[0]-96) && getY() == (move[1]- '0')){//checking if moving to same square
            return false;
        }
        if(getX()+2 == move[1]-96 || getX()-2 == move[1]-96){
            return(getY()+1 == move[2]-'0' || getY()-1 == move[2]-'0');
        }
        else if(getY()+2 == move[2]-'0' || getY()-2 == move[2]-'0'){
            return(getX()+1 == move[1]-96 || getX()-1 == move[1]-96);
        }
        return false;
    }

};

class King:public Piece{
    public:
    King(char color, int symbol, int x, int y) : Piece(color, 'K', x, y){}

    bool inCheck(std::string move, std::vector<RowType> Board){
        for (const auto& row : Board) {
            for (const auto& piece : row) {
                if(piece->getColor() != getColor()){
                    if(legalMove(move, piece->getColor(), Board)){
                        std::cout<<"You Are In Check! "<<std::endl;
                        return false;
                    }
                }
            }
        }
        return true;
    }

    bool legalMove(std::string move, int color, std::vector<RowType> Board) override{
        if(getX() == (move[1]-96) && getY() == (move[2]- '0')){//checking if moving to same square
            return false;
        }

        if(std::abs((move[1]-96) - getX()) == 1 || std::abs((move[2]-'0') - getY() == 1)) return(inCheck(move, Board));
        return false;
    }
};

class ChessBoard {
public:
    ChessBoard() {
        const int SIZE = 8;
        RowType SECOND_RANK;
        RowType SEVENTH_RANK;
        RowType EIGHTH_RANK;

        RowType FIRST_RANK{
            std::make_shared<Rook>(Black, 'R',1,8),
            std::make_shared<Knight>(Black, 'N',2,8),
            std::make_shared<Bishop>(Black, 'B',3,8),
            std::make_shared<Queen>(Black, 'Q',4,8),
            std::make_shared<King>(Black, 'K',5,8),
            std::make_shared<Bishop>(Black, 'B',6,8),
            std::make_shared<Knight>(Black, 'N',7,8),
            std::make_shared<Rook>(Black, 'R',8,8)};
        
        for(int x = 1; x<SIZE+1; x++){
            SECOND_RANK.emplace_back(std::make_shared<Pawn>(Black, 'P',x,7));
        }
        
        board.emplace_back(std::move(FIRST_RANK));
        board.emplace_back(std::move(SECOND_RANK));

        for (int i = 2; i < SIZE - 2; ++i) {
            RowType BLANK_RANK; 
            for(int x = 0; x<SIZE; x++){
                BLANK_RANK.emplace_back(std::make_shared<Piece>('.'));
            }
            board.emplace_back(std::move(BLANK_RANK));
        }

        for(int x = 1; x<SIZE+1; x++){
            SEVENTH_RANK.emplace_back(std::make_shared<Pawn>(White, 'P',x,2));
        }

        board.emplace_back(SEVENTH_RANK);

        EIGHTH_RANK.emplace_back(std::make_shared<Rook>(White, 'R',1,1));
        EIGHTH_RANK.emplace_back(std::make_shared<Knight>(White, 'N',2,1));
        EIGHTH_RANK.emplace_back(std::make_shared<Bishop>(White, 'B',3,1));
        EIGHTH_RANK.emplace_back(std::make_shared<Queen>(White, 'Q',4,1));
        EIGHTH_RANK.emplace_back(std::make_shared<King>(White, 'K',5,1));
        EIGHTH_RANK.emplace_back(std::make_shared<Bishop>(White, 'B',6,1)); 
        EIGHTH_RANK.emplace_back(std::make_shared<Knight>(White, 'N',7,1));
        EIGHTH_RANK.emplace_back(std::make_shared<Rook>(White, 'R',8,1));  

        board.emplace_back(EIGHTH_RANK);
    }

    const std::vector<RowType>& getBoard() const {
        return board;
    }

    

    void setPiece(int x, int y, std::shared_ptr<Piece> piece) {
        board[8-(piece->getY())][(piece->getX())-1] = std::make_shared<Piece>('.'); // set old position to empty
        piece->setX(x+1); // set new position for the piece
        piece->setY(y);
        board[8-y][x] = std::move(piece); // move the piece to the new position
    }
private:
    std::vector<RowType> board;
};

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

    const auto& b = board.getBoard();

    while (true) {
        std::cout << "Enter move: ";
        std::cin >> move;
        if(move.length()<2 || move.length()>4){
            std::cout<<"Move is not possible!"<<std::endl;
            continue;

        } else if(move.length() == 2){
            for (const auto& row : b){
                //std::cout<<std::endl;
                for (const auto& piece : row) {
                    //std::cout<<piece->legalMove(move,moveNumber%2)<<" ";
                
                    if(piece->getColor() == moveNumber%2 && piece->getSymbol() == 'P' && piece->legalMove(move, moveNumber%2, b) == 1){
                        std::cout<<"here! "<<std::endl;
                        possiblePiece.emplace_back(piece);
                    }
                }
            }
        } else if(move.length()>=3){
            for (const auto& row : b){
                //std::cout<<std::endl;
                for (const auto& piece : row) {
                    //std::cout<<piece->legalMove(move,moveNumber%2)<<" ";
                    if(piece->getSymbol() == toupper(move[0]) && piece->getColor() == moveNumber%2 && piece->legalMove(move, moveNumber%2, b) == 1){
                        std::cout<<"here! "<<std::endl;
                        possiblePiece.emplace_back(piece);
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
            continue;
        }

        doMove(move, board, moveNumber, possiblePiece[0]);

        possiblePiece.clear();

        //doMove(move, board, moveNumber);
        std::cout<<std::endl;;
        printBoard(board);
        moveNumber++;
    }
    return 0;
}