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
#include <utility>
#include <random>

class Piece;
class ChessBoard;
class King;

using RowType = std::vector<std::shared_ptr<Piece>>;
using piecePair = std::pair<std::string, std::shared_ptr<Piece>>;

int Black = 0;
int White = 1;
std::vector<char> coords = {'a','b','c','d','e','f','g','h'};

void doMove(const std::string&, ChessBoard&, int, std::shared_ptr<Piece>);
void printBoard(const ChessBoard&);
void printvector(std::vector<std::string>);
void printvector(std::vector<piecePair>);
void printX(const ChessBoard&);
void printY(const ChessBoard&);
void Promote(ChessBoard&, std::shared_ptr<Piece>, bool);
void shortCastle(ChessBoard&, int, bool);
void longCastle(ChessBoard&, int, bool);
void castle(std::string, ChessBoard&, int);
bool movingToCheck(ChessBoard&, std::string, int, std::shared_ptr<Piece>);
bool moveIsValid(std::string, ChessBoard&, int, std::shared_ptr<King>);
bool mated(ChessBoard&, int);
RowType getPieces(std::vector<RowType>, std::string, int);

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

    virtual bool legalMove(std::string move, const std::vector<RowType>& Board){return false;}
    virtual void movePiece(std::string move){};
    virtual ~Piece() {}
    
    int getColor() const {return color;}
    char getSymbol() const {return symbol;}
    int getX() const {return x;}
    int getY() const {return y;}
    int getFirstMove() const{return firstMove;}
    int getValue() const{return value;}

    void printInfo(){
        std::cout<<"x: "<<x<<", "<<"y: "<<y<<", "
        "COLOR: "<<color<<", "<<"SYMBOL: "<<symbol<<std::endl;
    }

    std::vector<piecePair> getLegal(std::vector<RowType> board){
        //std::vector<std::string> legalMoves
        std::vector<piecePair> legalMoves;
        const std::vector<char> coords = {'a','b','c','d','e','f','g','h'};
        RowType ambiguousPieces;
        std::string move;

        for (const auto& let : coords) {
            for (int x = 1; x<9; x++){
                move = std::string(1,let)+std::to_string(x);
                if(getSymbol() == 'P' && abs(int(let-96)-getX()) == 1 && abs(getY()-x)==1
                && legalMove((std::string(1, (char)(getX()+96))+"x"+move), board)){
                    legalMoves.emplace_back(std::make_pair(std::string(1, (char)(getX()+96))+"x"+move, board[8-getY()][getX()-1]));
                }
                move = (getSymbol() == 'P') ? move : getSymbol() + move;

                if(legalMove(move, board)){
                    ambiguousPieces = getPieces(board, move, getColor());//can be pawn: one pawn one bishop
                    if(ambiguousPieces.size() > 1){
                        move.erase(0,1);
                        for(auto& piece : ambiguousPieces){
                            if(piece->getY() == getY()){
                                legalMoves.emplace_back(std::make_pair(std::string(1, getSymbol())+std::string(1,(char)piece->getX()+96)+move, piece));
                            }
                            else{
                                legalMoves.emplace_back(std::make_pair(std::string(1, getSymbol())+std::to_string(piece->getY())+move, piece));
                            }
                        }
                    }
                    else{
                        legalMoves.emplace_back(move, ambiguousPieces[0]);
                    }
                }
            }
        }
        return legalMoves;
    }

    bool checkRook(std::string move, std::vector<RowType> Board){
        const int moveSize = move.length()-2;

        const int movingX = move[moveSize]-96;
        const int movingY = move[moveSize+1]-'0';

        const int smallerX = std::min(getX(), movingX);
        const int biggerX = std::max(getX(), movingX);
        const int smallerY = std::min(getY(), movingY);
        const int biggerY = std::max(getY(), movingY);

        if(getX() == movingX && getY() == movingY){return false;}

        else if(getX() == (movingX)){//if the rook is travelling vertically
            for(int i = smallerY+1; i<biggerY; i++){
                if(Board[8-i][getX()-1]->getSymbol() != '.') return false;
            }
        }

        else if(getY() == (movingY)){//if the rook is travelling horizontally
            for(int i = smallerX+1; i<biggerX; i++){
                if(Board[8-getY()][i-1]->getSymbol() != '.') return false;
            }
        }
        else if(getX() != movingX && getY() != movingY){return false;}
        return(Board[8-movingY][movingX-1]->getColor() != getColor());
    }

    bool checkBishop(std::string move, std::vector<RowType> Board) {
        if (getX() == (move[move.length() - 2] - 96) && getY() == (move[move.length() - 1] - '0')) {return false;}

        const int distX = abs(getX() - (move[move.length() - 2] - 96));
        const int distY = abs(getY() - (move[move.length() - 1] - '0'));

        if (distX != distY) {return false;}

        const int incX = (move[move.length() - 2] - 96 > getX()) ? 1 : -1;
        const int incY = (move[move.length() - 1] - '0' > getY()) ? 1 : -1;

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
    bool firstMove = true;
};

class Pawn: public Piece{
    public:
        Pawn(char color, int symbol, int x, int y, int value) : Piece(color, 'P', x, y, 1){}

    bool legalMove(std::string move, const std::vector<RowType>& Board) override {
        int moveSize = move.length() - 2;
        bool isTaking = move.find('x') != std::string::npos;
        
        const int targetX = move[moveSize] - 96;
        const int targetY = move[moveSize + 1] - '0';

        const int currentX = getX();
        const int currentY = getY();

        const int multi = getColor() == White ? -1 : 1;

        if (currentX == targetX && currentY == targetY) {return false;}

        else if(isTaking && currentY - targetY == 1 * multi && currentX == move[0] - 96){

            if(currentX+1 == targetX && getX() != 8 && Board[8 + multi - currentY][currentX]->getSymbol() != '.'){//if this not true defaults to the other one
                return Board[8 + multi - currentY][currentX]->getColor() != getColor();
            }

            else if(currentX-1 == targetX && getX() != 1 && Board[8 + multi - currentY][currentX - 2]->getSymbol() != '.'){
                return Board[8 + multi - currentY][currentX - 2]->getColor() != getColor();
            }
        }

        else if (currentX == targetX && currentY - targetY == 1 * multi && Board[8 + multi - currentY][currentX - 1]->getSymbol() == '.') {
            return true;
        } else if (getFirstMove() && currentX == targetX && currentY - targetY == 2 * multi && Board[8 + multi - currentY][currentX - 1]->getSymbol() == '.' && Board[8 + (2*multi) - currentY][currentX - 1]->getSymbol() == '.') {
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

    bool legalMove(std::string move, const std::vector<RowType>& Board) override{
        if(move.length() == 4){
            if(getX() != move[1]-96 || getY() != move[1]-'0') return false;
        } return checkRook(move, Board);
    }
};

class Bishop: public Piece{
    public:
    Bishop(char color, int symbol, int x, int y, int value) : Piece(color, 'B', x, y, 3){}

    bool legalMove(std::string move, const std::vector<RowType>& Board) override{
        return checkBishop(move, Board);
    }
};

class Queen:public Piece{
    public:
    Queen(char color, int symbol, int x, int y, int value) : Piece(color, 'Q', x, y, 8){}
    bool legalMove(std::string move, const std::vector<RowType>& Board) override{
        return(checkRook(move,Board) || checkBishop(move,Board));
    }
};

class Knight:public Piece{
    public:
    Knight(char color, int symbol, int x, int y, int value) : Piece(color, 'N', x, y, 3){}
    
    bool legalMove(std::string move, const std::vector<RowType>& Board) override{
        if(getX() == (move[move.length()-2]-96) && getY() == (move[move.length()-1]- '0')) return false;
        if(move.length() == 4){
            if(getX() != move[1]-96 || getY() != move[1]-'0') return false;
        }
        int moveX = move[move.length()-2]-96;
        int moveY = move[move.length()-1]-'0';

        std::shared_ptr<Piece> movingSquare = Board[8-moveY][moveX-1];
    
        if(getX()+2 == moveX || getX()-2 == moveX){
            if(getY()+1 == moveY || getY()-1 == moveY){
                if(movingSquare->getSymbol() == '.'){
                    return true;
                }
                return(((movingSquare)->getSymbol() != '.') && (movingSquare)->getColor() != getColor());//checking if moving to same color
            }
        }
        else if(getY()+2 == moveY || getY()-2 == moveY){
            if(getX()+1 == moveX || getX()-1 == moveX){
                if(movingSquare->getSymbol() == '.'){
                    return true;
                }
                return(movingSquare->getSymbol() != '.' && movingSquare->getColor() != getColor());//checking if moving to same color
            }
        }
        return false;
    }

};

class King:public Piece{
    public:
    King(char color, int symbol, int x, int y, int value) : Piece(color, 'K', x, y, 0){}

    std::vector<std::pair<int, int>> kingAttackingBox(const std::vector<RowType>& Board, int color) {
        std::vector<std::pair<int, int>> coords;
        std::shared_ptr<Piece> piece;
    
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                piece = Board[i][j];
                if (piece->getSymbol() == 'K' && piece->getColor() == color) {
                    for (int n = i - 1; n <= i + 1; n++) {
                        for (int m = j - 1; m <= j + 1; m++) {
                            if (n == i && m == j) continue;
                            if (n < 0 || m < 0 || n >= 8 || m >= 8) continue;
                        
                            coords.push_back(std::make_pair(m+1,8-n));
                        }
                    }
                }
            }
        }
        //for(auto& coord : coords){
        //    std::cout<<coord.first<<" "<<coord.second<<" ";
        //}


        //std::cout<<std::endl;
        return coords;
    }


    bool inCheck(std::string move, const std::vector<RowType>& Board) {//returns true if not in check... a bit confusing
        int pieceColor;
        char pieceSymbol;
        for (const auto& row : Board) {
            for (const auto& piece : row) {
                pieceColor = piece->getColor();
                pieceSymbol = piece->getSymbol();

                if (pieceColor != getColor() && pieceSymbol != '.') {
                    if (pieceSymbol == 'P') {
                        if (piece->legalMove(std::string(1,(char)(piece->getX() + 96)) + "x" + move, Board)) {
                           return false;
                        }
                    }

                    else if(pieceSymbol != 'P' && piece->legalMove(std::to_string(pieceSymbol)+move, Board)) {
                        piece->printInfo();
                        return false;
                    }
                }
            }
        }
    

    return true;
    }

    bool legalMove(std::string move, const std::vector<RowType>& Board) override{//change to const & later
        int moveX = (move[move.length()-2])- 96;
        int moveY = (move[move.length()-1])-'0';
        int oppositeColor = (getColor() == White) ? Black : White;
        std::pair<int, int> coord = std::make_pair(moveX, moveY);
        std::vector<std::pair<int, int>> oppositeKingBox = kingAttackingBox(Board, oppositeColor);

        for(auto& i : oppositeKingBox){
            if(i == coord){
                return false;
            }
        }

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
        if (getFirstMove() == false) {return false;}
        
        int rank = (getColor() == White) ? 7 : 0;

        if (move == "O-O") {
            if (Board[rank][4]->getSymbol() == 'K' && Board[rank][7]->getSymbol() == 'R' && Board[rank][5]->getSymbol() == '.' && Board[rank][6]->getSymbol() == '.') {
                if (getFirstMove() && Board[rank][7]->getFirstMove()) {
                    if (!inCheck("f"+std::to_string(8-rank), Board) || !inCheck("g"+std::to_string(8-rank), Board)) {
                        return false;
                    }
                    return true;
                }
            }
        } else if (move == "O-O-O") {
            if (Board[rank][4]->getSymbol() == 'K' && Board[rank][0]->getSymbol() == 'R' && Board[rank][1]->getSymbol() == '.' && Board[rank][2]->getSymbol() == '.' && Board[rank][3]->getSymbol() == '.') {
                if (Board[rank][4]->getFirstMove() && Board[rank][0]->getFirstMove()) {
                    if (!inCheck("d"+std::to_string(8-rank), Board) || !inCheck("c"+std::to_string(8-rank), Board)) {
                        return false;
                    }
                    return true;
                }
            }
        }

        return false;
    }
};

#endif