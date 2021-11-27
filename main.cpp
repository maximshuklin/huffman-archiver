#include "ArgumentParser.h"
#include "Help.h"
#include "Encoder.h"
#include "Decoder.h"

#include <iostream>

void OutputHelp() {
	std::cout << HelpMessage << std::endl;
}

void GatherAll(const ArgumentParser &argument_parser) {
	if (argument_parser.IsHelp()) {
		OutputHelp();
	} else if (argument_parser.IsEncoder()) {
		std::string archive_name = argument_parser.GetArchiveName();
		std::vector<std::string> files_to_archive = argument_parser.GetFilesToArchive();

		Encoder encoder = Encoder();
		encoder.CompressFilesIntoArchive(files_to_archive, archive_name);

		std::cerr << "Files were succesfully archived!" << std::endl;
	} else if (argument_parser.IsDecoder()) {
		std::string archive_name = argument_parser.GetArchiveName();

		std::ifstream inf(archive_name);
		Decoder decoder = Decoder(inf);
		decoder.Decode();
		inf.close();

		std::cerr << "Files were succesfully unziped!" << std::endl;
	} else {
		throw std::runtime_error("Invalid command. To get help run: ./archiver -h");
	}
}

int main(int argc, char const *argv[]) {
	ArgumentParser argument_parser;
	try {
		argument_parser = ArgumentParser(argc, argv);
		GatherAll(argument_parser);
	} catch (std::exception &e) {
		std::cerr << e.what() << std::endl;
		return -1;
	}
	return 0;
}
