#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <regex>
#include <set>
#include <map>
#include <tuple>

const int ID_MAX = 205;


int main() {
    std::string line;
    std::ifstream input;
    int id = 1;
    int result = 0;

    input.open("input.txt");
    
    std::map<int, std::pair<std::set<int>, std::set<int> > > cards;
    std::map<int, int> counts;

    if (input.is_open()) {
        while (getline(input, line)) {

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
    }

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

    std::cout << result << std::endl;
    return 0;
}