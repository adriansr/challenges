#include "inputstate.h"
#include <iostream>

int main(int argc, char *argv[]) {
	
	if (argc != 1) {
		std::cerr << "Usage: " << argv[0] << " < input_file [ > output_file ]\n"
				  << "\n"
				  << "Don't pass any command-line arguments to this program.\n"
				  << "It reads input from standard-input and writes output to standard-output\n"
				  << "\n";
		return 1;
	}
	badoolization::InputState initial_state,
							  final_state;

	if (! initial_state.read_from(std::cin)) {
		std::cerr << "Failed to read initial state from standard-input (Wrong format)\n";
		return 1;
	}

	if (! final_state.read_from(std::cin)) {
		std::cerr << "Failed to read final state from standard-input (Wrong format)\n";
		return 1;
	}
	
	initial_state.log(std::cerr,"initial");
	final_state.log(std::cerr,"final");

}

