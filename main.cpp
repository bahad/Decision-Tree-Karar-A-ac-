#include <iostream>
#include "Haberman.h"
#include "Node.h"
#include "conio.h"



int main() {

    Haberman::read("D:\\haberman.txt");

    auto c = Haberman(Haberman::all_data);

    Node n = Node(c, {0,1,2});

	std::ofstream cikti("D:\\output.txt");
   n.print(cikti);
   cikti.close();

	_getch();
    return 0;
}