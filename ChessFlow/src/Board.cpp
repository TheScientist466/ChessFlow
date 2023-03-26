#include "ChessFlow/Board.h"

#include <plog/Log.h>
#include <unordered_map>

namespace ChessFlow {
    Board::Board() {
        //sqs = new std::vector<Square>;
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
                //PLOGV << sqs->size();
            }
        }
        for(int i = 0; i < pieces.size(); i++) {
            pieces[i] = Piece(Piece::None);
        }

        setDefaultFen();

        for(int i = 0; i < pieces.size(); i++) {
            pieces[i].setPos(glm::vec2(i % 8, i / 8));
        }
    }

    void Board::init() {
        Piece::boardPtr = &pieces;
    }

    void Board::setFen(std::string fenString) {
        for(Piece& p : pieces) {
            Piece::setPiece(p, Piece::None);
        }

        std::unordered_map<char, int> pieceIdFromSymbol{
            {'k', Piece::King},
            {'q', Piece::Queen},
            {'b', Piece::Bishop},
            {'n', Piece::Knight},
            {'r', Piece::Rook},
            {'p', Piece::Pawn}
        };

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
                    int pieceType = pieceIdFromSymbol[(char)tolower(symbol)];
                    Piece::setPiece(pieces[file + rank * 8], pieceColor | pieceType);
                    file++;
                }
            }
        }
    }

    void Board::setDefaultFen() {
        setFen("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    }

    glm::vec2 Board::getPosFromBoardCoord(std::string boardPos)
    {
        return glm::vec2((float)(boardPos.c_str()[0] - 'a'), (float)(boardPos.c_str()[1] - '0' - 1));
    }

    void Board::flip() {
        for(auto& s : sqs) {
            s.setPosition(glm::vec3(s.position.x, 7.f - s.position.y, 0.f));
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