#include "ChessFlow/Engines.h"

#include <iostream>

#include <plog/Log.h>

Engines::Engines(std::string path)
{
	engineProcess = new TinyProcessLib::Process(path, "",
		[&](const char* bytes, std::size_t n) {
			std::string k(bytes, n);
			processStdOutLast = k;
			processStdOut += k;
#ifndef NDEBUG
			PLOGI << k;
#endif
		}, nullptr, true);
	//for(long i = 0; i < 2000000000; i++);
	engineProcess->write("uci\n");
	engineProcess->write("ucinewgame\n");
}

Engines::~Engines() {
	engineProcess->close_stdin();
	PLOGV << "Engine exit status " << engineProcess->get_exit_status();
	delete engineProcess;
}

void Engines::loadEngine(std::string path) {
}

void Engines::writeToStdIn(std::string toWrite) {
	engineProcess->write(toWrite + "\n");
}

std::string Engines::readFromStdOut() {
	return "";
}