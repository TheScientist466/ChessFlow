#include <iostream>
#include <process.hpp>

using namespace TinyProcessLib;
using namespace std;


int main() {
    Process process(
        "E:\\Games\\Chess\\Engines\\stockfish_15_win_x64_avx2\\stockfish", "",
        [](const char* bytes, size_t n) {
            cout << "Output from stdout: " << string(bytes, n);
        },
        [](const char* bytes, size_t n) {
            cout << "Output from stderr: " << string(bytes, n);
        },
            true);
    //process.write("uci\n");
    process.write("ucinewgame\n");
    process.write("go depth 20\n");
    //process.write("quit\n");
    cout << process.get_exit_status() << endl;

}