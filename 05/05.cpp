#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <regex>
#include <set>
#include <map>
#include <tuple>
#include <algorithm>


typedef struct map_line {
    long int dest_start;
    long int source_start;
    long int length;
};

typedef struct seed_map {
    std::string name;
    std::vector<map_line> map_lines;
};


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


long int get_map_value(long int key, seed_map& map) {
    for (auto mapline: map.map_lines) {
        if (key >= mapline.source_start && key <= mapline.source_start + mapline.length) {
            long int value = mapline.dest_start + (key - mapline.source_start);
            return value;
        }
    }
    return key;
}


void split_maps(seed_map& to_split, seed_map& base) {
    for (auto& splitter: base.map_lines) {
        for (auto& target: to_split.map_lines) {
            if (splitter.source_start > target.dest_start && splitter.source_start < target.dest_start + target.length) {
                map_line new_mapline;
                new_mapline.dest_start = splitter.source_start;
                new_mapline.source_start = target.source_start + (splitter.source_start - target.dest_start);
                new_mapline.length = target.length - (splitter.source_start - target.dest_start);
                to_split.map_lines.push_back(new_mapline);
                target.length = splitter.source_start - target.dest_start;
            }
        }
    }
}


bool compare_maplines(map_line a, map_line b) {
    return a.source_start < b.source_start;
}


long int part_one_two(std::vector<std::string>& lines) {
    std::string line = lines[0];
    line = line.substr(7, line.size());

    std::vector<long int> seeds;

    long int i;
    std::stringstream ss(line);
    while (ss >> i) {
        seeds.push_back(i);
    }

    seed_map seed_to_soil;
    seed_to_soil.name = "seed-to-soil map:";
    seed_map soil_to_fertilizer;
    soil_to_fertilizer.name = "soil-to-fertilizer map:";
    seed_map fertilizer_to_water;
    fertilizer_to_water.name = "fertilizer-to-water map:";
    seed_map water_to_light;
    water_to_light.name = "water-to-light map:";
    seed_map light_to_temperature;
    light_to_temperature.name = "light-to-temperature map:";
    seed_map temperature_to_humidity;
    temperature_to_humidity.name = "temperature-to-humidity map:";
    seed_map humidity_to_location;
    humidity_to_location.name = "humidity-to-location map:";

    std::vector<seed_map> maps = {
        seed_to_soil,
        soil_to_fertilizer,
        fertilizer_to_water,
        water_to_light,
        light_to_temperature,
        temperature_to_humidity,
        humidity_to_location
    };

    for (auto& map: maps) {
        std::string line;
        auto it = std::find(lines.begin(), lines.end(), map.name);
        if (it != lines.end()) {
            int idx = it - lines.begin();
            idx++;
            line = lines[idx];
            do {
                map_line mapline;
                std::stringstream ss(line);

                ss >> mapline.dest_start;
                ss >> mapline.source_start;
                ss >> mapline.length;

                map.map_lines.push_back(mapline);

                idx++;
                line = lines[idx];
            } while (line != "");
        }
        long int lowest_source = map.map_lines[0].source_start;
        if (lowest_source > 0) {
            map_line new_mapline;
            new_mapline.dest_start = 0;
            new_mapline.source_start = 0;
            new_mapline.length = lowest_source;
            map.map_lines.push_back(new_mapline);
        }
    }

    for (int i=6; i>0; i--) {
        split_maps(maps[i-1], maps[i]);
    }

    std::vector<long int> min_range_values;
    for (auto m: maps[0].map_lines) {
        min_range_values.push_back(m.source_start);
    }

    std::sort(min_range_values.begin(), min_range_values.end());
    min_range_values.erase( unique( min_range_values.begin(), min_range_values.end() ), min_range_values.end() );

    long int lowest = 3464208731; 

    int current_range;
    long int seed, end;
    for (int i=0; i<seeds.size()-1; i+=2) {
        current_range = 0;
        seed = seeds[i];
        end = seed+seeds[i+1];

        while (seed < end) {
            if (seed == 0) return lowest;

            printf("%ld\n", seed);

            while (current_range < min_range_values.size() && seed > min_range_values[current_range]) {
                current_range++;
            }

            long int soil = get_map_value(seed, maps[0]);
            long int fertilizer = get_map_value(soil, maps[1]);
            long int water = get_map_value(fertilizer, maps[2]);
            long int light = get_map_value(water, maps[3]);
            long int temp = get_map_value(light, maps[4]);
            long int humidity = get_map_value(temp, maps[5]);
            long int location = get_map_value(humidity, maps[6]);

            if (location < lowest) {
                lowest = location;
            }

            seed = min_range_values[current_range+1];
            current_range++;
        }
    }

    return lowest;
}


int main(int argc, char* argv[]) {
    char* filename = argv[1];
    std::vector<std::string> lines;

    read_file(filename, lines);
    printf("%ld\n", part_one_two(lines));
}