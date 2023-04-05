#include "ChessFlow/Board.h"

#include <plog/Log.h>
#include <unordered_map>
#include <string>

namespace ChessFlow {
    Board::Board() {
        PLOGV << "Creating Chessboard";
        int ctr = 0;
        for(unsigned int file = 0; file < 8; file++) {
            for(unsigned int rank = 0; rank < 8; rank++) {
                Square s;
                s.setPosition(glm::vec3((float)file, (float)rank, 0.f));
                s.color = (file + rank) % 2 != 0 ? &ChessFlow::Square::lightColor : &ChessFlow::Square::darkColor;
                s.id = ctr;
                ctr++;
                sqs[file * 8 + rank] = s;
            }
        }
        for(int i = 0; i < pieces.size(); i++) {
            pieces[i] = Piece(Piece::None);
        }

        setDefaultFen();

        for(int i = 0; i < pieces.size(); i++) {
            pieces[i].setPos(glm::vec2(i % 8, i / 8));
        }

        moves = "";
    }

    void Board::init() {
        Piece::boardPtr = &pieces;

        Piece::movesPtr = &moves;
        Piece::whiteToMovePtr = &whiteToMove;
        whiteToMove = true;
    }

    void Board::setFen(std::string fenString) {
        for(Piece& p : pieces) {
            Piece::setPiece(p, Piece::None);
        }

        std::string fenBoard = fenString.substr(0, fenString.find_first_of(' '));
        int file = 0, rank = 7;
        for(char symbol : fenBoard) {
            if(symbol == '/') {
                file = 0;
                rank--;
            }
            else {
                if(isdigit(symbol)) {
                    file += symbol - '0';
                }
                else {
                    int pieceColor = isupper(symbol) ? Piece::White : Piece::Black;
                    int pieceType = Piece::getPieceIdFromSymbol((char)tolower(symbol));
                    Piece::setPiece(pieces[file + rank * 8], pieceColor | pieceType);
                    file++;
                }
            }
        }
        whiteToMove = fenString.c_str()[fenString.find_first_of(' ') + 1] == 'w';
    }

    std::string Board::getFen() {
        std::string fen = "";
        int ctr = 0;
        for(int rank = 7; rank >= 0; rank--) {
            for(int file = 0; file < 8; file++) {
                if(pieces[file + rank * 8].getPieceID() == Piece::None)
                    ctr++;
                else {
                    if(ctr != 0)
                        fen += ctr + '0';
                    ctr = 0;
                    char symbol = Piece::getSymbolFromPieceId(pieces[file + rank * 8].getPieceID());
                    if(pieces[file + rank * 8].getPieceColor())
                        fen += symbol;
                    else
                        fen += toupper(symbol);
                }
            }
            if(ctr != 0)
                fen += ctr + '0';
            ctr = 0;
            if(rank > 0)
                fen += "/";
        }

        fen += " ";
        fen += whiteToMove ? 'w' : 'b';
        return fen;
    }

    void Board::setDefaultFen() {
        setFen("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    }

    glm::vec2 Board::getPosFromBoardCoord(std::string boardPos)
    {
        return glm::vec2((float)(boardPos.c_str()[0] - 'a'), (float)(boardPos.c_str()[1] - '0' - 1));
    }

    void Board::flip() {
        Piece::flipped = !Piece::flipped;
        for(auto& p : pieces) {
            p.setPos(glm::vec2(7, 7) - p.getPos());
        }
    }

    void Board::onMouseDown() {
        for(auto& p : pieces)
            p.onMouseDown();
    }

    void Board::onMouseUp() {
        for(auto& p : pieces) {
            p.onMouseUp();
        }
    }

    void Board::draw() {

        for(auto& s : sqs)
            s.draw();

        Piece* clickedOnPiece = nullptr;

        for(auto& p : pieces) {
            p.draw();
            if(p.clickedOnMe)
                clickedOnPiece = &p;
        }

        if(clickedOnPiece != nullptr)
            clickedOnPiece->draw();
    }

    void Board::update() {
        for(auto& p : pieces) {
            p.update();
        }
    }
}