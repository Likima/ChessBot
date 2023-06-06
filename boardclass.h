#ifndef BOARDCLASS_H
#define BOARDCLASS_H

#include "piececlass.h"


class ChessBoard {
public:
    ChessBoard() {
        const int SIZE = 8;
        RowType SECOND_RANK;
        RowType SEVENTH_RANK;
        RowType EIGHTH_RANK;

        RowType FIRST_RANK{
            std::make_shared<Rook>(Black, 'R',1,8,5),
            std::make_shared<Knight>(Black, 'N',2,8,3),
            std::make_shared<Bishop>(Black, 'B',3,8,3),
            std::make_shared<Queen>(Black, 'Q',4,8,8),
            std::make_shared<King>(Black, 'K',5,8,0),
            std::make_shared<Bishop>(Black, 'B',6,8,3),
            std::make_shared<Knight>(Black, 'N',7,8,3),
            std::make_shared<Rook>(Black, 'R',8,8,5)};
        
        for(int x = 1; x<SIZE+1; x++){
            SECOND_RANK.emplace_back(std::make_shared<Pawn>(Black, 'P',x,7,1));
        }
        
        board.emplace_back(std::move(FIRST_RANK));
        board.emplace_back(std::move(SECOND_RANK));

        for (int i = 2; i < SIZE - 2; ++i) {
            RowType BLANK_RANK; 
            for(int x = 0; x<SIZE; x++){
                BLANK_RANK.emplace_back(std::make_shared<Piece>(-1, '.', x+1, 8-i));
            }
            board.emplace_back(std::move(BLANK_RANK));
        }

        for(int x = 1; x<SIZE+1; x++){
            SEVENTH_RANK.emplace_back(std::make_shared<Pawn>(White, 'P',x,2,1));
        }

        board.emplace_back(SEVENTH_RANK);

        EIGHTH_RANK.emplace_back(std::make_shared<Rook>(White, 'R',1,1,5));
        EIGHTH_RANK.emplace_back(std::make_shared<Knight>(White, 'N',2,1,3));
        EIGHTH_RANK.emplace_back(std::make_shared<Bishop>(White, 'B',3,1,3));
        EIGHTH_RANK.emplace_back(std::make_shared<Queen>(White, 'Q',4,1,8));
        EIGHTH_RANK.emplace_back(std::make_shared<King>(White, 'K',5,1,0));
        EIGHTH_RANK.emplace_back(std::make_shared<Bishop>(White, 'B',6,1,3)); 
        EIGHTH_RANK.emplace_back(std::make_shared<Knight>(White, 'N',7,1,3));
        EIGHTH_RANK.emplace_back(std::make_shared<Rook>(White, 'R',8,1,5));  

        board.emplace_back(EIGHTH_RANK);
    }


    const std::vector<RowType>& getBoard() const {return board;}
    const std::vector<std::string>& getMoves() const {return moves;}

    void setMoves(std::string move){moves.emplace_back(move);}

    void setMoves(std::shared_ptr<Piece> piece){
        if(piece->getSymbol() == 'P') moves.emplace_back(std::string(1, char(97+piece->getX()-1)) + std::to_string(piece->getY()));
        else moves.emplace_back(std::string(1, piece->getSymbol()) + std::string(1, 97+piece->getX()-1) + std::to_string(piece->getY()));
    }
    
    void setPiece(int x, int y, std::shared_ptr<Piece> piece, std::shared_ptr<Piece> prevPiece = nullptr) {
        if(prevPiece == nullptr){
            prevPiece = std::make_shared<Piece>(-1, '.', piece->getX(), piece->getY());
        }
        board[8-(piece->getY())][(piece->getX())-1] = prevPiece; // set old position to empty
        piece->setX(x+1); // set new position for the piece
        piece->setY(y);
        board[8-y][x] = std::move(piece); // move the piece to the new position
    }

    void materialPrint(){
        int whiteMaterial = 0;
        int blackMaterial = 0;
        std::cout << "Material: " << std::endl;
        for(auto& row : board){
            for(auto& piece : row){
                if(piece->getColor() == 1 || piece->getColor() == 0)
                piece->getColor() == White ? whiteMaterial += piece->getValue() : blackMaterial += piece->getValue();
            }
        }
        std::cout << "White: " << whiteMaterial << std::endl;
        std::cout << "Black: " << blackMaterial << std::endl;
    }

    void playedMovePrint(){
        std::cout << "Played moves: " << std::endl;
        for(auto& move : moves){
            std::cout << move << " ";
        }
        std::cout << std::endl;
    }

    std::shared_ptr<Piece> findPiece(std::string location){
        char pieceX = location[location.size()-2]-97;
        char pieceY = 8-(location[location.size()-1]-'0');

        for(int i = 0; i<board.size(); ++i){
            for(int j = 0; j< board.size(); ++j){
                if(i == pieceY && j == pieceX){
                    return board[i][j];
                }
            }
        }
    return std::shared_ptr<Piece>(nullptr);
    }
    std::shared_ptr<Piece> findPiece(int x, int y){
        return board[8-y][x-1];
    }

    std::vector<int> findKing(int color) {
        std::vector<int> kingPosition;
        for (auto i = 0; i < board.size(); i++) {
            for (auto j = 0; j < board.size(); j++) {
                if (board[i][j]->getSymbol() == 'K' && board[i][j]->getColor() == color) {
                    kingPosition.emplace_back(j);
                    kingPosition.emplace_back(i);
                    return kingPosition;
                }
            }
        }
        return kingPosition;
    }

    RowType& operator[](int index) {
        return board[index];
    }

    const RowType& operator[](int index) const {
        return board[index];
    }


private:
    std::vector<RowType> board;
    std::vector<std::string> moves;
};

#endif