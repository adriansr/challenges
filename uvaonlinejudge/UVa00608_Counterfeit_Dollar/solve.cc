#include "../../helper/helper.1.inc"

#define NCOINS 12

#define	FLAG_IS_LIGHT 0
#define	FLAG_IS_HEAVY 1
#define FLAG_IS_EVEN  2
#define NUM_FLAGS	  3

struct C {
	int flags[NCOINS][NUM_FLAGS];
} clear_coins, coins;

void set(const char *s, int flag1, int flag2) {
	char c;
	while ( (c=*s++) != ' ') {
		coins.flags[ c - 'A' ][flag1] ++;
	}
	while ( (c=*s++) != ' ') {
		coins.flags[ c - 'A' ][flag2] ++;
	}
}

helper::LineReader<> reader;
std::string line;

#define MSG_LEN 43
//                          1234567890123456789012345678901234567890123
char msg[3][MSG_LEN+1] = { "K is the counterfeit coin and it is light.\n" ,
   			   	           "K is the counterfeit coin and it is heavy.\n"}; 

void output(int coin_idx, int flag) {
	char *sel_msg = msg[flag]; // heavy by default ?
	sel_msg[0] = 'A' + coin_idx;
	::write(1,sel_msg,MSG_LEN);
}

void process_weight(int test_num) {
	reader.get_line(line);
	//std::cout << "Test case " << test_num << " : " << line << "\n";
	char opcode = line[ line.size()-2 ];
	int flag1,flag2;

	switch(opcode) {
		case 'e':
			flag1 = flag2 = FLAG_IS_EVEN;
			break;

		case 'w':
			flag1 = FLAG_IS_LIGHT;
			flag2 = FLAG_IS_HEAVY;
			break;

		case 'u':
			flag1 = FLAG_IS_HEAVY;
			flag2 = FLAG_IS_LIGHT;
			break;
	}
	set(line.data(),flag1,flag2);
}

int main() {
	
	reader.get_line(line);
	
	const int num_cases = std::stol(line,nullptr,10);
	// for each test case
	for (int test_num=0;test_num < num_cases;++test_num) {
		if (test_num) {
			//reader.get_line(line);
			coins = clear_coins;
		}
		process_weight(test_num);
		process_weight(test_num);
		process_weight(test_num);
		
#if 0
		for (int i=0;i<NCOINS;++i) {
			std::cout << char('A' + i);
		}
		std::cout << "\n"; 
		for (int i=0;i<NCOINS;++i) {
			std::cout << char('0' + coins.flags[i][FLAG_IS_EVEN]);
		}
		std::cout << "\n"; 

		for (int i=0;i<NCOINS;++i) {
			std::cout << char('0' + coins.flags[i][FLAG_IS_LIGHT]);
		}
		std::cout << "\n"; 

		for (int i=0;i<NCOINS;++i) {
			std::cout << char('0' + coins.flags[i][FLAG_IS_HEAVY]);
		}
		std::cout << "\n"; 
#endif


		struct {
			int idx, max;
		} light, heavy;

		light.max = 0;
		heavy.max = 0;

		for (int i = 0; i < NCOINS; ++i) {
			const auto & r = coins.flags[i];
			if ( !r[FLAG_IS_EVEN] ) {
				int level;
				if ((level=r[FLAG_IS_HEAVY]) > heavy.max) {
					heavy.idx = i;
					heavy.max = level;
				}
				if ((level=r[FLAG_IS_LIGHT]) > light.max) {
					light.idx = i;
					light.max = level;
				}
			}
		}
		
		if (light.max > heavy.max) {
			output(light.idx, FLAG_IS_LIGHT);
		}
		else if (light.max < heavy.max) {
			output(heavy.idx, FLAG_IS_HEAVY);
		}
		else {
		// fallback if coin not referenced
			for (int i = 0; i < NCOINS; ++i) {
				const auto & r = coins.flags[i];
				if ( !r[FLAG_IS_EVEN] ) {
					output(i,FLAG_IS_HEAVY); 
					break;
				}
			}
		}
	}
}

