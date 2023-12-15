#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <regex>
#include <set>
#include <map>
#include <tuple>
#include <unordered_set>


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


std::vector<std::string> split(const std::string &str, char delim) {
    std::vector<std::string> res;
    std::stringstream ss(str);
    std::string item;

    while (getline (ss, item, delim)) {
        res.push_back (item);
    }
    return res;
}


int hash_s(std::string s) {
    int cv = 0;
    for (char c: s) {
        cv = ((cv + int(c)) * 17) % 256;
    }
    return cv;
}


int part_one(std::vector<std::string>& lines) {
    int result = 0;

    std::vector<std::string> seq = split(lines[0], ','); 
    for (auto s: seq) result += hash_s(s);

    return result;
}


int part_two(std::vector<std::string>& lines) {
    int result = 0;

    std::vector<std::string> seq = split(lines[0], ','); 

    // vector of (label -> position) maps for each box
    std::vector<std::map<std::string, int> > boxes(256);
    
    // label -> focal length
    std::map<std::string, int> focal_lengths;

    for (auto s: seq) {
        if (std::regex_match(s, std::regex("^.*-$"))) {
            std::string label = s.substr(0, s.size()-1);
            int bi = hash_s(label);
            auto it = boxes[bi].find(label);
            if (it != boxes[bi].end()) {
                int idx = boxes[bi][label];
                boxes[bi].erase(label);
                focal_lengths.erase(label);
                // push lenses forward within box
                for (const auto& [l, i]: boxes[bi]) {
                    if (i > idx) {
                        boxes[bi][l] = i-1;
                    }
                }
            }
        } else {
            std::vector<std::string> lens = split(s, '=');
            int bi = hash_s(lens[0]);
            auto it = boxes[bi].find(lens[0]);
            if (it != boxes[bi].end()) {
                focal_lengths[lens[0]] = std::stoi(lens[1]);
            } else {
                boxes[bi][lens[0]] = int(boxes[bi].size());
                focal_lengths[lens[0]] = std::stoi(lens[1]);
            }
        }
    }

    for (int i=0; i<256; i++) {
        auto box = boxes[i];
        if (box.size() > 0) {
            for (const auto& [l, idx]: box) {
                // printf("%s %d %d %d\n", l.c_str(), i+1, idx+1, focal_lengths[l]);
                result += (i + 1) * (idx + 1) * focal_lengths[l];
            }
        }
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