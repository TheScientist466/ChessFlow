#include "ChessFlow/Board.h"

#include <plog/Log.h>

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
        Piece WK(Piece::White | Piece::King);
        WK.setPos(getPosFromBoardCoord("e1"));
        pieces.push_back(WK);
        Piece WQ(Piece::White | Piece::Queen);
        WQ.setPos(getPosFromBoardCoord("d1"));
        pieces.push_back(WQ);

        Piece WB(Piece::White | Piece::Bishop);
        WB.setPos(getPosFromBoardCoord("c1"));
        pieces.push_back(WB);
        WB.setPos(getPosFromBoardCoord("f1"));
        pieces.push_back(WB);

        Piece WN(Piece::White | Piece::Knight);
        WN.setPos(getPosFromBoardCoord("b1"));
        pieces.push_back(WN);
        WN.setPos(getPosFromBoardCoord("g1"));
        pieces.push_back(WN);

        Piece WR(Piece::White | Piece::Rook);
        WR.setPos(getPosFromBoardCoord("a1"));
        pieces.push_back(WR);
        WR.setPos(getPosFromBoardCoord("h1"));
        pieces.push_back(WR);

        Piece WP(Piece::White | Piece::Pawn);
        for(int i = 0; i < 8; i++) {
            WP.setPos(glm::vec2((float)i, 1));
            pieces.push_back(WP);
        }



        Piece BK(Piece::Black | Piece::King);
        BK.setPos(getPosFromBoardCoord("e8"));
        pieces.push_back(BK);
        Piece BQ(Piece::Black | Piece::Queen);
        BQ.setPos(getPosFromBoardCoord("d8"));
        pieces.push_back(BQ);

        Piece BB(Piece::Black | Piece::Bishop);
        BB.setPos(getPosFromBoardCoord("c8"));
        pieces.push_back(BB);
        BB.setPos(getPosFromBoardCoord("f8"));
        pieces.push_back(BB);

        Piece BN(Piece::Black | Piece::Knight);
        BN.setPos(getPosFromBoardCoord("b8"));
        pieces.push_back(BN);
        BN.setPos(getPosFromBoardCoord("g8"));
        pieces.push_back(BN);

        Piece BR(Piece::Black | Piece::Rook);
        BR.setPos(getPosFromBoardCoord("a8"));
        pieces.push_back(BR);
        BR.setPos(getPosFromBoardCoord("h8"));
        pieces.push_back(BR);

        Piece BP(Piece::Black | Piece::Pawn);
        for(int i = 0; i < 8; i++) {
            BP.setPos(glm::vec2((float)i, 6));
            pieces.push_back(BP);
        }
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

        for(auto& p : pieces)
            p.draw();

        for(auto& p : pieces)
            if(p.clickedOnMe) {
                p.draw();
                break;
            }
    }

    void Board::update() {
        for(auto& p : pieces) {
            p.update();
        }
    }
}