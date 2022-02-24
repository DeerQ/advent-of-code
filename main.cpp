#include <algorithm>
#include <exception>
#include <fstream>
#include <iostream>
#include <map>
#include <stack>
#include <vector>

class SyntaxScoring {
    const std::map<char, char> valid_pairs;
    const std::map<char, uint16_t> corruption_score_table;
    const std::map<char, uint16_t> incomplete_score_table;
    std::ifstream input_chunks;
    std::vector<std::stack<char>> incomplete_chunks;
    std::vector<char> corrupting_char;

    bool getNextLineOfChunks(std::string& next_line)
    {
        if (!input_chunks.is_open())
            throw std::runtime_error("Given file of the chunks cannot be opened!");
        std::getline(input_chunks, next_line);
        if (next_line.empty())
            return false;
        return true;
    }

    bool is_opener_char(char c)
    {
        if (valid_pairs.find(c) != valid_pairs.end())
            return true;
        return false;
    }

public:
    SyntaxScoring(const std::string& input_chunks_path)
        : valid_pairs { { '(', ')' }, { '<', '>' }, { '[', ']' }, { '{', '}' } }
        , corruption_score_table { { ')', 3 }, { '>', 25137 }, { ']', 57 }, { '}', 1197 } }
        , incomplete_score_table { { '(', 1 }, { '<', 4 }, { '[', 2 }, { '{', 3 } }
        , input_chunks { input_chunks_path }
    {
    }
    ~SyntaxScoring()
    {
        input_chunks.close();
    }

    void siftCorruptedAndIncopleteChunks()
    {
        std::string next_line_of_chunks;
        while (getNextLineOfChunks(next_line_of_chunks)) {
            std::stack<char> openers;
            bool incomplete = true;
            for (size_t i = 0; i < next_line_of_chunks.size(); i++) {
                auto current_char = next_line_of_chunks.at(i);
                if (is_opener_char(current_char)) {
                    openers.push(current_char);
                } else {
                    auto closest_opener = openers.top();
                    openers.pop();
                    if (current_char != valid_pairs.at(closest_opener)) {
                        incomplete = false;
                        corrupting_char.push_back(current_char);
                        break;
                    }
                }
            }
            if (incomplete) {
                incomplete_chunks.push_back(std::move(openers));
            }
        }
    }
    uint64_t getCorruptedScore()
    {
        uint64_t total_score = 0;
        for (auto e : corrupting_char)
            total_score += corruption_score_table.at(e);
        return total_score;
    }
    uint64_t getMiddleIncompleteScore()
    {
        std::vector<uint64_t> completion_scores;
        for (auto& openers : incomplete_chunks) {
            uint64_t completion_score = 0;
            while (openers.size()) {
                auto incomplete_opener = openers.top();
                completion_score *= 5;
                completion_score += incomplete_score_table.at(incomplete_opener);
                openers.pop();
            }
            completion_scores.push_back(completion_score);
        }
        std::sort(completion_scores.begin(), completion_scores.end());
        return completion_scores.at(completion_scores.size() / 2);
    }
};

int main()
{
    std::string input_chunk_path = "input-chunk.txt";
    SyntaxScoring syn_scoring { input_chunk_path };
    syn_scoring.siftCorruptedAndIncopleteChunks();
    auto corrupted_total_score = syn_scoring.getCorruptedScore();
    std::cerr << corrupted_total_score << std::endl;
    auto middle_score = syn_scoring.getMiddleIncompleteScore();
    std::cerr << middle_score << std::endl;

    return 0;
}
