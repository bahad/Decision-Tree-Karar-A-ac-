
#include <sstream>
#include <iostream>
#include "Haberman.h"
#include "prettyprint.hpp"


Table empty_table() {
    return std::vector<std::pair<std::vector<double>, double>>(0, std::make_pair(std::vector<double>(0, 0), 0.0));
}

Table Haberman::all_data = empty_table();

void Haberman::read(std::string csv_file) {
    std::ifstream input_stream(csv_file);

    for (std::string line; std::getline(input_stream, line);) {
        std::stringstream line_stream(line);
        std::string ss;

        std::vector<double> tmp;
        while (getline(line_stream, ss, ',')) {
            tmp.push_back(std::stoi(ss));
        }
        double c = tmp.back();
        tmp.pop_back();
        Haberman::all_data.push_back(std::make_pair(tmp, c));
    }
}

std::pair<std::vector<double>, double> Haberman::operator[](int index) {
    return this->data[index];
}

double Haberman::get_value(int row, int feature_index) {
    return this->data[row].first[feature_index];
}

Haberman::Haberman(Table d) {
    this->data = d;
    this->feature_count = 4;


    this->count_c1 = this->count_class();
    this->count_c2 = this->data.size() - count_c1;

    this->info = -(((count_c1 / this->data.size()) * log2(count_c1 / this->data.size()) +
                    ((count_c2 / this->data.size()) * log2(count_c2 / this->data.size()))));
}

// Bu kurucu fonksiyon hiç kullanılmadı, ama Haberman() tanımlanması gerekiyor
Haberman::Haberman() {
    this->data = Haberman::all_data;
    this->feature_count = data[0].first.size();

    double c1 = ((double) this->count_class() / (double) this->length());
    double c2 = 1 - c1;
    this->info = -(c1 * log2(c1) + c2 * log2(c2));


}

Table Haberman::get_data() {
    return this->data;
}

int Haberman::count_greater(int feature_index, double split) {

    if (feature_index > this->feature_count) {
        return -1;
    }

    int count = 0;
    for (Row a : this->data) {
        if (a.first[feature_index] >= split) {
            count++;
        }
    }

    return count;
}

double Haberman::count_less(int feature_index, double split) {

    if (feature_index > this->feature_count) {
        return -1;
    }

    int count = 0;
    for (Row a : this->data) {
        if (a.first[feature_index] < split) {
            count++;
        }
    }

    return count;
}

Table Haberman::get_greater(int feature_index, double split) {
    Table t;
    if (feature_index > this->feature_count) {
        return t;
    }

    int count = 0;
    for (Row a : this->data) {
        if (a.first[feature_index] >= split) {
            t.push_back(a);
        }
    }
    return t;
}

Table Haberman::get_less(int feature_index, double split) {
    Table t;
    if (feature_index > this->feature_count) {
        return t;
    }

    int count = 0;
    for (Row a : this->data) {
        if (a.first[feature_index] < split) {
            t.push_back(a);
        }
    }

    return t;
}


int Haberman::length() {
    return (int) this->data.size();
}

double Haberman::count_class() {
    int count = 0;
    for (Row a: this->data) {
        if (a.second == 1) {
            count++;
        }
    }

    return count;
}

double Haberman::get_label() {
    return this->data[0].second;

}
