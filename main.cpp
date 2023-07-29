#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>


auto Input(std::istream& in){
    std::vector<double> X;
    std::vector<double> Y;
    int N;
    double XX;
    double eps;
    in >> N;
    Y.resize(N);
    X.resize(N);
    for (auto& x : X){
        in >> x;
    }
    for (auto& y : Y){
        in >> y;
    }
    in >> XX >> eps;
    return std::make_tuple(N, X, Y, XX, eps);
}

struct Result{
    int code;
    double answer;
};

#define LOG(X) std::cout << #X << ": " << X << "\n"

using Type = decltype(Input(std::declval<std::istream&>()));

Result Interpolation(const Type& value){
    auto& [N, X, Y, XX, eps] = value;
    if (!std::is_sorted(X.begin(), X.end(), [](double left, double right){
        return left < right;
    })){
        return Result{
            .code = 3,
            .answer = 0
        };
    }
    if (XX < X.front() || XX > X.back()){
        return Result{
            .code = 4,
            .answer = 0
        };
    }
    size_t k = 1;
    double prev_eps = 999;
    double cur_eps = 998;
    std::vector<double> b;
    b.reserve(N);
    double Pn = Y[0];
    while (k < N && cur_eps < prev_eps){
        auto& bk = b.emplace_back(0);
        for (size_t i = 0; i <= k; i++){
            auto f = Y[i];
            auto znam = 1.0;
            for (size_t j = 0; j <= k; j++){
                if (j!=i){
                    znam *= (X[i] - X[j]);
                }
            }
            bk += f/znam;
        }
        auto prev_Pn = Pn;
        double Pi = bk;
        for (size_t i = 0; i < k; i++){
            Pi *= XX - X[i];
        }
        Pn += Pi;
        prev_eps = cur_eps;
        cur_eps = abs(Pn - prev_Pn);
        k++;
    }
    if (k!=N - 1){
        return Result{
          .code = 2,
          .answer = Pn
        };
    }
    else if (cur_eps > eps){
        return Result{
            .code = 1,
            .answer = Pn
        };
    }
    return Result{
        .code = 0,
        .answer = Pn
    };

}


int main() {
    std::ifstream file("file.txt");
    if (!file.is_open()){
        LOG(file.is_open());
        return 1;
    }
    auto value = Input(file);
    auto result = Interpolation(value);
    LOG(result.answer);
    LOG(result.code);
}
