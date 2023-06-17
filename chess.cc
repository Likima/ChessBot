#include "piececlass.h"
#include "boardclass.h"
#include "chessalgorithm.h"
#include "printStatements.h"
#include "specialmoves.h"
#include "validmovecheck.h"

bool pawnMove(const std::string move)
/*function detecting if a given move is a pawn*/
{
    if (move.length() == 2)
        return true;
    else if (move.length() == 4 && move.find('x') != std::string::npos)
        return true;
    else if (move[0] == 'P')
        return true;
    return false;
}

void doMove(const std::string &move, ChessBoard &board, const int moveNumber, const std::shared_ptr<Piece> passedPiece)
{
    if(board.findPiece(move)->getSymbol() == 'P'){
        if(board.findPiece(move)->getY() == 8 || board.findPiece(move)->getY() == 1){
            Promote(board, board.findPiece(move), true);
        }
    }

    int moveSize = move.length();
    char horizontalCoord = move[moveSize - 2] - 97;
    int verticalCoord = (move[moveSize - 1] - '0');
    board.setPiece(horizontalCoord, verticalCoord, passedPiece);
}

RowType getPieces(std::vector<RowType> board, const std::string move, const int color)
{
    RowType possiblePiece;

    for (const auto &row : board)
    {
        for (const auto &piece : row)
        {
            if (piece->getColor() == color)
            {
                if (!pawnMove(move) && piece->getSymbol() == toupper(move[0]) && piece->getSymbol() != 'P' && piece->legalMove(move, board))
                {
                    possiblePiece.emplace_back(piece);
                }
                else if (piece->getSymbol() == 'P' && pawnMove(move) && piece->legalMove(move, board))
                {
                    possiblePiece.emplace_back(piece);
                }
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
    std::vector<std::vector<std::uint64_t>> zobristKeys = board.initZobristKeys();
    std::uint64_t hash = board.computeZobristHash(zobristKeys);

    // Print the hash value
    std::cout << "Zobrist Hash: " << hash << std::endl;

    std::vector<std::string> legalMoves;
    auto &b = board.getBoard();

    printBoard(board);

    std::cout << "\nWhat Side Are You Playing? [B/W] ";
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

    ChessAlgorithm bot(BotMove);
    ChessAlgorithm secondBot(BotMove == 1 ? 0 : 1);

    while (true)
    {
        kingPos = board.findKing(moveNumber % 2);
        if (kingPos[0] == -1)
        {
            std::cout << "King was Taken? " << std::endl;
            return -1;
        }
        piecePtr = board.getBoard()[kingPos[1]][kingPos[0]];
        kingPtr = std::dynamic_pointer_cast<King>(piecePtr);
        kingPosString = "K" + std::string(1, static_cast<char>(kingPos[0] + 97)) + std::to_string(8 - kingPos[1]);

        if (board.checkStalemate())
        {
            std::cout << "Stalemate!" << std::endl;
            break;
        }

        if (!kingPtr->inCheck(kingPosString, board.getBoard()))
        {
            if (mated(board, moveNumber % 2, kingPtr))
            {
                printvector(kingPtr->getLegal(board.getBoard()));
                std::cout << "Checkmate!" << std::endl;
                moveNumber % 2 == 0 ? std::cout << "White Wins!" << std::endl : std::cout << "Black Wins!" << std::endl;
                break;
            }
        }
        if (bot.getColor() == moveNumber % 2)
        {
            bot.moveChoice(board, moveNumber % 2, moveNumber);
            moveNumber++;
            printBoard(board);
            continue;
        }

        else{
            std::cout<<moveNumber<<std::endl;
            secondBot.moveChoice(board, moveNumber % 2, moveNumber);
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

        std::cout << std::endl
                  << std::endl;

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
            board[8 - (board.findPiece(move)->getY())][board.findPiece(move)->getX() - 1] = std::make_shared<Piece>('.');
            board.setPiece((board.findPiece(move)->getX() - 1), (board.findPiece(move)->getY()), std::make_shared<Piece>('.'));
            continue;
        }
        if (move[0] == '!')
        {
            std::cout << "Not a valid piece " << std::endl;
            continue;
        }

        if (!moveIsValid(move, board, moveNumber, kingPtr))
            continue;

        printBoard(board);
        board.playedMovePrint();
        board.materialPrint();
        moveNumber++;
    }
    return 0;
}