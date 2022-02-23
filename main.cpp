#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <stack>
#include <vector>

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

    std::map<char, uint16_t> corruption_score_table;
    corruption_score_table[')'] = 3;
    corruption_score_table['>'] = 25137;
    corruption_score_table[']'] = 57;
    corruption_score_table['}'] = 1197;

    std::map<char, uint16_t> incomplete_score_table;
    incomplete_score_table['('] = 1;
    incomplete_score_table['<'] = 4;
    incomplete_score_table['['] = 2;
    incomplete_score_table['{'] = 3;

    std::string read;
    uint32_t total_score = 0;
    std::vector<uint64_t> completion_scores;
    while (std::getline(fs, read)) {
        uint64_t completion_score = 0;
        std::stack<char> openers;
        bool incomplete = true;
        for (size_t i = 0; i < read.size(); i++) {
            auto current_char = read.at(i);
            if (is_opener(current_char)) {
                openers.push(current_char);
            } else {
                auto closest_opener = openers.top();
                openers.pop();
                if (current_char != valid_pairs.at(closest_opener)) {
                    incomplete = false;
                    //                    std::cerr << "corrupted chunk is: " << read << std::endl;
                    total_score += corruption_score_table.at(current_char);
                    break;
                }
            }
        }
        if (incomplete) {
            while (openers.size()) {
                auto incomplete_opener = openers.top();
                completion_score *= 5;
                completion_score += incomplete_score_table.at(incomplete_opener);
                openers.pop();
            }
            completion_scores.push_back(completion_score);
        }
    }
    std::sort(completion_scores.begin(), completion_scores.end());
    auto middle_score = completion_scores.at(completion_scores.size() / 2);

    fs.close();
    std::cerr << total_score << std::endl;
    std::cerr << middle_score << std::endl;

    return 0;
}
