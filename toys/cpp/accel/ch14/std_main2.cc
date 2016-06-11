#include <iostream>
#include <iomanip>
#include <stdexcept>
#include <string>
#include <vector>
#include <algorithm>
#include "Ref_handle.h"
#include "Student_info.h"

using std::vector;
using std::string;
using std::max;
using std::cin;
using std::cout;
using std::endl;
using std::streamsize;
using std::setprecision;
using std::domain_error;

int main()
{
    vector< Ref_handle<Core> > students;
    Ref_handle<Core> record;
    char ch;
    string::size_type maxlen = 0;

    while (cin >> ch) {
        if (ch == 'U')
            record = new Core;
        else
            record = new Grad;

        record->read(cin);
        maxlen = max(maxlen, record->name().size()); // Ref_handle<T>::->
        students.push_back(record);
    }

    sort(students.begin(), students.end(), compare_Core_Ref_handles);

    for (vector< Ref_handle<Core> >::size_type i = 0;
            i != students.size(); ++i) {
        cout << students[i]->name()
             << string(maxlen + 1 - students[i]->name().size(), ' ');
        try {
            double final_grade = students[i]->grade();
            streamsize prec = cout.precision();
            cout << setprecision(3) << final_grade
                 << setprecision(prec) << endl;
        } catch (domain_error e) {
            cout << e.what() << endl;
        }
    }
    return 0;
}
