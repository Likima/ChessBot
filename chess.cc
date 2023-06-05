<<<<<<< HEAD
#include <iostream>
#include <vector>
#include <string>
#include <cstdio>
#include <cstring>
#include <ctype.h>
#include <algorithm>
#include <memory>

//make polymorphism (look into it later) [virtual keyword]
//this is the actual code
//hello

class Piece;

std::vector<char> coords = {'a','b','c','d','e','f','g','h'};

using RowType = std::vector<std::shared_ptr<Piece>>;
int Black = 0;
int White = 1;

class Piece {
public:
    const static int BLACK = 0;
    const static int WHITE = 1;

    Piece(const Piece& other) : color(other.color), symbol(other.symbol), x(other.x), y(other.y), onEdge(other.onEdge) {}


    Piece() {}
    Piece(int symbol) : symbol(symbol){}
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
    

    void setColor(int color) {this->color = color;}
    void setSymbol(char symbol) {this->symbol = symbol;}
    void setX(int x){this->x = x;}
    void setY(int y){this->y = y;}

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
    bool legalMove(std::string move, int color, std::vector<RowType> Board) override{
        //std::cout<<"("<<getX()<<", "<<(move[0]-97)<<", "<<getY()<<", "<<(move[1] - '0')<<")"<<std::endl;
        if(color == White){
            if(getX() == (move[0]-96) && getY()+1 == (move[1] - '0')){
                this->firstMove = false;
                return true;
            }
            else if(firstMove == true && getX()-1 == (move[0]-97) && getY()+2 == (move[1] - '0')){
                this->firstMove = false;
                return true;
            }
        }
        else if(color == Black){
            if(getX() == (move[0]-96) && getY()-1 == (move[1] - '0')){
                this->firstMove = false;
                return true;
            }
            else if(firstMove == true && getX()-1 == (move[0]-97) && getY()-2 == (move[1] - '0')){
                this->firstMove = false;
                return true;
            }
        }
        return false;
=======
#include "piececlass.h"
#include "boardclass.h"
#include "chessalgorithm.h"
#include "printStatements.h"
#include "specialmoves.h"
#include "validmovecheck.h"

bool pawnMove(std::string move)
{
    if (move.length() == 2)
        return true;
    else if (move.length() == 4 && move.find('x') != std::string::npos)
        return true;
    return false;
}
>>>>>>> 867c3707c0d4bc13ae25c9716ca106ae7b6081f3

void doMove(const std::string &move, ChessBoard &board, int moveNumber, std::shared_ptr<Piece> passedPiece)
{
    int moveSize = move.length() - 2;
    char horizontalCoord = move[moveSize] - 97;
    int verticalCoord = (move[moveSize + 1] - '0');

    board.setPiece(horizontalCoord, verticalCoord, passedPiece);
}

RowType getPieces(ChessBoard &board, std::string move, int moveNumber)
{
    int movenumber = moveNumber%2;
    RowType possiblePiece;

    for (const auto &row : board.getBoard())
    {
        for (const auto &piece : row)
        {
            if (piece->getSymbol() == toupper(move[0]) && piece->getSymbol() != 'P' && piece->getColor() == movenumber && move.length() == 3 &&
                piece->legalMove(move, board.getBoard()) == 1)
            {
                possiblePiece.emplace_back(piece);
            }
            else if (piece->getSymbol() == 'P' && piece->getColor() == movenumber && piece->legalMove(move, board.getBoard()) == 1 && pawnMove(move))
            {
                possiblePiece.emplace_back(piece);
            }
        }
    }
    return possiblePiece;
}

int main()
{
    RowType possiblePiece;
    int moveNumber = 1;
    ChessBoard board;
    int prevX, prevY, BotMove;
    char BotSide;
    std::string kingPosString, move;
    std::vector<std::string> playedMoves;
    std::vector<int> kingPos;
    std::shared_ptr<Piece> piecePtr;
    std::shared_ptr<King> kingPtr;

    std::vector<std::string> legalMoves;
    auto &b = board.getBoard();

    printBoard(board);

    std::cout << "What Side Are You Playing? [B/W] ";
    if (std::cin >> BotSide)
    {
        if (BotSide == 'B')
            BotMove = 1;
        else if (BotSide == 'W')
            BotMove = 0;
        else
        {
            std::cout << "Invalid Input" << std::endl;
            return 0;
        }
    }
    else
    {
        std::cout << "Invalid Input" << std::endl;
        return 0;
    }

    while (true)
    {
        kingPos = board.findKing(moveNumber % 2);
        piecePtr = board[kingPos[1]][kingPos[0]];
        kingPtr = std::dynamic_pointer_cast<King>(piecePtr);
        kingPosString = "K" + std::string(1, static_cast<char>(kingPos[0] + 97)) + std::to_string(8 - kingPos[1]);
        legalMoves = kingPtr->getLegal(board.getBoard());

        if (!kingPtr->inCheck(kingPosString, board.getBoard()) && mated(board, moveNumber % 2))
        {
            std::cout << "Checkmate!" << std::endl;
            moveNumber%2 == 0 ? std::cout << "White Wins!" << std::endl : std::cout << "Black Wins!" << std::endl;
            break;
        }
        if(moveNumber == 100){
            std::cout<<"Draw!"<<std::endl;
            printvector(board.getMoves());
            break;
        }

        if (BotMove == moveNumber % 2)
        {
            moveChoice(board, moveNumber % 2);
            moveNumber++;
            continue;
        }

        else{
            std::cout<<moveNumber<<std::endl;
            moveChoice(board, moveNumber % 2);
            moveNumber++;
            printBoard(board);
            continue;
        }

        moveNumber % 2 == 1 ? std::cout << "White's move" 
                : std::cout << "\033[0;01;02"
                << "m"
                << "Black's Move"
                << " "
                << "\033[m";

        std::cout<<std::endl<<std::endl;
        
        std::cout << "Enter move: ";
        std::cin >> move;
        if (move == "display")
        {
            printBoard(board);
            continue;
        }

        if (move[0] == '?' && move.length() == 4 && board.findPiece(move))
        {
            printvector(board.findPiece(move)->getLegal(board.getBoard()));
            if (board.findPiece(move)->getLegal(board.getBoard()).empty())
                std::cout << "No Legal Moves " << std::endl;
            continue;
        }
        if (move[0] == '?')
        {
            std::cout << "Not a valid piece " << std::endl;
            continue;
        }
        if (move[0] == '!' && move.length() == 4 && board.findPiece(move))
        {
            board.setPiece((board.findPiece(move)->getX() - 1), (board.findPiece(move)->getY()), std::make_shared<Piece>('.'));
            continue;
        }
        if(move[0] == '!'){
            std::cout<<"Not a valid piece "<<std::endl;
            continue;
        }

        if (!moveIsValid(move, board, moveNumber, kingPtr)) continue;

        printBoard(board);
        board.playedMovePrint();
        board.materialPrint();
        moveNumber++;
    }
    return 0;
}