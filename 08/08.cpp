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


typedef std::pair<std::string, std::string> choices;


int part_one(std::vector<std::string>& lines) {
    int result = 0;
    std::string path = lines[0];
    std::map<std::string, choices> nodes;

    for (int i=2; i<lines.size(); i++) {
        std::string line = lines[i];
        std::string from = line.substr(0, 3);
        std::string left = line.substr(7, 3);
        std::string right = line.substr(12, 3);
        nodes[from] = std::make_pair(left, right);
    }

    std::string current = "AAA";
    int c = 0;
    while (current != "ZZZ") {
        if (path[c] == 'L') { 
            current = nodes[current].first;
        } else {
            current = nodes[current].second;
        }

        if (c == path.size()-1) c = 0; else c++;
        result++;
    } 

    return result;
}


bool is_end(int it, std::vector<std::string>& current_nodes, std::vector<int>& zs) {
    for (int i=0; i<current_nodes.size(); i++) {
        std::string node = current_nodes[i];
        if (std::regex_match(node, std::regex("^..Z$")) && zs[i] == -1) {
            zs[i] = it;
        }
    }

    for (int i: zs) {
        if (i == -1) return false;
    }
    return true;
}


int update_nodes(
    std::string path, int step,
    std::vector<std::string>& current_nodes,
    std::map<std::string, choices>& node_map)
{
    for (auto& node: current_nodes) {
        std::string next_node;
        if (path[step] == 'L') {
            next_node = node_map[node].first;
        } else {
            next_node = node_map[node].second;
        }
        node = next_node;
    }

    if (step == path.size()-1) step = 0; else step++;

    return step;
}


int part_two(std::vector<std::string>& lines) {
    std::string path = lines[0];
    std::map<std::string, std::pair<std::string, std::string> > node_map;

    for (int i=2; i<lines.size(); i++) {
        std::string line = lines[i];
        std::string from = line.substr(0, 3);
        std::string left = line.substr(7, 3);
        std::string right = line.substr(12, 3);
        node_map[from] = std::make_pair(left, right);
    }

    std::vector<std::string> current_nodes;
    for (const auto& [node, paths]: node_map) {
        if (std::regex_match(node, std::regex("^..A$"))) {
            current_nodes.push_back(node);
        }
    }
    
    std::vector<int> zs;
    for (int i=0; i<current_nodes.size(); i++) {
        zs.push_back(-1);
    }

    int it, step;
    while (not is_end(it, current_nodes, zs)) {
        step = update_nodes(path, step, current_nodes, node_map);
        it++;
    }

    for (auto i: zs) {
        printf("%d,", i);
    }
    printf("\n");

    // then i used an online calculator to find the lcm :D
    return 0;
}


int main(int argc, char* argv[]) {
    char* filename = argv[1];
    std::vector<std::string> lines;

    read_file(filename, lines);
    printf("%d\n", part_one(lines));
    printf("%ld\n", part_two(lines));
}