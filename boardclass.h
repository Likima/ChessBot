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

    std::vector<int> findKing(int color) {
        std::vector<int> kingPosition;
        for (int i = 0; i < board.size(); ++i) {
            for (int j = 0; j < board.size(); ++j) {
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

    // Overload square bracket operator for setting elements
    const RowType& operator[](int index) const {
        return board[index];
    }


private:
    std::vector<RowType> board;
};

#endif