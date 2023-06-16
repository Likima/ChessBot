#ifndef BOARDCLASS_H
#define BOARDCLASS_H

#include "piececlass.h"

class ChessBoard {
public:
    ChessBoard() {
        const int SIZE = 8;
        RowType SECOND_RANK, SEVENTH_RANK, EIGHTH_RANK;

        RowType FIRST_RANK{
            std::make_shared<Rook>(Black, 'R',1,8,500),
            std::make_shared<Knight>(Black, 'N',2,8,300),
            std::make_shared<Bishop>(Black, 'B',3,8,300),
            std::make_shared<Queen>(Black, 'Q',4,8,800),
            std::make_shared<King>(Black, 'K',5,8,0),
            std::make_shared<Bishop>(Black, 'B',6,8,300),
            std::make_shared<Knight>(Black, 'N',7,8,300),
            std::make_shared<Rook>(Black, 'R',8,8,500)};
        
        for(int x = 1; x<SIZE+1; x++){
            SECOND_RANK.emplace_back(std::make_shared<Pawn>(Black, 'P',x,7,10));
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
            SEVENTH_RANK.emplace_back(std::make_shared<Pawn>(White, 'P',x,2,100));
        }

        board.emplace_back(SEVENTH_RANK);

        EIGHTH_RANK.emplace_back(std::make_shared<Rook>(White, 'R',1,1,500));
        EIGHTH_RANK.emplace_back(std::make_shared<Knight>(White, 'N',2,1,300));
        EIGHTH_RANK.emplace_back(std::make_shared<Bishop>(White, 'B',3,1,300));
        EIGHTH_RANK.emplace_back(std::make_shared<Queen>(White, 'Q',4,1,800));
        EIGHTH_RANK.emplace_back(std::make_shared<King>(White, 'K',5,1,0));
        EIGHTH_RANK.emplace_back(std::make_shared<Bishop>(White, 'B',6,1,300)); 
        EIGHTH_RANK.emplace_back(std::make_shared<Knight>(White, 'N',7,1,300));
        EIGHTH_RANK.emplace_back(std::make_shared<Rook>(White, 'R',8,1,500));  

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
        board[8-(piece->getY())][(piece->getX())-1] = std::move(prevPiece); // set old position to empty
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

    std::pair<int,int> getMaterial(){
        int whiteMaterial = 0;
        int blackMaterial = 0;
        for(auto& row : board){
            for(auto& piece : row){
                if(piece->getColor() == 1 || piece->getColor() == 0)
                piece->getColor() == White ? whiteMaterial += piece->getValue() : blackMaterial += piece->getValue();
            }
        }
        return(std::make_pair(whiteMaterial, blackMaterial));
    }

    bool isEndgame(){return (getMaterial().first == 190 || getMaterial().second == 190);}


    void playedMovePrint(){
        int x = 1;
        std::cout << "Played moves: " << std::endl;
        for(auto& move : moves){
            x % 2 == 0 ? std::cout << "\033[0;35m" << move << " \033[0m" :
                std::cout << "\033[0;36m" << move << " \033[0m";
            x++;
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
    
    bool checkStalemate(){
        for(auto& row : board){
            for(auto& piece : row){
                if(piece->getSymbol() == 'R' || piece->getSymbol() == 'P' || piece->getSymbol() == 'Q' || piece->getSymbol() == 'N'){
                    return false;
                }
            }
        }
        return true;
    }

    std::string kingString(int color){
        std::string kingString = "K";
        for(auto& row : board){
            for(auto& piece : row){
                if(piece->getSymbol() == 'K' && piece->getColor() == color){
                    kingString += std::string(1, char(96+piece->getX())) + std::to_string(piece->getY());
                }
            }
        }
        return kingString;
    }

    std::shared_ptr<King> getKing(int color){
        for(auto& row : board){
            for(auto& piece : row){
                if(piece->getSymbol() == 'K' && piece->getColor() == color){
                    return std::dynamic_pointer_cast<King>(piece);
                }
            }
        }
        return std::shared_ptr<King>(nullptr);
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
        if(kingPosition.empty()) return {-1,-1};
        return kingPosition;
    }

    RowType& operator[](int index) {
        return board[index];
    }

    const RowType& operator[](int index) const {
        return board[index];
    }

    //void analyzeFen(std::string FenString){
    //    std::vector<RowType> toBoard;
    //    RowType row;
    //    for(auto& pieces : FenString){
    //        if(isdigit(pieces)){
    //            for(int x = 0; x<(pieces-'0'); x++){
    //                row.emplace_back(std::make_shared<Piece>('.'));
    //            }
    //        }
    //        else
    //    }
    //}


private:
    std::vector<RowType> board;
    std::vector<std::string> moves;
};

#endif