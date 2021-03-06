#include "../../helper/helper.1.h"
#include <array>
#include <bitset>
#include <iostream>
#include <map>
#include <string>
#include <vector>

using S = std::string;

class Problem {
    public:
        Problem(S&& op1, S&& op2, S&& res, char op) {
            //std::cerr << "op: " << op1 << op << op2 << '=' << res << '\n';
            isAdd_ = op == '+';
            std::swap(operand_[0], op1);
            std::swap(operand_[1], op2);
            std::swap(operand_[2], res);
            int numStages = std::max(operand_[0].size(), operand_[1].size());
            stage_.resize(numStages);
            for (const S& e : operand_) {
                prepare_operands_(e);
            }
            prepare_stages_(operand_[0]);
            prepare_stages_(operand_[1]);
        }
        
        std::string solve() {
            if (isAdd_) {
                bool allOne = true;
                for (const auto& op : operand_) {
                    allOne &=  op.size() == 1;
                }
                if (allOne) {
                    return "0 + 0 = 0";
                }
            }

            bool solved = solve_(0, 0, 0);
            if (solved) {
                std::string result(std::to_string(subst_(0)));
                result.append(isAdd_? " + " : " * ");
                result.append(std::to_string(subst_(1)));
                result.append(" = ");
                result.append(std::to_string(subst_(2)));
                return result;
            }
            else
                return "Invalid input";
        }

    private:

        bool solve_(const int level, const int pos, const int mask) {

            if (level < stage_.size()) {
                if (pos < stage_[level].size()) {

                    // check
                    char cur = stage_[level][pos];
                    if (mapping_[cur] != -1) {
                        return solve_(level, pos+1, mask);
                    }

                    // bruteforce
                    
                    for (int i=noZero_[cur]; i<10; ++i) {
                        if ( ! (mask & (1<<i)) ) {
                            mapping_[cur] = i;
                            if (solve_(level, pos+1, mask | (1<<i))) {
                                return true;
                            }
                        }
                    }
                    mapping_[cur] = -1;
                    return false;

                } else {
                    int64_t val[2];
                    for (int i=0;i<2;++i) {
                        val[i] = subst_(operand_[i], level);
                    }
                    int64_t res = op_(val[0], val[1]);
                    char unk = operand_[2][operand_[2].size()-1-level];
                    int digit = (res / pow10_(level)) % 10;
                    bool mapped;
                    if ( (mapped=mapping_[unk]==-1)) {
                        if ( (mask & (1 << digit)) || (digit==0 && noZero_[unk])) return false;
                        mapping_[unk] = digit;
                    } else {
                        if (mapping_[unk] != digit) return false;
                    }
                    if (solve_(level+1, 0, mask | (1 << digit))) {
                        return true;
                    }
                    if (mapped) mapping_[unk] = -1;
                    return false;
                }
            } else {
                int64_t val[3];
                for (int i=0; i<2; ++i) {
                    val[i] = subst_(i);
                }
                int64_t target = op_(val[0], val[1]);
                
                std::string mapped_here;

                for (int lvl=0; lvl<operand_[2].size(); ++lvl) {
                    char unk = operand_[2][operand_[2].size() - lvl - 1];
                    if (mapping_[unk] == -1) {
                        int digit = (target / pow10_(lvl)) % 10;
                        if ( (mask & (1 << digit)) || (digit==0 && noZero_[unk])) {
                            for (char c : mapped_here) mapping_[c] = -1;
                            return false;
                        }
                        mapping_[unk] = digit;
                        mapped_here.push_back(unk);
                    }
                }
                
                val[2] = subst_(2);
                bool result = target == val[2];
                if (!result) {
                    for (char c : mapped_here) mapping_[c] = -1;
                }
                return result;
            }
        }
        
        static int64_t pow10_(int level) {
            switch(level) {
                case  0: return 1;
                case  1: return 10;
                case  2: return 100;
                case  3: return 1000;
                case  4: return 10000;
                case  5: return 100000;
                case  6: return 1000000;
                case  7: return 10000000;
                case  8: return 100000000;
                case  9: return 1000000000;
                case 10: return 10000000000L;
                case 11: return 100000000000L;
                case 12: return 1000000000000L;
                case 13: return 10000000000000L;
                case 14: return 100000000000000L;
                case 15: return 1000000000000000L;
                case 16: return 10000000000000000L;
                case 17: return 100000000000000000L;
                case 18: return 1000000000000000000L;
                default: return 1;
            }
        }

        int64_t subst_(int oper) const {
            return subst_(operand_[oper], operand_[oper].size()-1);
        }
        
        int64_t subst_(const S& op, int level) const {
            int64_t num = 0;
            int end = op.size();
            int start = std::max(0, end - level - 1);
            for (int i=start; i<end; ++i) {
                num = num*10 + mapping_[op[i]];
            }
            return num;
        }
        
        int64_t op_(int64_t a, int64_t b) const {
            return isAdd_? a + b : a * b;
        }

        void prepare_operands_(const S& operand) {
            for (char c : operand) {
                mapping_[c] = -1;
            }
            noZero_[operand[0]] = true;
        }

        void prepare_stages_(const S& operand) {
            int s = operand.size() - 1;
            for (char c : operand) {
                stage_[s--].push_back(c);
            }
        }

        S operand_[3];
        bool isAdd_;
        std::array<int, 256> mapping_;
        std::bitset<256> noZero_;
        std::vector<S> stage_;
};

constexpr int MAX_INPUT_LINE_LENGTH = 8192;

helper::BufferedStdout output;
helper::LineReader<MAX_INPUT_LINE_LENGTH> reader; // <-- check or RE

int main() {

    S line, operand1, operand2, result, operation, dummy;

    operation.resize(1);

    while (reader(line)) {
        int pos = line.find(' ');
        operand1.assign(line, 0, pos);
        operation[0] = line[pos+1];
        int pos2 = line.find('=', pos);
        operand2.assign(line, pos+3, pos2 - 1 - (pos+3));
        result.assign(line,pos2+2, line.size() - (pos2+2));
        Problem p(std::move(operand1), std::move(operand2), std::move(result), operation[0]);
        std:: cout << p.solve() << '\n';
    }
}

