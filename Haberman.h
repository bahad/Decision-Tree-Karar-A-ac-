
#ifndef C4_5_CSV_H
#define C4_5_CSV_H

#include <vector>
#include <fstream>


typedef std::vector<std::pair<std::vector<double>, double>> Table;
typedef std::pair<std::vector<double>, double> Row;

class Haberman {
private:

    Table data;
    unsigned long feature_count;


public:
    Haberman(Table d);
    Haberman();


    static Table all_data;

    static void read(std::string);

    std::pair<std::vector<double>, double> operator[] (int index);
    double get_value(int row, int feature_index);

    Table get_data();


    int count_greater(int feature_index, double split);

    double count_less(int feature_index, double split);

    Table get_greater(int feature_index, double split);

    Table get_less(int feature_index, double split);

    int length();

    double count_class();

    double get_label();

    double info;
    double count_c1;
    double count_c2;
};



#endif //C4_5_CSV_H
