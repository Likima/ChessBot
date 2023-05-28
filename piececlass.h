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

class Piece;

using RowType = std::vector<std::shared_ptr<Piece>>;

int Black = 0;
int White = 1;
std::vector<char> coords = {'a','b','c','d','e','f','g','h'};

class Piece {
public:
    const static int BLACK = 0;
    const static int WHITE = 1;
    //bool firstMove = true;

    Piece(const Piece& other) : color(other.color), symbol(other.symbol), x(other.x), y(other.y){}

    Piece() {}
    Piece(int symbol) : symbol(symbol), color(-1){}
    Piece(int color, char symbol) : symbol(symbol), color(color){}
    Piece(int color, char symbol, int x, int y) : symbol(symbol), color(color), x(x), y(y){}

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
        for (const auto& let : coords) {
            for (int x = 1; x<8; x++){
                //std::cout<<"MOVES CHECKED "<< std::string(1, getSymbol())+std::string(1, let)+std::to_string(x)<<std::endl;
                if(legalMove("^"+std::string(1, getSymbol())+std::string(1, let)+std::to_string(x), board)){
                    legalMoves.emplace_back(std::string(1, getSymbol())+std::string(1, let)+std::to_string(x));
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
        Pawn(char color, int symbol, int x, int y) : Piece(color, 'P', x, y){}

    bool legalMove(std::string move, std::vector<RowType> Board) override {
        int moveSize = move.length() - 2;
        int multi = 1;
        bool isTaking = move.find('x') != std::string::npos;
        bool isChecking = move.find('^') != std::string::npos;

        int targetX = move[moveSize] - 96;
        int targetY = move[moveSize + 1] - '0';

        int currentX = getX();
        int currentY = getY();

        if(getColor() == White) multi*=-1;

        if (currentX == targetX && currentY == targetY) {
            return false;  // Same square
        }

        if (currentX == targetX && abs(currentY - targetY) == 1 && Board[8 + multi - currentY][currentX - 1]->getSymbol() == '.') {
            if (!isChecking) setFirstMove();
            return true;
        } else if (getFirstMove() && currentX == targetX && abs(currentY - targetY) == 2 && Board[8 + multi - currentY][currentX - 1]->getSymbol() == '.' && Board[8 + (2*multi) - currentY][currentX - 1]->getSymbol() == '.') {
            if (!isChecking) setFirstMove();
            return true;
        } else if (isTaking && currentX == move[0] - 96 && currentX != 1 && abs(currentX - targetX) == 1 && Board[8 + multi - currentY][currentX - 2]->getSymbol() != '.') {
            if (Board[8 + multi - currentY][currentX - 2]->getColor() == getColor()) {
                return false;
            }
            if (!isChecking) setFirstMove();
            return true;
        } else if (isTaking && currentX == move[0] - 96 && currentX != 8 && abs(currentX - targetX) == 1 && Board[8 + multi - currentY][currentX]->getSymbol() != '.') {
            if (Board[8 + multi - currentY][currentX]->getColor() == getColor()) {
                return false;
            }
            if (!isChecking) setFirstMove();
            return true;
        }

        return false;
    }

    private:
        bool enPassantable = false;
};

class Rook: public Piece{
    public:
    Rook(char color, int symbol, int x, int y) : Piece(color, 'R', x, y){}

    bool legalMove(std::string move, std::vector<RowType> Board) override{
        return checkRook(move, Board);
    }
};

class Bishop: public Piece{
    public:
    Bishop(char color, int symbol, int x, int y) : Piece(color, 'B', x, y){}

    bool legalMove(std::string move, std::vector<RowType> Board) override{
        return checkBishop(move, Board);
    }
};

class Queen:public Piece{
    public:
    Queen(char color, int symbol, int x, int y) : Piece(color, 'Q', x, y){}
    bool legalMove(std::string move, std::vector<RowType> Board) override{
        return(checkRook(move,Board) || checkBishop(move,Board));
    }
};

class Knight:public Piece{
    public:
    Knight(char color, int symbol, int x, int y) : Piece(color, 'N', x, y){}
    bool legalMove(std::string move, std::vector<RowType> Board) override{
        if(getX() == (move[move.length()-2]-96) && getY() == (move[move.length()-1]- '0')){//checking if moving to same square
            return false;
        }
        if(getX()+2 == move[move.length()-2]-96 || getX()-2 == move[move.length()-2]-96){
            return(getY()+1 == move[move.length()-1]-'0' || getY()-1 == move[move.length()-1]-'0');
        }
        else if(getY()+2 == move[move.length()-1]-'0' || getY()-2 == move[move.length()-1]-'0'){
            return(getX()+1 == move[move.length()-2]-96 || getX()-1 == move[move.length()-2]-96);
        }
        return false;
    }

};

class King:public Piece{
    public:
    King(char color, int symbol, int x, int y) : Piece(color, 'K', x, y){}

    bool inCheck(std::string move, const std::vector<RowType>& Board) {
        int pieceColor;
        char pieceSymbol;
        for (const auto& row : Board) {
            for (const auto& piece : row) {
                pieceColor = piece->getColor();
                pieceSymbol = piece->getSymbol();

                if (pieceColor != getColor() && pieceSymbol != 'K' && pieceSymbol != '.') {
                    if (pieceSymbol == 'P') {
                        if (piece->legalMove("^"+std::string(1,(char)(getX() + 96)) + "x" + move, Board)) {
                            printInfo();
                            return false;
                        }
                    }
                    if(pieceSymbol != 'P' && piece->legalMove(pieceSymbol+move, Board)) {
                        printInfo();
                        return false;
                    }
                }
            }
        }

    return true;
}

    bool legalMove(std::string move, std::vector<RowType> Board) override{
        if(getX() == (move[move.length()-2]-96) && getY() == (move[move.length()-1]- '0')){//checking if moving to same square
            return false;
        }
        if(std::abs((move[move.length()-2]-96) - getX()) <= 1 && std::abs((move[move.length()-1]-'0') - getY() <= 1)){
            if(Board[8-(move[move.length()-1]-'0')][move[move.length()-2]-97]->getSymbol() == '.') return(inCheck(move, Board));
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
                        kingPtr = std::dynamic_pointer_cast<King>(Board[7][5]);
                        if (kingPtr->inCheck("f1", Board) || kingPtr->inCheck("g1", Board)) {
                            return false;
                        }
                        return true;
                    }
                }
            } else {
                if (Board[0][4]->getSymbol() == 'K' && Board[0][7]->getSymbol() == 'R' && Board[0][5]->getSymbol() == '.' && Board[0][6]->getSymbol() == '.') {
                    if (Board[0][4]->getFirstMove() && Board[0][7]->getFirstMove()) {
                        kingPtr = std::dynamic_pointer_cast<King>(Board[0][5]);
                        if (kingPtr->inCheck("f8", Board) || kingPtr->inCheck("g8", Board)) {
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
                        kingPtr = std::dynamic_pointer_cast<King>(Board[7][3]);
                        if (kingPtr->inCheck("d1", Board) || kingPtr->inCheck("c1", Board)) {
                            return false;
                        }
                        return true;
                    }
                }
            } else {
                if (Board[0][4]->getSymbol() == 'K' && Board[0][0]->getSymbol() == 'R' && Board[0][1]->getSymbol() == '.' && Board[0][2]->getSymbol() == '.' && Board[0][3]->getSymbol() == '.') {
                    if (Board[0][4]->getFirstMove() && Board[0][0]->getFirstMove()) {
                        kingPtr = std::dynamic_pointer_cast<King>(Board[0][3]);
                        if (kingPtr->inCheck("d8", Board) || kingPtr->inCheck("c8", Board)) {
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