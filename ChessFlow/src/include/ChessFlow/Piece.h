#pragma once

#include "Square.h"

#include <array>

namespace ChessFlow {

    class Piece {

    public:
        static const int None		= 0;
        static const int King		= 1;
        static const int Pawn		= 2;
        static const int Knight		= 3;
        static const int Bishop		= 4;
        static const int Rook		= 5;
        static const int Queen		= 6;
        
        static const int White		= 0;
        static const int Black		= 8;

        static Texture KingTex;
        static Texture QueenTex;
        static Texture BishopTex;
        static Texture KnightTex;
        static Texture RookTex;
        static Texture PawnTex;

        static glm::vec2 mousePos;
        static std::string lastMove;

        static std::array<Piece, 64>* boardPtr;
        static bool flipped;

        Piece(int pieceId = White | None);

        int pieceId;
        Square sq;
        
        bool clickedOnMe;
        glm::vec2 previousPos;

        static void init();
        static void setPiece(Piece& p, int PieceId);
        void draw();
        void update();

        void onMouseDown();
        void onMouseUp();

        void setPos(glm::vec2 pos);
        int getPieceID();
        int getPieceColor();
        glm::vec2 getPos();

    };

}