#include "ChessFlow/Piece.h"
#include <plog/Log.h>

namespace ChessFlow {

    Texture Piece::KingTex;
    Texture Piece::QueenTex;
    Texture Piece::BishopTex;
    Texture Piece::KnightTex;
    Texture Piece::RookTex;
    Texture Piece::PawnTex;

    glm::vec2 Piece::mousePos;

    std::string Piece::lastMove = "";

    Piece::Piece(int _pID) {
        pieceId = _pID;
        clickedOnMe = false;
        setTex(pieceId, *this);
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

    void Piece::setTex(int pieceId, Piece& p) {
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

        if((pieceId & Piece::Black) == Piece::Black)
            p.sq.toInvertColor = true;
    }

    void Piece::draw() {
        if(pieceId != None)
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
        if(clickedOnMe) {
            clickedOnMe = false;
            sq.setPosition(glm::floor(sq.position + glm::vec3(0.5, 0.5, 0)));
            if(previousPos != glm::vec2(sq.position.x, sq.position.y)) {
                lastMove = "";
                //lastMove += 'B';
                lastMove += ((char)('a' + previousPos.x));
                lastMove += (char)(previousPos.y + 1 + '0');
                lastMove += ((char)('a' + sq.position.x));
                lastMove += (char)(sq.position.y + 1 + '0');
                //PLOGW << "Move B" << (char)('a' + previousPos.x) << previousPos.y + 1 << (char)('a' + sq.position.x) << sq.position.y + 1;
            }
        }
    }

    void Piece::setPos(glm::vec2 pos) {
        sq.setPosition(glm::vec3(pos, 0.f));
    }

    glm::vec2 Piece::getPos() {
        return glm::vec2(sq.position.x, sq.position.y);
    }
}