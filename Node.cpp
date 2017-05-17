

#include <iostream>
#include <fstream>
#include <thread>
#include <mutex>
#include <future>
#include <string>
#include "Node.h"




std::valarray<std::string> feature_names = {"age", "op-year", "pos-axillary"};
std::valarray<std::string> class_names = {"survived", "died"};

std::valarray<std::valarray<double>> make_lookup() {
    return std::valarray<std::valarray<double>>{
            {50, 60, 70},
            {62, 63, 64},
            {5,  10, 19}
    };
}

std::valarray<std::valarray<double>> Node::split_lookup = make_lookup();

Node::Node(Haberman t, std::vector<int> feature_list) {
    this->table = t;
    this->feature_list = feature_list;
    this->left = nullptr;
    this->right = nullptr;

    // düğüme ait feature_list boş mu ? boş olma koşulu (-1, -1, -1)
    bool empty = true;
    for (int f : feature_list) {
        if (f != -1) {
            empty &= false;
        }
    }

    if (empty) {
        this->label.first = (this->table.count_c1 / this->table.length());
        this->label.second = (this->table.count_c2 / this->table.length());
        this->type = "leaf";
        return;
    }


    std::pair<int, double> bs = this->best_split();
    this->split = bs.second; // hangi değere göre böldüm
    this->split_by = bs.first; // hangi özelliğe göre böldüm
    this->type = "non-leaf"; // düğümün tipi

    if (this->table.get_less(this->split_by, this->split).size() > 0) {
        auto f_list = feature_list;
        f_list[this->split_by] = -1;
        this->left = new Node(this->table.get_less(bs.first, bs.second), f_list);
    }

    if (this->table.get_greater(bs.first, bs.second).size() > 0) {
        auto f_list = feature_list;
        f_list[bs.first] = -1;
        this->right = new Node(this->table.get_greater(bs.first, bs.second), f_list);
    }


}

Table Node::get_table() {
    return this->table.get_data();
}


std::pair<int, std::pair<double, double>> worker(int f, Haberman table) {
    double best_gain = -1;
    std::pair<int, std::pair<double, double>> ret;
    for (double s : Node::split_lookup[f]) {

        Haberman gt = Haberman(table.get_greater(f, s));
        Haberman lt = Haberman(table.get_less(f, s));
        double c_lt = lt.length();
        double c_gt = gt.length();


//            double h_lt = -((lt.count_c1 / c_lt) * log2(lt.count_c1 / c_lt) +
//                            (lt.count_c2 / c_lt) * log2(lt.count_c2 / c_lt));
//            double h_gt = -((gt.count_c1 / c_lt) * log2(gt.count_c1 / c_lt) +
//                            (gt.count_c2 / c_lt) * log2(gt.count_c2 / c_lt));

        /*
        double h_fc = (c_lt / (c_lt+c_gt))*(h_lt) +
                (c_gt / (c_lt+c_gt))*(h_gt);
*/
        double h_fc = (c_lt / (c_lt + c_gt)) * lt.info + (c_gt / (c_lt + c_gt)) * gt.info;
		// table.info
        double h_d = -((table.count_c1 / table.length()) *
                       log2((table.count_c1 / table.length())) +
                       (table.count_c2 / table.length()) *
                       log2(table.count_c2 / table.length()));

        double g = h_d - h_fc;

		// g -> gain, f -> özellik indexi, s -> özelliği hangi değere göre bölcem
        if (g > best_gain) {
            best_gain = g;
            auto a = std::make_pair(f, std::make_pair(g, s));
            ret.swap(a);
        }
    }
    return ret;
}

//http://stackoverflow.com/questions/7686939/c-simple-return-value-from-stdthread
std::pair<int, double> Node::best_split() {
    std::vector<std::future<std::pair<int, std::pair<double, double>>>> futs;
    std::valarray<std::pair<double, double>> best(3);

    for (int f: this->feature_list) {
        if (f == -1) {
            continue;
        }

        futs.push_back(std::async(std::launch::async, worker, f, this->table));


    }


    int cc = 0;
    for (auto &e : futs) {
		// threadden verinin geri dönmesini bekliyoruz
        auto a = e.get();
        best[a.first].swap(a.second);
    }

    int best_index = -1;
    double best_gain = -1;
    double best_split = -1;
    for (int i = 0; i < best.size(); i++) {
        if (best[i].first > best_gain) {
            best_gain = best[i].first;
            best_split = best[i].second;
            best_index = i;
        }
    }
    return std::make_pair(best_index, best_split);
}

//std::pair<int, double> Node::best_split() {
//    // ( feature_index, ( gain, split ) )
//    std::valarray<std::pair<double, double>> best(3);
//
//    std::thread t[3];
//    std::mutex mu;
//
//    int j = 0;
//    for (int f: feature_list) {
//        if (f == -1) {
//            continue;
//        }
//
//
//        double best_gain = -1;
//        for (double s : Node::split_lookup[f]) {
//
//            Haberman gt = Haberman(this->table.get_greater(f, s));
//            Haberman lt = Haberman(this->table.get_less(f, s));
//            double c_lt = lt.length();
//            double c_gt = gt.length();
//
//
////            double h_lt = -((lt.count_c1 / c_lt) * log2(lt.count_c1 / c_lt) +
////                            (lt.count_c2 / c_lt) * log2(lt.count_c2 / c_lt));
////            double h_gt = -((gt.count_c1 / c_lt) * log2(gt.count_c1 / c_lt) +
////                            (gt.count_c2 / c_lt) * log2(gt.count_c2 / c_lt));
//
//            /*
//            double h_fc = (c_lt / (c_lt+c_gt))*(h_lt) +
//                    (c_gt / (c_lt+c_gt))*(h_gt);
//*/
//            double h_fc = (c_lt / (c_lt + c_gt)) * lt.info + (c_gt / (c_lt + c_gt)) * gt.info;
//            double h_d = -((this->table.count_c1 / this->table.length()) *
//                           log2((this->table.count_c1 / this->table.length())) +
//                           (this->table.count_c2 / this->table.length()) *
//                           log2((this->table.count_c2 / this->table.length()))
//            );
//
//            double g = h_d - h_fc;
//
//
//            if (g > best_gain) {
//                best_gain = g;
//                auto a = std::make_pair(g, s);
//                best[f].swap(a);
//
//            }
//        }
//    }
//
//
//    int best_index = -1;
//    double best_gain = -1;
//    double best_split = -1;
//    for (int i = 0; i < best.size(); i++) {
//        auto row = best[i];
//        if (row.first > best_gain) {
//            best_gain = row.first;
//            best_split = row.second;
//            best_index = i;
//        }
//    }
//
//    return std::make_pair(best_index, best_split);
//}


/*
 *
 *  System.out.println(prefix + (isTail ? "└── " : "├── ") + name);
        for (int i = 0; i < children.size() - 1; i++) {
            children.get(i).print(prefix + (isTail ? "    " : "│   "), false);
        }
        if (children.size() > 0) {
            children.get(children.size() - 1)
                    .print(prefix + (isTail ?"    " : "│   "), true);
        }
 *
 *
 */
//http://stackoverflow.com/a/8948691
void Node::print(std::ofstream& cikti) {
    print(cikti, "", true);
}

//http://stackoverflow.com/a/8948691
void Node::print(std::ofstream& cikti, std::string prefix, bool is_tail) {
     cikti << prefix << (is_tail ? "└── " : "├── ");
    if (this->type == "leaf") {
        cikti << class_names[0] << " " << this->label.first << "  " << class_names[1] << " " << this->label.second;

    } else {
        cikti << feature_names[this->split_by] << " . " << this->split;
    }
    cikti << std::endl;

    if (this->right != nullptr) {
        this->right->print(cikti, prefix + (is_tail ? "    " : "│   "), false);
    }

    if (this->left != nullptr) {
        this->left->print(cikti, prefix + (is_tail ? "    " : "│   "), true);
    }
}
