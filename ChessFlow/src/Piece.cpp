#include "ChessFlow/Piece.h"

#include <plog/Log.h>
#include <unordered_map>

namespace ChessFlow {

    Texture Piece::KingTex;
    Texture Piece::QueenTex;
    Texture Piece::BishopTex;
    Texture Piece::KnightTex;
    Texture Piece::RookTex;
    Texture Piece::PawnTex;

    glm::vec2 Piece::mousePos;

    std::string Piece::lastMove = "";

    std::array<Piece, 64>* Piece::boardPtr;
    std::string* Piece::movesPtr;
    bool Piece::flipped = false;
    bool* Piece::whiteToMovePtr = nullptr;
    Engines* Piece::enginePtr;
    int* Piece::engineDepth;

    Piece::Piece(int _pID) {
        clickedOnMe = false;
        setPiece(*this, _pID);
        sq.useTexture = true;
        sq.setPosition(glm::vec3(0, 7, 0));
        previousPos = glm::vec2(sq.position.x, sq.position.y);
    }

    void Piece::init() {
        Piece::KingTex.loadFromFile("Assets/King.png");
        Piece::QueenTex.loadFromFile("Assets/Queen.png");
        Piece::BishopTex.loadFromFile("Assets/Bishop.png");
        Piece::KnightTex.loadFromFile("Assets/Knight.png");
        Piece::RookTex.loadFromFile("Assets/Rook.png");
        Piece::PawnTex.loadFromFile("Assets/Pawn.png");
    }

    void Piece::setPiece(Piece& p, int pieceId) {
        p.pieceId = pieceId;
        switch(pieceId & ~8) {
        case Piece::King:
            p.sq.tex = &KingTex;
            break;
        case Piece::Queen:
            p.sq.tex = &QueenTex;
            break;
        case Piece::Bishop:
            p.sq.tex = &BishopTex;
            break;
        case Piece::Knight:
            p.sq.tex = &KnightTex;
            break;
        case Piece::Rook:
            p.sq.tex = &RookTex;
            break;
        default:
            p.sq.tex = &PawnTex;
            break;
        }

        p.sq.toInvertColor = p.getPieceColor();
    }

    int Piece::getPieceIdFromSymbol(char symbol) {
        std::unordered_map<char, int> pieceIdFromSymbol{
           {'k', Piece::King},
           {'q', Piece::Queen},
           {'b', Piece::Bishop},
           {'n', Piece::Knight},
           {'r', Piece::Rook},
           {'p', Piece::Pawn}
        };
        return pieceIdFromSymbol[symbol];
    }

    char Piece::getSymbolFromPieceId(int Id)
    {
        std::unordered_map<char, int> pieceIdFromSymbol{
           {Piece::King,    'k'},
           {Piece::Queen,   'q'},
           {Piece::Bishop,  'b'},
           {Piece::Knight,  'n'},
           {Piece::Rook,    'r'},
           {Piece::Pawn,    'p'}
        };
        return pieceIdFromSymbol[Id];
    }

    void Piece::draw() {
        if((pieceId & 0b0111) != None)
            sq.draw();
    }

    void Piece::update() {
        if(clickedOnMe)
            sq.setPosition(glm::vec3(glm::clamp(mousePos - glm::vec2(0.5, 0.5), glm::vec2(0, 0), glm::vec2(7, 7)), 0.f));
    }

    void Piece::onMouseDown() {
        if(glm::vec3(glm::floor(mousePos), 0) == sq.position) {
            clickedOnMe = true;
            previousPos = glm::vec2(sq.position.x, sq.position.y);
        }
    }

    void Piece::onMouseUp() {
        glm::vec2 finalPos = glm::floor(getPos() + glm::vec2(0.5, 0.5));
        if(flipped)
            finalPos = glm::vec2(7, 7) - finalPos;
        if(clickedOnMe) {
            clickedOnMe = false;
            Piece* destinationPiece = &(*boardPtr)[finalPos.x + finalPos.y * 8];
            if((destinationPiece->getPieceID() == Piece::None || destinationPiece->getPieceColor() != getPieceColor()) && getPieceColor() ^ *whiteToMovePtr) {
                setPos(previousPos);
                setPiece(*destinationPiece, pieceId);
                setPiece((*boardPtr)[previousPos.x + previousPos.y * 8], Piece::None);
                if(previousPos != finalPos) {
                    lastMove = "";
                    lastMove += ((char)('a' + previousPos.x));
                    lastMove += (char)(previousPos.y + 1 + '0');
                    lastMove += ((char)('a' + finalPos.x));
                    lastMove += (char)(finalPos.y + 1 + '0');
                    *movesPtr += (" " + lastMove);
                    PLOGW << *movesPtr;
                    *whiteToMovePtr = !(*whiteToMovePtr);
                    if(enginePtr != nullptr) {
                        enginePtr->writeToStdIn(("position startpos moves" + *movesPtr));
                        enginePtr->goDepth(*engineDepth);
                    }
                }
            }
            else
                setPos(previousPos);
        }
    }

    void Piece::setPos(glm::vec2 pos) {
        sq.setPosition(glm::vec3(pos, 0.f));
    }

    int Piece::getPieceID() {
        return pieceId & 0b0111;
    }

    int Piece::getPieceColor() {
        return (pieceId & 0b1000) >> 3;
    }

    glm::vec2 Piece::getPos() {
        return glm::vec2(sq.position.x, sq.position.y);
    }
}