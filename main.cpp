#include <fstream>
#include <iostream>
#include <map>
#include <stack>

int main()
{
    std::ifstream fs("input-chunk.txt");
    if (!fs.is_open())
        std::cerr << "problem with opening the input chunk!" << std::endl;

    auto is_opener = [](char c) {
        if (c == '(' || c == '[' || c == '{' || c == '<')
            return true;
        return false;
    };

    std::map<char, char> valid_pairs;
    valid_pairs['('] = ')';
    valid_pairs['<'] = '>';
    valid_pairs['['] = ']';
    valid_pairs['{'] = '}';

    std::map<char, uint16_t> score_table;
    score_table[')'] = 3;
    score_table['>'] = 25137;
    score_table[']'] = 57;
    score_table['}'] = 1197;

    std::string read;
    uint32_t total_score = 0;
    while (std::getline(fs, read)) {
        std::stack<char> openers;
        bool corrupted = false;
        for (size_t i = 0; i < read.size(); i++) {
            auto current_char = read.at(i);
            if (is_opener(current_char)) {
                openers.push(current_char);
            } else {
                auto closest_opener = openers.top();
                openers.pop();
                if (current_char != valid_pairs.at(closest_opener)) {
                    corrupted = true;
                    //                    std::cerr << "corrupted chunk is: " << read << std::endl;
                    total_score += score_table.at(current_char);
                    break;
                }
            }
        }
    }
    std::cerr << total_score << std::endl;

    return 0;
}
