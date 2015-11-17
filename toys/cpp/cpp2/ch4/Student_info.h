#ifndef _Student_info_
#define _Student_info_

#include <string>
#include <vector>

struct Student_info {
    std::string name;
    double midterm, final;
    std::vector<double> homework;
};

bool compare(const Student_info& x, const Student_info& y);

std::istream& read_hw(std::istream& in, std::vector<double>& hw);

std::istream& read(std::istream& is, Student_info& s);
#endif
