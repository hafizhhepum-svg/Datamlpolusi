#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

void separate() {

    std::ifstream infile("./Datamlpolusi/data2.txt");   // your original file
    std::ofstream out23("out_23.txt");
    std::ofstream out45("out_45.txt");
    std::ofstream out67("out_67.txt");
    std::ofstream out89("out_89.txt");

    if (!infile.is_open()) {
        std::cout << "Error opening input file!" << std::endl;
        return;
    }

    std::string line;

    while (std::getline(infile, line)) {

        std::stringstream ss(line);

        std::string date;
        double c2, c3, c4, c5, c6, c7, c8, c9, c10, c11;

        ss >> date >> c2 >> c3 >> c4 >> c5 >> c6 >> c7 >> c8 >> c9 >> c10 >> c11;

        // Write selected column pairs to different files
        out23 << c2 << " " << c3 << std::endl;
        out45 << c4 << " " << c5 << std::endl;
        out67 << c6 << " " << c7 << std::endl;
        out89 << c8 << " " << c9 << std::endl;
    }

    infile.close();
    out23.close();
    out45.close();
    out67.close();
    out89.close();

    std::cout << "Separation complete!" << std::endl;
}
