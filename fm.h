#ifndef _LIBFM_H
#define _LIBFM_H

#include <string>
#include <unordered_map>

namespace fm {

using namespace std;

typedef float fm_float;
typedef double fm_double;
typedef int fm_int;
typedef long long fm_long;

struct fm_node {
    fm_int idx; // feature index
    fm_float value; // value
};

struct fm_weight_unit {
    fm_double w; // weight w_i
    fm_double w_n; // auxiliary parameter for weight w_i
    fm_double w_z;
    vector<fm_double> v; // weight v
    vector<fm_double> v_n; // auxiliary parameter for weight v
    vector<fm_double> v_z;
};

struct fm_model {
    fm_int n; // number of features
    fm_int k; // number of latent factors
    unordered_map<fm_int, fm_weight_unit> weight_map;
    fm_float w0; // weight w_0
    fm_float w0_n; // auxiliary parameter for weight0
    fm_float w0_z;
    ~fm_model();
};

struct fm_parameter {
    fm_float stdev = 0.1;
    fm_float alpha; // for ftrl
    fm_float beta;
    fm_float L1;
    fm_float L2;
    fm_int nr_iters = 15;
    fm_int k = 4; // number of latent factors
    bool auto_stop = false;
};

void fm_read_problem_to_disk(string txt_path, string bin_path);

fm_int fm_save_model(fm_model &model, string path);

fm_model fm_load_model(string path);

fm_model fm_train_on_disk(string Tr_path, string Va_path, fm_parameter param, string model_path);

fm_float fm_predict(fm_node *begin, fm_node *end, fm_model &model);

} // namespace fm

#endif // _LIBFM_H
