#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <boost/multiprecision/cpp_int.hpp>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include <stdlib.h>
#include <stdio.h>

namespace mp = boost::multiprecision;

mp::int128_t pow2(int x){
    mp::int128_t result = 1;
    for (unsigned int i = 1; i <= x; ++i){
        result *= 2;
    }

    return result;
}

const long long n_lwe = 3000;
const long long s = 8;
const long long n_gd_max = 402250;
const mp::int128_t p = pow2(48);
const mp::int128_t q = pow2(77);
// std::vector<std::vector<mp::int128_t>> R(n_lwe, std::vector<mp::int128_t>(n_gd_max, 0));
// std::vector<std::vector<mp::int128_t>> S(n_lwe, std::vector<mp::int128_t>(n_gd_max, 0));
// std::vector<std::vector<mp::int128_t>> A(n_lwe, std::vector<mp::int128_t>(n_lwe, 0));
// std::vector<std::vector<mp::int128_t>> e_1(1, std::vector<mp::int128_t>(n_lwe, 0));
// std::vector<std::vector<mp::int128_t>> e_2(1, std::vector<mp::int128_t>(n_lwe, 0));
// std::vector<std::vector<mp::int128_t>> e_3(1, std::vector<mp::int128_t>(n_gd_max, 0));
// std::vector<std::vector<mp::int128_t>> c_1(1, std::vector<mp::int128_t>(n_lwe, 0));
// std::vector<std::vector<mp::int128_t>> c_2(1, std::vector<mp::int128_t>(n_gd_max, 0));

void vec_zero(int a, mp::int128_t *x){
    for (int i = 0; i < a; i++){
        x[i] = 0;
    }
}

void mat_zero(int a, int b, mp::int128_t **A){
    for (int i = 0; i < a; i++){
        for (int j = 0; j < b; j++){
            A[i][j] = 0;
        }
        std::cout << i << std::endl;
    }
}

void vec_show(int a, mp::int128_t *x){
    for (int i = 0; i < a; i++){
        std::cout << x[i] << std::endl;
    }
}

// void mat_product(mp::int128_t A[][], mp::int128_t B[][], mp::int128_t C[][]){
//     mat_zero(&C[][])
//     for (int i = 0; i < A.size(); i++){
//         for (int j = 0; j < B[0].size(); j++){
//             for (int k = 0; k < A[0].size(); k++){
//                 C[i][j] += A[i][k] + B[k][j];
//             }
//         }
//     }
// }

int main (){    
    // メモリ確保
    mp::int128_t *R[n_lwe];
    for (int j = 0; j < n_lwe; j++){
        R[j] = (mp::int128_t*)malloc(n_gd_max*sizeof(mp::int128_t));
    }
    mp::int128_t *S[n_lwe];
    for (int j = 0; j < n_lwe; j++){
        S[j] = (mp::int128_t*)malloc(n_gd_max*sizeof(mp::int128_t));
    }
    mp::int128_t *A[n_lwe];
    for (int j = 0; j < n_lwe; j++){
        A[j] = (mp::int128_t*)malloc(n_lwe*sizeof(mp::int128_t));
    }

    mp::int128_t *e_1;
    e_1 = (mp::int128_t*)malloc(n_lwe*sizeof(mp::int128_t));
    mp::int128_t *e_2;
    e_2 = (mp::int128_t*)malloc(n_lwe*sizeof(mp::int128_t));
    mp::int128_t *e_3;
    e_3 = (mp::int128_t*)malloc(n_gd_max*sizeof(mp::int128_t));

    mp::int128_t *m;
    m = (mp::int128_t*)malloc(n_gd_max*sizeof(mp::int128_t));

    mp::int128_t *c_1;
    c_1 = (mp::int128_t*)malloc(n_lwe*sizeof(mp::int128_t));
    mp::int128_t *c_2;
    c_2 = (mp::int128_t*)malloc(n_gd_max*sizeof(mp::int128_t));
    
    mp::int128_t *c_1_a;
    c_1_a = (mp::int128_t*)malloc(n_lwe*sizeof(mp::int128_t));
    mp::int128_t *c_2_a;
    c_2_a = (mp::int128_t*)malloc(n_gd_max*sizeof(mp::int128_t));

    // ファイルロード
    // std::ifstream ifs_R("discrete_gaussian_distribution/sample/R.txt");
    // std::ifstream ifs_S("discrete_gaussian_distribution/sample/S.txt");
    std::ifstream ifs_A("discrete_gaussian_distribution/sample/A.txt");
    std::ifstream ifs_e_1("discrete_gaussian_distribution/sample/e_1.txt");
    std::ifstream ifs_e_2("discrete_gaussian_distribution/sample/e_2.txt");
    std::ifstream ifs_e_3("discrete_gaussian_distribution/sample/e_3.txt");
    std::ifstream ifs_m("discrete_gaussian_distribution/sample/m.txt");

    // 配列に保存
    int i = 0;
    int j = 0;
    std::string str;

    // while(getline(ifs_R, str)){
    //     std::string token;
    //     std::istringstream stream(str);
    //     while(getline(stream, token, ' ')){
    //         mp::int128_t temp(token);
    //         R[i][j] = temp;
    //         j++;
    //     }
    //     j = 0;
    //     i++;
    // }

    // i = 0;
    // j = 0;
    // while(getline(ifs_S, str)){
    //     std::string token;
    //     std::istringstream stream(str);
    //     while(getline(stream, token, ' ')){
    //         mp::int128_t temp(token);
    //         S[i][j] = temp;
    //         j++;
    //     }
    //     j = 0;
    //     i++;
    // }

    j = 0;
    i = 0;
    while(getline(ifs_A, str)){
        std::string token;
        std::istringstream stream(str);
        while(getline(stream, token, ' ')){
            mp::int128_t temp(token);
            A[i][j] = temp;
            j++;
        }
        j = 0;
        i++;
    }

    j = 0;
    i = 0;
    while(getline(ifs_e_1, str)){
        std::string token;
        std::istringstream stream(str);
        while(getline(stream, token, ' ')){
            mp::int128_t temp(token);
            e_1[j] = temp;
            j++;
        }
        j = 0;
        i++;
    }

    j = 0;
    i = 0;
    while(getline(ifs_e_2, str)){
        std::string token;
        std::istringstream stream(str);
        while(getline(stream, token, ' ')){
            mp::int128_t temp(token);
            e_2[j] = temp;
            j++;
        }
        j = 0;
        i++;
    }

    j = 0;
    i = 0;
    while(getline(ifs_e_3, str)){
        std::string token;
        std::istringstream stream(str);
        while(getline(stream, token, ' ')){
            mp::int128_t temp(token);
            e_3[j] = temp;
            j++;
        }
        j = 0;
        i++;
    }

    j = 0;
    i = 0;
    while(getline(ifs_m, str)){
        std::string token;
        std::istringstream stream(str);
        while(getline(stream, token, ' ')){
            mp::int128_t temp(token);
            m[j] = temp;
            j++;
        }
        j = 0;
        i++;
    }

    mat_zero(n_lwe, n_gd_max, R);
    vec_zero(n_lwe, c_1);
    vec_show(n_lwe, c_1);

    return 0;
}