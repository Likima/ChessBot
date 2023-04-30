#include <iostream>
#include <vector>
#include <string>
#include <cstdio>
#include <cstring>
#include <algorithm>
#include <memory>

//make polymorphism (look into it later)

//using namespace std;
class Piece;


//std::vector<char> coords = {'h','g','f','e','d','c','b','a'};
std::vector<char> coords = {'a','b','c','d','e','f','g','h'};

using RowType = std::vector<std::shared_ptr<Piece>>;
int Black = 0;
int White = 1;

class Piece {
public:
    const static int BLACK = 0;
    const static int WHITE = 1;

    Piece(const Piece& other) : color(other.color), x(other.x), y(other.y), symbol(other.symbol), onEdge(other.onEdge) {}


    Piece() {}
    Piece(char symbol) : symbol(symbol){}
    Piece(char color, int symbol) : symbol(symbol), color(color){}
    Piece(char color, int symbol, int x, int y) : symbol(symbol), color(color), x(x), y(y){}


    int getColor() const {
        return color;
    }
    char getSymbol() const {
        return symbol;
    }
    int getX() const {
        return x;
    }
    int getY() const {
        return y;
    }

    void setColor(int color) {
        this->color = color;
    }
    void setSymbol(char symbol) {
        this->symbol = symbol;
    }
    void setX(int x){
        this->x = x;
    }
    void setY(int y){
        this->y = y;
    }

private:
    int color;
    int x;
    int y;
    char symbol;
    bool onEdge;
};
class Pawn: public Piece{
    public:
    Pawn(char color, int symbol, int x, int y) : Piece(color, 'P', x, y){}
        //bool legalMove(string move){
            
        //}
    private:
        bool firstMove = false;
        bool enPassantable = false;
};

class ChessBoard {
public:
    ChessBoard() {
        const int SIZE = 8;
        RowType SECOND_RANK;
        RowType SEVENTH_RANK;
        RowType EIGHTH_RANK;

        RowType FIRST_RANK{
            std::make_shared<Piece>(Black, 'R',0,0),
            std::make_shared<Piece>(Black, 'N',1,0),
            std::make_shared<Piece>(Black, 'B',2,0),
            std::make_shared<Piece>(Black, 'Q',3,0),
            std::make_shared<Piece>(Black, 'K',4,0),
            std::make_shared<Piece>(Black, 'B',5,0),
            std::make_shared<Piece>(Black, 'N',6,0),
            std::make_shared<Piece>(Black, 'R',7,0)};
        
        for(int x = 0; x<SIZE; x++){
            SECOND_RANK.emplace_back(std::make_shared<Pawn>(Black, 'P',x,1));
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

        for(int x = 0; x<SIZE; x++){
            SEVENTH_RANK.emplace_back(std::make_shared<Pawn>(White, 'P',x,1));
        }

        board.emplace_back(SEVENTH_RANK);

        EIGHTH_RANK.emplace_back(std::make_shared<Piece>(White, 'R',0,0));
        EIGHTH_RANK.emplace_back(std::make_shared<Piece>(White, 'N',1,0));
        EIGHTH_RANK.emplace_back(std::make_shared<Piece>(White, 'B',2,0));
        EIGHTH_RANK.emplace_back(std::make_shared<Piece>(White, 'Q',3,0));
        EIGHTH_RANK.emplace_back(std::make_shared<Piece>(White, 'K',4,0));
        EIGHTH_RANK.emplace_back(std::make_shared<Piece>(White, 'B',5,0)); 
        EIGHTH_RANK.emplace_back(std::make_shared<Piece>(White, 'N',6,0));
        EIGHTH_RANK.emplace_back(std::make_shared<Piece>(White, 'R',7,0));  

        board.emplace_back(EIGHTH_RANK);
    }

    const std::vector<RowType>& getBoard() const {
        return board;
    }

    void setPiece(int x, int y, std::shared_ptr<Piece> piece) {
        board[piece->getY()][piece->getX()] = std::make_shared<Piece>('.'); // set old position to empty
        piece->setX(x); // set new position for the piece
        piece->setY(y);
        board[y][x] = std::move(piece); // move the piece to the new position
    }
private:
    std::vector<RowType> board;
};

void printBoard(const ChessBoard& board) {
    const auto& b = board.getBoard();
    for (const auto& row : b) {
        for (const auto& piece : row) {
            std::cout << piece->getSymbol() << " ";
        }

        std::cout << std::endl;
    }
}

void doMove(const std::string& move, ChessBoard& board, int moveNumber) {//rn only works for three letter things like ke5...
    //cout<<move[2]<<endl;

    char m = move[0];
    int horizontalCoord = move[1] - 97;
    int verticalCoord = move[2] - '0';

    std::shared_ptr<Piece> moved;
    const auto& b = board.getBoard();
    for (const auto& row : b) {
        for (auto& piece : row) {
            if(piece && piece->getSymbol() == toupper(m) && piece->getColor() == moveNumber%2){
                moved = piece;
                break;
            }
        }
    }
    int horizontalInt = 0;

    for(int x = 8; x>1; x--){
        if(x == verticalCoord){
            for (std::vector<char>::iterator t = coords.begin(); t != coords.end(); ++t) {
                if(*t == horizontalCoord){
                    board.setPiece(horizontalInt, verticalCoord-1, std::move(moved));
                    break;
                }
                horizontalInt++;            
            }
        }

    }
}


int main() {
    std::string move;
    RowType possiblePiece;
    int moveNumber = 1;
    int incRow = 0;
    int incCol = 0;
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
                if(move[1] == incRow){
                    for (const auto& piece : row) {
                        if(move[0]-'0' == incCol && piece->getSymbol() == 'P'){
                            possiblePiece.insert(possiblePiece.end(), piece);
                        }
                        incCol++;
                    }
                }
                incRow++;
            }
        }

        //if(possiblePiece.empty()){
        //    cout<<"Move is not possible!"<<endl;
        //    continue;
       // }
        
        for(const auto& piece : possiblePiece){
            std::cout<<piece->getSymbol()<<", "<<piece->getColor()<<std::endl;
            /*
            if(piece.legalMove()){
                //movingPiece = &piece;
                
            }
            */
        }
        

        doMove(move, board, moveNumber);
        //else continue;
        printBoard(board);
        moveNumber++;
    }
    return 0;
}