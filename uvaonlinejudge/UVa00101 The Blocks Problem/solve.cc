#include "../../helper/helper.1.inc"
#include <map>
#include <list>

class RobotArm {
	public:
		typedef int Block;
		typedef int Position;
		using Stack = std::list<Block>;

		RobotArm(int num_blocks) : N_(num_blocks) {
			stacks_.reserve(N_);
			for (int i = 0 ; i < N_ ; ++i) {
				stacks_.push_back( { i } );
				location_[i] = &stacks_.back();
			}
		}
		
		bool parse_cmd(const std::string& line);
		
		void move_onto(Block a, Block b) {
			if (a == b) return;

			Stack *src_stack = location_[a],
				  *dst_stack = location_[b];
			
			if (src_stack == dst_stack)
				return;

			expose(src_stack,a);
			expose(dst_stack,b);
			src_stack->pop_back();
			dst_stack->push_back(a);
			location_[a] = dst_stack;
		}
		
		void move_over(Block a, Block b) {
			if (a == b) return;

			Stack *src_stack = location_[a],
				  *dst_stack = location_[b];
			
			if (src_stack == dst_stack)
				return;

			expose(src_stack,a);
			src_stack->pop_back();
			dst_stack->push_back(a);
			location_[a] = dst_stack;
		}
		
		void pile_onto(Block a, Block b) {
			if (a == b) return;

			Stack *src_stack = location_[a],
				  *dst_stack = location_[b];
			
			if (src_stack == dst_stack)
				return;
			
			expose(dst_stack,b);

			auto   end_it = src_stack->end(),
				 start_it = std::find( src_stack->begin(), end_it, a);

			std::for_each(start_it,end_it,
						  [this,dst_stack](Block val) { location_[val] = dst_stack; } );

			dst_stack->splice(dst_stack->end(), *src_stack, start_it, end_it);
		}
		
		void pile_over(Block a, Block b) {
			if (a == b) return;

			Stack *src_stack = location_[a],
				  *dst_stack = location_[b];
			
			if (src_stack == dst_stack)
				return;

			auto   end_it = src_stack->end(),
				 start_it = std::find( src_stack->begin(), end_it, a);

			std::for_each(start_it,end_it,
						  [this,dst_stack](Block val) { location_[val] = dst_stack; } );
			
			dst_stack->splice(dst_stack->end(), *src_stack, start_it, end_it);
		}

		const Stack& stack(Position pos) const {
			return stacks_[pos];
		}

	private:
		void expose(Stack *stack, Block b) {
			Block a;
			while ( (a=stack->back()) != b) {
				stack->pop_back();
				move_to_initial(a);
			}
		}

		void move_to_initial(Block b) {
			Stack *dst_stack = &stacks_[b];

			if (!dst_stack->empty()) {
				throw "this should never happen";
			}
			
			dst_stack->push_back(b);
			location_[b] = dst_stack;
		}

		// given a block, remove it from wherever it is
		// doesn't update it's location 
		void remove_block(Block b) {
			location_[b]->remove(b);
		}

		const int N_;
		std::vector<Stack> stacks_;
		std::map<Block,Stack*> location_;
};

bool RobotArm::parse_cmd(const std::string& line) {
	if (line[0] == 'q')
		return false;
	
	int opcode = line[0]=='m'? 0 : 2;

	size_t pos;
	int a = helper::extract_positive_number(line,5,&pos);
	
	if (line[pos+2] == 'n' )
		opcode |= 1;

	int b = helper::extract_positive_number(line,pos+6,nullptr);

	switch (opcode) {
	case 0: // move over
		move_over(a,b);
		break;

	case 1: // move onto
		move_onto(a,b);
		break;

	case 2: // pile over
		pile_over(a,b);
		break;

	case 3: // pile onto
		pile_onto(a,b);
		break;
	}

	return true;
}

int main() {
	helper::LineReader<> reader;
	std::string line;

	helper::BufferedStdout output;

	reader.get_line(line);
	const int N = std::stoi(line,nullptr,10);

	RobotArm robot(N);
	
	while ( reader.get_line(line) && robot.parse_cmd(line) )
		;
	for (int i = 0; i < N ; ++i ) {
		output.append(i).append(':');
		const auto& stack = robot.stack(i);
		for (auto elem : stack) {
			output.append(' ').append(elem);
		}
		output.append('\n');
	}
}

