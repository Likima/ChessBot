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

        int moveSize = move.length()-2;
        bool isTaking = move.find('x') != std::string::npos;

        if(getX() == (move[moveSize]-96) && getY() == (move[moveSize+1]- '0')){//checking if moving to same square
            return false;
        }
        std::cout<<"("<<getX()<<", "<<(move[moveSize]-97)<<", "<<getY()<<", "<<(move[moveSize+1] - '0')<<")"<<std::endl;
        if(color == White){
            if(getX() == (move[moveSize]-96) && getY()+1 == (move[moveSize+1] - '0') && Board[7-getY()][getX()-1]->getSymbol() == '.'){
                this->firstMove = false;
                return true;
            }
            else if(firstMove && getX() == (move[moveSize]-96) && getY()+2 == (move[moveSize+1] - '0')&& Board[7-getY()][getX()-1]->getSymbol() == '.' && Board[6-getY()][getX()-1]->getSymbol() == '.'){
                this->firstMove = false;
                return true;
            }
            else if(isTaking && getX() == move[0]-96 && getX() != 1 && getX()-1 == move[moveSize]-96 && Board[7-getY()][getX()-2]->getSymbol() != '.'){
                if(Board[7-getY()][getX()-2]->getColor() == getColor()) return false;
                this->firstMove = false;
                return true;
            }
            else if(isTaking && getX() == move[0]-96 && getX() != 8 && getX()+1 == move[moveSize]-96 && Board[7-getY()][getX()]->getSymbol() != '.'){
                if(Board[7-getY()][getX()]->getColor() == getColor()) return false;
                this->firstMove = false;
                return true;
            }
        }
        else if(color == Black){
            if(getX() == (move[moveSize]-96) && getY()-1 == (move[moveSize+1] - '0') && Board[9-getY()][getX()-1]->getSymbol() == '.'){
                this->firstMove = false;
                return true;
            }
            else if(firstMove && getX()-1 == (move[moveSize]-97) && getY()-2 == (move[moveSize+1] - '0')
            && Board[9-getY()][getX()-1]->getSymbol() == '.' && Board[10-getY()][getX()-1]->getSymbol() == '.'){
                this->firstMove = false;
                return true;
            }
            else if(isTaking && getX() == move[0]-96 && getX() != 1 && getX()-1 == move[moveSize]-96 && Board[9-getY()][getX()-2]->getSymbol() != '.'){
                if(Board[9-getY()][getX()-2]->getColor() == getColor()) return false;//taking left
                this->firstMove = false;
                return true;
            }
            else if(isTaking && getX() == move[0]-96 && getX() != 8 && getX()+1 == move[moveSize]-96 && Board[9-getY()][getX()]->getSymbol() != '.'){
                if(Board[9-getY()][getX()]->getColor() == getColor()) return false;
                this->firstMove = false;
                return true;
            }
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

    bool inCheck(std::string move, std::vector<RowType>& Board){
        //std::cout<<"HERE ";
        for (const auto& row : Board) {
            //std::cout<<std::endl;
            for (const auto& piece : row) {
                if(piece->getColor() != getColor()){
                    //std::cout<<piece->getColor()<<" "<<getColor()<<" ";
                    if(piece->getSymbol() == 'P'){
                        if(piece->legalMove((char)(getX()+96)+"x"+move, piece->getColor(), Board)) return false;
                    }
                    else if(piece->getSymbol() != 'K' && piece->legalMove(move, piece->getColor(), Board)){
                        std::cout<<"You Are In Check! "<<std::endl;
                        return false;
                    }
                }
            }
        }
        return true;
    }

    bool legalMove(std::string move, int color, std::vector<RowType> Board) override{
        //std::cout<<"hERE";
        if(getX() == (move[1]-96) && getY() == (move[2]- '0')){//checking if moving to same square
            return false;
        }

        if(std::abs((move[1]-96) - getX()) == 1 || std::abs((move[2]-'0') - getY() == 1)) return(inCheck(move, Board));
        return false;
    }

    private:
        bool checked = false;
};

#endif