#pragma once
#include <vector>
#include <array>

#include "Square.h"
#include "Piece.h"

namespace ChessFlow {
    class Board {
    public:
        Board();
        std::array<Square, 64> sqs;

        std::array<Piece, 64> pieces;

        bool whiteToMove;
        static glm::vec2 getPosFromBoardCoord(std::string boardPos);

        std::string moves;
        void init();
        void setFen(std::string fenString);
        std::string getFen();
        void setDefaultFen();

        void flip();
        void onMouseDown();
        void onMouseUp();

        void update();
        void draw();

        bool isLegalMove();
    };
}