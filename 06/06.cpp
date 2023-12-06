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


int part_one_two(std::vector<std::string>& lines) {
    std::vector<long long int> times;
    std::vector<long long int> distances;
    std::string line;

    for (int i=0; i<2; i++) {
        line = lines[i].substr(11, lines[i].size());
        std::cout << line << std::endl;
        std::stringstream ss(line);
        long long int val;
        while (ss >> val) {
            if (i == 0) {
                times.push_back(val);
            } else {
                distances.push_back(val);
            }
        }
    }

    int result = 1;
    std::vector<int> wins;
    for (int i=0; i<times.size(); i++) {
        long long int time = times[i];
        long long int record = distances[i];
        int w = 0;
        for (long long int h=0; h<=time; h++) {
            long long int time_rem = time - h;
            long long int dist = time_rem * h;
            if (dist > record) w++;
        }
        result *= w;
    }

    return result;
}


int main(int argc, char* argv[]) {
    char* filename = argv[1];
    std::vector<std::string> lines;

    read_file(filename, lines);
    printf("%d\n", part_one_two(lines));
}