#include <cstring>
#include <fstream>
#include <iostream>
#include <string>
#include <iomanip>
#include <memory>
#include <cmath>
#include <stdexcept>
#include <vector>
#include <cstdlib>

#include "fm.h"

using namespace std;
using namespace fm;

struct Option {
    string test_path, model_path, output_path;
};

string predict_help() {
    return string(
"usage: fm-predict test_file model_file output_file\n");
}

Option parse_option(int argc, char **argv) {
    vector<string> args;
    for (int i = 0; i < argc; i++)
        args.push_back(string(argv[i]));

    if (argc == 1)
        throw invalid_argument(predict_help());

    Option option;

    if (argc != 4)
        throw invalid_argument("cannot parse argument");

    option.test_path = string(args[1]);
    option.model_path = string(args[2]);
    option.output_path = string(args[3]);

    return option;
}

void predict(string test_path, string model_path, string output_path) {
    int const kMaxLineSize = 1000000;

    FILE *f_in = fopen(test_path.c_str(), "r");
    ofstream f_out(output_path);
    char line[kMaxLineSize];

    fm_model model = fm_load_model(model_path);

    fm_double loss = 0;
    vector<fm_node> x;
    fm_int i = 0;

    for (; fgets(line, kMaxLineSize, f_in) != nullptr; i++) {
        x.clear();
        char *y_char = strtok(line, " \t");
        fm_float y = (atoi(y_char)>0)? 1.0f : -1.0f;

        while (true) {
            char *idx_char = strtok(nullptr,":");
            char *value_char = strtok(nullptr," \t");

            if (idx_char == nullptr || *idx_char == '\n')
                break;

            fm_node N;
            N.idx = atoi(idx_char);
            N.value = atof(value_char);

            x.push_back(N);
        }

        fm_float y_bar = fm_predict(x.data(), x.data()+x.size(), model);

        loss -= y==1? log(y_bar) : log(1-y_bar);

        f_out << y_bar << "\n";
    }

    loss /= i;

    cout << "logloss = " << fixed << setprecision(5) << loss << endl;

    fclose(f_in);
}

int main(int argc, char **argv) {
    Option option;
    try {
        option = parse_option(argc, argv);
    } catch(invalid_argument const &e) {
        cout << e.what() << endl;
        return 1;
    }

    predict(option.test_path, option.model_path, option.output_path);

    return 0;
}
