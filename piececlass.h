#ifndef PIECECLASS_H
#define PIECECLASS_H

#include <iostream>
#include <vector>
#include <string>
#include <cstdio>
#include <cstring>
#include <ctype.h>
#include <algorithm>
#include <memory>
#include <random>

class Piece;
class ChessBoard;
class King;

using RowType = std::vector<std::shared_ptr<Piece>>;

int Black = 0;
int White = 1;
std::vector<char> coords = {'a','b','c','d','e','f','g','h'};

void doMove(const std::string&, ChessBoard&, int, std::shared_ptr<Piece>);
void printBoard(const ChessBoard&);
void printvector(std::vector<std::string>);
void printX(const ChessBoard&);
void printY(const ChessBoard&);
void Promote(ChessBoard&, std::shared_ptr<Piece>);
void shortCastle(ChessBoard&, int);
void longCastle(ChessBoard&, int);
void castle(std::string, ChessBoard&, int);
bool movingToCheck(ChessBoard&, std::string, int, std::shared_ptr<Piece>);
bool moveIsValid(std::string, ChessBoard&, int, std::shared_ptr<King>);
bool mated(ChessBoard&, int);
RowType getPieces(ChessBoard&, std::string, int);

struct pieceType{
    RowType ptype;
    int location;
    std::vector<std::string> legalmoves;
};

class Piece {
public:
    const static int BLACK = 0;
    const static int WHITE = 1;

    Piece(const Piece& other) : color(other.color), symbol(other.symbol), x(other.x), y(other.y), value(other.value){}

    Piece() {}
    Piece(char symbol) : symbol(symbol), color(-1){}
    Piece(int color, char symbol, int x, int y) : color(color), symbol(symbol), x(x), y(y){}
    Piece(int color, char symbol) : symbol(symbol), color(color){}
    Piece(int color, char symbol, int x, int y, int value) : color(color), symbol(symbol), x(x), y(y), value(value){}

    virtual bool legalMove(std::string move, std::vector<RowType> Board){return false;}
    virtual void movePiece(std::string move){};
    virtual ~Piece() {}
    
    int getColor() const {return color;}
    char getSymbol() const {return symbol;}
    int getX() const {return x;}
    int getY() const {return y;}
    int getFirstMove() const{return firstMove;}

    void printInfo(){
        std::cout<<"x: "<<x<<", "<<"y: "<<y<<", "
        "COLOR: "<<color<<", "<<"SYMBOL: "<<symbol<<std::endl;
    }

    std::vector<std::string> getLegal(std::vector<RowType> board){
        std::vector<std::string> legalMoves;
        const std::vector<char> coords = {'a','b','c','d','e','f','g','h'};
        std::string move;
        for (const auto& let : coords) {
            for (int x = 1; x<8; x++){
                move = std::string(1,let)+std::to_string(x);
                move = (getSymbol() == 'P') ? move : getSymbol() + move;
                //std::cout<<"MOVES CHECKED "<< std::string(1, getSymbol())+std::string(1, let)+std::to_string(x)<<std::endl;
                if(legalMove(move, board)){
                    //if(getSymbol() != 'P') legalMoves.emplace_back(std::string(1, getSymbol())+std::string(1, let)+std::to_string(x));
                    //else(legalMoves.emplace_back(std::string(1, let)+std::to_string(x)));
                    legalMoves.emplace_back(move);
                }
            }
        }
        return legalMoves;
    }

    bool checkRook(std::string move, std::vector<RowType> Board){
        int moveSize = move.length()-2;
        if(getX() == (move[moveSize]-96) && getY() == (move[moveSize+1]- '0')){return false;}
        else if(getX() == (move[moveSize]-96)){//if the rook is travelling vertically
            movingY = move[moveSize+1]-'0';
            if(getY()<movingY){
                for(int i = getY()+1; i<movingY; i++){
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
        else if(getY() == (move[moveSize+1] - '0')){//if the rook is travelling horizontally
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
                return true;                
            }
        }
        return false;
    }

    bool checkBishop(std::string move, std::vector<RowType> Board) {
        if (getX() == (move[move.length() - 2] - 96) && getY() == (move[move.length() - 1] - '0')) {
            // Checking if moving to the same square
            return false;
        }

        int distX = abs(getX() - (move[move.length() - 2] - 96));
        int distY = abs(getY() - (move[move.length() - 1] - '0'));

        if (distX != distY) {
            return false;
        }

        int incX = (move[move.length() - 2] - 96 > getX()) ? 1 : -1;
        int incY = (move[move.length() - 1] - '0' > getY()) ? 1 : -1;

        for (int x = 1; x < distX; x++) {
            if (Board[8 - (getY() + incY * x)][getX() + incX * x - 1]->getSymbol() != '.') {
                return false;
            }
        }

        if (Board[8 - (move[move.length() - 1] - '0')][(move[move.length() - 2] - 97)]->getColor() == getColor()) {
            return false;
        }

        return true;
    }


    void setFirstMove(){this->firstMove = false;}
    void setColor(int color) {this->color = color;}
    void setSymbol(char symbol) {this->symbol = symbol;}
    void setX(int x){this->x = x;}
    void setY(int y){this->y = y;}

private:
    int color;
    int x;
    int y;
    int value;
    char symbol;
    int distY;
    int distX;
    int incX;
    int incY;
    int movingX;
    int movingY;
    bool firstMove = true;
};

class Pawn: public Piece{
    public:
        Pawn(char color, int symbol, int x, int y, int value) : Piece(color, 'P', x, y, 1){}

    bool legalMove(std::string move, std::vector<RowType> Board) override {
        int moveSize = move.length() - 2;
        int multi = 1;
        bool isTaking = move.find('x') != std::string::npos;
        
        int targetX = move[moveSize] - 96;
        int targetY = move[moveSize + 1] - '0';

        //std::cout<<move<<" ";

        int currentX = getX();
        int currentY = getY();
        /*
        std::cout<<"TargetX "<<targetX<<" "
                 <<"TargetY "<<targetY<<" "
                 <<"CurrentX "<<currentX<<" "
                 <<"CurrentY "<<currentY<<" "<<std::endl;
        */

        if(getColor() == White) multi*=-1;

        if (currentX == targetX && currentY == targetY) {
            return false;  // Same square
        }

        if (currentX == targetX && currentY - targetY == 1 * multi && Board[8 + multi - currentY][currentX - 1]->getSymbol() == '.') {
            return true;
        } else if (getFirstMove() && currentX == targetX && currentY - targetY == 2 * multi && Board[8 + multi - currentY][currentX - 1]->getSymbol() == '.' && Board[8 + (2*multi) - currentY][currentX - 1]->getSymbol() == '.') {
            return true;
        } else if (isTaking && currentX == move[0] - 96 && currentX != 1 && currentX - targetX == 1 && Board[8 + multi - currentY][currentX - 2]->getSymbol() != '.') {
            if (Board[8 + multi - currentY][currentX - 2]->getColor() == getColor()) {
                return false;
            }
            return true;
        } else if (isTaking && currentX == move[0] - 96 && currentX != 8 && currentX - targetX == -1 && Board[8 + multi - currentY][currentX]->getSymbol() != '.') {
            if (Board[8 + multi - currentY][currentX]->getColor() == getColor()) {
                return false;
            }
            return true;
        }

        return false;
    }

    private:
        bool enPassantable = false;
};

class Rook: public Piece{
    public:
    Rook(char color, int symbol, int x, int y, int value) : Piece(color, 'R', x, y, 5){}

    bool legalMove(std::string move, std::vector<RowType> Board) override{
        return checkRook(move, Board);
    }
};

class Bishop: public Piece{
    public:
    Bishop(char color, int symbol, int x, int y, int value) : Piece(color, 'B', x, y, 3){}

    bool legalMove(std::string move, std::vector<RowType> Board) override{
        return checkBishop(move, Board);
    }
};

class Queen:public Piece{
    public:
    Queen(char color, int symbol, int x, int y, int value) : Piece(color, 'Q', x, y, 8){}
    bool legalMove(std::string move, std::vector<RowType> Board) override{
        return(checkRook(move,Board) || checkBishop(move,Board));
    }
};

class Knight:public Piece{
    public:
    Knight(char color, int symbol, int x, int y, int value) : Piece(color, 'N', x, y, 3){}
    bool legalMove(std::string move, std::vector<RowType> Board) override{
        if(getX() == (move[move.length()-2]-96) && getY() == (move[move.length()-1]- '0')) return false;
        int moveX = move[move.length()-2]-96;
        int moveY = move[move.length()-1]-'0';
        std::shared_ptr<Piece> movingSquare = Board[8-moveY][moveX-1];

        if(getX()+2 == moveX || getX()-2 == moveX){
            if(getY()+1 == moveY || getY()-1 == moveY){
                if(movingSquare->getSymbol() == '.'){
                    return true;
                }
                return(((movingSquare)->getSymbol() != '.') && (movingSquare)->getColor() != getColor());//checking if moving to same square
            }
        }
        else if(getY()+2 == moveY || getY()-2 == moveY){
            if(getX()+1 == moveX || getX()-1 == moveX){
                if(movingSquare->getSymbol() == '.'){
                    return true;
                }
                return(movingSquare->getSymbol() != '.' && movingSquare->getColor() != getColor());//checking if moving to same square
            }
        }
        return false;
    }

};

class King:public Piece{
    public:
    King(char color, int symbol, int x, int y, int value) : Piece(color, 'K', x, y, 0
    ){}

    bool inCheck(std::string move, const std::vector<RowType>& Board) {
        int pieceColor;
        char pieceSymbol;
        for (const auto& row : Board) {
            for (const auto& piece : row) {
                int pieceColor = piece->getColor();
                //std::cout << "Checking for check" << std::endl;
                char pieceSymbol = piece->getSymbol();

                //piece->printInfo();

                if (pieceColor != getColor() && pieceSymbol != 'K' && pieceSymbol != '.') {
                    if (pieceSymbol == 'P') {
                        //if (piece->legalMove("^"+std::string(1,(char)(getX() + 96)) + "x" + move, Board)) {
                            //piece->printInfo();
                        //    return false;
                        //}
                    }
                    if(pieceSymbol != 'P' && piece->legalMove(std::to_string(pieceSymbol)+move, Board)) {
                        //piece->printInfo();
                        //std::cout << "You Are In Check!" << std::endl;
                        return false;
                    }
                }
            }
        }

    return true;
}

    bool legalMove(std::string move, std::vector<RowType> Board) override{
        int moveX = (move[move.length()-2])- 96;
        int moveY = (move[move.length()-1])-'0';
        std::shared_ptr<Piece> movingPiece = Board[8-moveY][moveX-1];
        if(getX() == moveX && getY() == moveY){//checking if moving to same square
            return false;
        }
        if(std::abs(moveX - getX()) <= 1 && std::abs(moveY - getY()) <= 1){
            if(movingPiece->getSymbol() == '.') return(inCheck(move, Board));
            else if(movingPiece->getSymbol() != '.' && movingPiece->getColor() != getColor()) return(inCheck(move, Board));
        }
        return false;
    }

    bool canCastle(std::string move, std::vector<RowType> Board) {
        std::string kingPosString;
        std::shared_ptr<King> kingPtr;

        if (move == "O-O") {
            if (getColor() == White) {
                if (Board[7][4]->getSymbol() == 'K' && Board[7][7]->getSymbol() == 'R' && Board[7][5]->getSymbol() == '.' && Board[7][6]->getSymbol() == '.') {
                    if (Board[7][4]->getFirstMove() && Board[7][7]->getFirstMove()) {
                        if (!inCheck("f1", Board) || !inCheck("g1", Board)) {
                            return false;
                        }
                        return true;
                    }
                }
            } else {
                if (Board[0][4]->getSymbol() == 'K' && Board[0][7]->getSymbol() == 'R' && Board[0][5]->getSymbol() == '.' && Board[0][6]->getSymbol() == '.') {
                    if (Board[0][4]->getFirstMove() && Board[0][7]->getFirstMove()) {
                        kingPtr = std::dynamic_pointer_cast<King>(Board[0][4]);
                        if (!kingPtr->inCheck("f8", Board) || !kingPtr->inCheck("g8", Board)) {
                            return false;
                        }
                        return true;
                    }
                }
            }
        } else if (move == "O-O-O") {
            if (getColor() == White) {
                if (Board[7][4]->getSymbol() == 'K' && Board[7][0]->getSymbol() == 'R' && Board[7][1]->getSymbol() == '.' && Board[7][2]->getSymbol() == '.' && Board[7][3]->getSymbol() == '.') {
                    if (Board[7][4]->getFirstMove() && Board[7][0]->getFirstMove()) {
                        kingPtr = std::dynamic_pointer_cast<King>(Board[7][4]);
                        if (!kingPtr->inCheck("d1", Board) || !kingPtr->inCheck("c1", Board)) {
                            return false;
                        }
                        return true;
                    }
                }
            } else {
                if (Board[0][4]->getSymbol() == 'K' && Board[0][0]->getSymbol() == 'R' && Board[0][1]->getSymbol() == '.' && Board[0][2]->getSymbol() == '.' && Board[0][3]->getSymbol() == '.') {
                    if (Board[0][4]->getFirstMove() && Board[0][0]->getFirstMove()) {
                        kingPtr = std::dynamic_pointer_cast<King>(Board[0][4]);
                        if (!kingPtr->inCheck("d8", Board) || !kingPtr->inCheck("c8", Board)) {
                            return false;
                        }
                        return true;
                    }
                }
            }
        }

        return false;
    }
};

#endif