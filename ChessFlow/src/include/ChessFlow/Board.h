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

        static glm::vec2 getPosFromBoardCoord(std::string boardPos);

        void init();
        void setFen(std::string fenString);
        void setDefaultFen();

        void flip();
        void onMouseDown();
        void onMouseUp();

        void update();
        void draw();

        bool isLegalMove();
    };
}