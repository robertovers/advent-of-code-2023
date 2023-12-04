#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <regex>
#include <set>
#include <map>
#include <tuple>


bool read_file(std::string filename, std::vector<std::string>& lines) {
    std::ifstream file;
    file.open(filename);

    if (not file.is_open()) return false;

    std::string line;
    while (getline(file, line)) {
        lines.push_back(line);
    }

    file.close();

    return true;
}


int part_one(std::vector<std::string>& lines) {
    int result = 0;

    for (std::string line: lines) {

        std::stringstream ss(line);
        std::set<int> w_numbers;

        line = line.substr(10, line.size());

        int delim = line.find("|");

        for (int i=0; i<delim; i+=3) {
            std::string num_s = line.substr(i, 3);
            int num = stoi(num_s);
            w_numbers.insert(num);
        }

        int score = 0;
        for (int i=delim+2; i<line.size(); i+=3) {
            std::string num_s = line.substr(i, 3);
            int num = stoi(num_s);
            if (w_numbers.find(num) != w_numbers.end()) {
                if (score == 0) {
                    score += 1;
                } else {
                    score *= 2;
                }
            }
        }
        result += score;
    }

    return result;
}


int part_two(std::vector<std::string>& lines) {
    int result = 0;
    int id = 1;

    std::map<int, std::pair<std::set<int>, std::set<int> > > cards;
    std::map<int, int> counts;

    for (std::string line: lines) {
        std::stringstream ss(line);
        std::set<int> w_numbers;
        std::set<int> my_numbers;

        line = line.substr(10, line.size());

        int delim = line.find("|");

        for (int i=0; i<delim; i+=3) {
            std::string num_s = line.substr(i, 3);
            int num = stoi(num_s);
            w_numbers.insert(num);
        }
        
        for (int i=delim+2; i<line.size(); i+=3) {
            std::string num_s = line.substr(i, 3);
            int num = stoi(num_s);
            my_numbers.insert(num);
        }

        cards[id] = std::make_pair(w_numbers, my_numbers);
        counts[id] = 1;
        id++;
    }

    const int ID_MAX = id;

    int current = 1;
    while (current <= ID_MAX) {
        int copies = counts[current];
        int next_card = current+1;
        std::set<int> w_numbers = cards[current].first;
        std::set<int> my_numbers = cards[current].second;

        for (int num: my_numbers) {
            if (w_numbers.find(num) != w_numbers.end()) {
                if (next_card <= ID_MAX) {
                    counts[next_card] += copies;
                    next_card++;
                }
            }
        }
        result += copies;
        current++;
    }

    return result;
}


int main(int argc, char* argv[]) {
    char* filename = argv[1];
    std::vector<std::string> lines;

    read_file(filename, lines);
    printf("%d\n", part_one(lines));
    printf("%d\n", part_two(lines));
}