

#ifndef C4_5_NODE_H
#define C4_5_NODE_H


#include <map>
#include <vector>
#include <utility>
#include <valarray>
#include "Haberman.h"


class Node {

private:

    double split;
    std::vector<int> feature_list;
    double split_by;
    std::pair<double, double> label;
    std::string type;
    Node *left;
    Node *right;

public:
    Node(Haberman t, std::vector<int> feature_list);
    static std::valarray<std::valarray<double>> split_lookup;

    Table get_table();
    Haberman table;

    std::pair<int,double> best_split();


    void print(std::ofstream&);
    void print(std::ofstream&, std::string prefix, bool is_tail);

    //oid pre_order(std::string which);
};


#endif //C4_5_NODE_H
