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
    bool firstMove = true;

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
        "COLOR: "<<color<<", "<<"SYMBOL: "<<symbol<<std::endl;
    }

    bool checkRook(std::string move, int color, std::vector<RowType> Board){
        int moveSize = move.length()-2;
        std::cout<<"Rook "<<std::endl;
        printInfo();
        //std::cout<<"("<<getX()<<", "<<(move[moveSize]-96)<<", "<<getY()<<", "<<(move[moveSize+1] - '0')<<")"<<std::endl;
        if(getX() == (move[moveSize]-96) && getY() == (move[moveSize+1]- '0')){//checking if moving to same square
            return false;
        }
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

    bool checkBishop(std::string move, int color, std::vector<RowType> Board) {
        printInfo();
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

    bool legalMove(std::string move, int color, std::vector<RowType> Board) override {
        int moveSize = move.length() - 2;
        int multi = 1;
        bool isTaking = move.find('x') != std::string::npos;

        int targetX = move[moveSize] - 96;
        int targetY = move[moveSize + 1] - '0';

        int currentX = getX();
        int currentY = getY();

        if(getColor() == White) multi*=-1;

        if (currentX == targetX && currentY == targetY) {
            return false;  // Same square
        }

        if (currentX == targetX && abs(currentY - targetY) == 1 && Board[8 + multi - currentY][currentX - 1]->getSymbol() == '.') {
            this->firstMove = false;
            return true;
        } else if (firstMove && currentX == targetX && abs(currentY - targetY) == 2 && Board[8 + multi - currentY][currentX - 1]->getSymbol() == '.' && Board[8 + (2*multi) - currentY][currentX - 1]->getSymbol() == '.') {
            this->firstMove = false;
            return true;
        } else if (isTaking && currentX == move[0] - 96 && currentX != 1 && abs(currentX - targetX) == 1 && Board[8 + multi - currentY][currentX - 2]->getSymbol() != '.') {
            if (Board[8 + multi - currentY][currentX - 2]->getColor() == getColor()) {
                return false;
            }
            this->firstMove = false;
            return true;
        } else if (isTaking && currentX == move[0] - 96 && currentX != 8 && abs(currentX - targetX) == 1 && Board[8 + multi - currentY][currentX]->getSymbol() != '.') {
            if (Board[8 + multi - currentY][currentX]->getColor() == getColor()) {
                return false;
            }
            this->firstMove = false;
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

    bool legalMove(std::string move, int color, std::vector<RowType> Board) override{
        return checkRook(move, color, Board);
    }
};

class Bishop: public Piece{
    public:
    Bishop(char color, int symbol, int x, int y) : Piece(color, 'B', x, y){}

    bool legalMove(std::string move, int color, std::vector<RowType> Board) override{
        std::cout<<"Bishop "<<std::endl;
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
        int targetX = move[move.length() - 2] - 96;
        int targetY = move[move.length() - 1] - '0';

        for (const auto& row : Board) {
            for (const auto& piece : row) {
                int pieceColor = piece->getColor();
                char pieceSymbol = piece->getSymbol();

                if (pieceColor != getColor() && pieceSymbol != 'K' && pieceSymbol != '.') {
                    if (pieceSymbol == 'P') {
                        if (piece->legalMove((char)(getX() + 96) + "x" + move, pieceColor, Board)) {
                            piece->printInfo();
                            std::cout<<std::endl;
                            return false;
                        }
                    }
                    std::cout<<"Comb Move is: "<<pieceSymbol+move<<std::endl;
                    if(pieceSymbol != 'P' && piece->legalMove(pieceSymbol+move, pieceColor, Board)) {
                        piece->printInfo();
                        std::cout << "You Are In Check!" << std::endl;
                        return false;
                    }
                }
            }
        }

    return true;
}

    bool legalMove(std::string move, int color, std::vector<RowType> Board) override{
        if(getX() == (move[move.length()-2]-96) && getY() == (move[move.length()-1]- '0')){//checking if moving to same square
            return false;
        }
        if(std::abs((move[move.length()-2]-96) - getX()) <= 1 || std::abs((move[move.length()-1]-'0') - getY() <= 1)) return(inCheck(move, Board));
        //else if(std::abs((move[move.length()-2]-96) - getX()) == 1 && std::abs((move[move.length()-1]-'0') - getY() == 0)) return(inCheck(move, Board));
        //else if(std::abs((move[move.length()-2]-96) - getX()) == 0 && std::abs((move[move.length()-1]-'0') - getY() == 1)) return(inCheck(move, Board));
        return false;
    }



    private:
        bool checked = false;
};

#endif