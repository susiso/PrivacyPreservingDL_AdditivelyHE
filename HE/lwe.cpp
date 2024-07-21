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
#include <time.h>

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
const long long n_gd = 20000;
const mp::int128_t p = pow2(48);
const mp::int128_t q = pow2(77);

void vec_zero(int a, int *x){
    for (int i = 0; i < a; i++){
        x[i] = 0;
    }
}

void vec_zero(int a, mp::int128_t *x){
    for (int i = 0; i < a; i++){
        x[i] = 0;
    }
}

void mat_zero(int a, int b, int **A){
    for (int i = 0; i < a; i++){
        for (int j = 0; j < b; j++){
            A[i][j] = 0;
        }
    }
}

void mat_zero(int a, int b, mp::int128_t **A){
    for (int i = 0; i < a; i++){
        for (int j = 0; j < b; j++){
            A[i][j] = 0;
        }
    }
}

void vec_show(int a, int *x){
    for (int i = 0; i < a; i++){
        std::cout << x[i] << " ";
    }
    std::cout << std::endl;
}

void vec_show(int a, mp::int128_t *x){
    for (int i = 0; i < a; i++){
        std::cout << x[i] << " ";
    }
    std::cout << std::endl;
}

void mat_show(int a, int b, int **A){
    for (int i = 0; i < a; i++){
        vec_show(b, A[i]);
    }
}

void mat_show(int a, int b, mp::int128_t **A){
    for (int i = 0; i < a; i++){
        vec_show(b, A[i]);
    }
}

void KeyGen(mp::int128_t **P, int **R, mp::int128_t **A, int **S){
    mp::int128_t temp = 0;
    for (int i = 0; i < n_lwe; i++){
        std::cout << i << std::endl;
        for (int j = 0; j < n_gd; j++){
            for (int k = 0; k < n_lwe; k++){
                temp += A[i][k] * S[k][j];
            }
            P[i][j] = p * R[i][j] - temp;
            temp = 0;
        }
    }
}

void Enc(int *e_1, int *e_2, int*e_3, mp::int128_t **A, mp::int128_t **P, mp::int128_t *m, mp::int128_t *c_1, mp::int128_t *c_2){
    mp::int128_t temp = 0;
    for (int i = 0; i < n_lwe; i++){
        for (int j = 0; j < n_lwe; j++){
            temp += e_1[j] * A[j][i];
        }
        c_1[i] = temp + p * e_2[i];
        temp = 0;
    }

    for (int i = 0; i < n_gd; i++){
        for (int j = 0; j < n_lwe; j++){
            temp += e_1[j] * P[j][i];
        }
        c_2[i] = temp + p * e_3[i] + m[i];
        temp = 0;
    }
}

void Dec(mp::int128_t *c_1, mp::int128_t *c_2, int **S, mp::int128_t *m_bar){
    mp::int128_t temp = 0;
    for (int i = 0; i < n_gd; i++){
        for (int j = 0; j < n_lwe; j++){
            temp += c_1[j] * S[j][i];
        }
        m_bar[i] = temp + c_2[i];
        temp = 0;
        m_bar[i] %= p;
        if (m_bar[i] < 0){
            m_bar[i] += p;
        }
    }
}

void Add(mp::int128_t *c_1_a, mp::int128_t *c_2_a, mp::int128_t *c_1_b, mp::int128_t *c_2_b, mp::int128_t *c_1, mp::int128_t *c_2){
    for (int i = 0; i < n_lwe; i++){
        c_1[i] = c_1_a[i] + c_1_b[i];
    }
    for (int i = 0; i < n_gd; i++){
        c_2[i] = c_2_a[i] + c_2_b[i];
    }
}

int main (){    
    // メモリ確保
    int *R[n_lwe];
    for (int j = 0; j < n_lwe; j++){
        R[j] = (int*)malloc(n_gd*sizeof(int));
    }
    int *S[n_lwe];
    for (int j = 0; j < n_lwe; j++){
        S[j] = (int*)malloc(n_gd*sizeof(int));
    }
    mp::int128_t *A[n_lwe];
    for (int j = 0; j < n_lwe; j++){
        A[j] = (mp::int128_t*)malloc(n_lwe*sizeof(mp::int128_t));
    }

    int *e_1_a;
    e_1_a = (int*)malloc(n_lwe*sizeof(int));
    int *e_2_a;
    e_2_a = (int*)malloc(n_lwe*sizeof(int));
    int *e_3_a;
    e_3_a = (int*)malloc(n_gd*sizeof(int));

    int *e_1_b;
    e_1_b = (int*)malloc(n_lwe*sizeof(int));
    int *e_2_b;
    e_2_b = (int*)malloc(n_lwe*sizeof(int));
    int *e_3_b;
    e_3_b = (int*)malloc(n_gd*sizeof(int));

    mp::int128_t *m;
    m = (mp::int128_t*)malloc(n_gd*sizeof(mp::int128_t));

    mp::int128_t *c_1_a;
    c_1_a = (mp::int128_t*)malloc(n_lwe*sizeof(mp::int128_t));
    mp::int128_t *c_2_a;
    c_2_a = (mp::int128_t*)malloc(n_gd*sizeof(mp::int128_t));
    
    mp::int128_t *c_1_b;
    c_1_b = (mp::int128_t*)malloc(n_lwe*sizeof(mp::int128_t));
    mp::int128_t *c_2_b;
    c_2_b = (mp::int128_t*)malloc(n_gd*sizeof(mp::int128_t));

    mp::int128_t *c_1;
    c_1 = (mp::int128_t*)malloc(n_lwe*sizeof(mp::int128_t));
    mp::int128_t *c_2;
    c_2 = (mp::int128_t*)malloc(n_gd*sizeof(mp::int128_t));

    // ファイルロード
    std::ifstream ifs_R("discrete_gaussian_distribution/sample/R.txt");
    std::ifstream ifs_S("discrete_gaussian_distribution/sample/S.txt");
    std::ifstream ifs_A("discrete_gaussian_distribution/sample/A.txt");
    std::ifstream ifs_e_1_a("discrete_gaussian_distribution/sample/e_1_a.txt");
    std::ifstream ifs_e_2_a("discrete_gaussian_distribution/sample/e_2_a.txt");
    std::ifstream ifs_e_3_a("discrete_gaussian_distribution/sample/e_3_a.txt");
    std::ifstream ifs_e_1_b("discrete_gaussian_distribution/sample/e_1_b.txt");
    std::ifstream ifs_e_2_b("discrete_gaussian_distribution/sample/e_2_b.txt");
    std::ifstream ifs_e_3_b("discrete_gaussian_distribution/sample/e_3_b.txt");
    std::ifstream ifs_m("discrete_gaussian_distribution/sample/m.txt");

    // 配列に保存
    int i = 0;
    int j = 0;
    std::string str;

    std::cout << "loading R" << std::endl;
    while(getline(ifs_R, str)){
        std::string token;
        std::istringstream stream(str);
        while(getline(stream, token, ' ')){
            int temp = std::stoi(token);
            R[i][j] = temp;
            j++;
        }
        j = 0;
        i++;
    }

    i = 0;
    j = 0;
    std::cout << "loading S" << std::endl;
    while(getline(ifs_S, str)){
        std::string token;
        std::istringstream stream(str);
        while(getline(stream, token, ' ')){
            int temp = std::stoi(token);
            S[i][j] = temp;
            j++;
        }
        j = 0;
        i++;
    }

    j = 0;
    i = 0;
    std::cout << "loading A" << std::endl;
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
    std::cout << "loading e_a" << std::endl;
    while(getline(ifs_e_1_a, str)){
        std::string token;
        std::istringstream stream(str);
        while(getline(stream, token, ' ')){
            int temp = std::stoi(token);
            e_1_a[j] = temp;
            j++;
        }
        j = 0;
        i++;
    }

    j = 0;
    i = 0;
    while(getline(ifs_e_2_a, str)){
        std::string token;
        std::istringstream stream(str);
        while(getline(stream, token, ' ')){
            int temp = std::stoi(token);
            e_2_a[j] = temp;
            j++;
        }
        j = 0;
        i++;
    }

    j = 0;
    i = 0;
    while(getline(ifs_e_3_a, str)){
        std::string token;
        std::istringstream stream(str);
        while(getline(stream, token, ' ')){
            int temp = std::stoi(token);
            e_3_a[j] = temp;
            j++;
        }
        j = 0;
        i++;
    }

    j = 0;
    i = 0;
    std::cout << "loading e_b" << std::endl;
    while(getline(ifs_e_1_b, str)){
        std::string token;
        std::istringstream stream(str);
        while(getline(stream, token, ' ')){
            int temp = std::stoi(token);
            e_1_b[j] = temp;
            j++;
        }
        j = 0;
        i++;
    }

    j = 0;
    i = 0;
    while(getline(ifs_e_2_b, str)){
        std::string token;
        std::istringstream stream(str);
        while(getline(stream, token, ' ')){
            int temp = std::stoi(token);
            e_2_b[j] = temp;
            j++;
        }
        j = 0;
        i++;
    }

    j = 0;
    i = 0;
    while(getline(ifs_e_3_b, str)){
        std::string token;
        std::istringstream stream(str);
        while(getline(stream, token, ' ')){
            int temp = std::stoi(token);
            e_3_b[j] = temp;
            j++;
        }
        j = 0;
        i++;
    }

    j = 0;
    i = 0;
    std::cout << "loading m" << std::endl;
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

    double time;

    // 鍵の生成
    std::cout << "KeyGen" << std::endl;
    mp::int128_t *P[n_lwe];
    for (int j = 0; j < n_lwe; j++){
        P[j] = (mp::int128_t*)malloc(n_gd*sizeof(mp::int128_t));
    }

    KeyGen(P, R, A, S);

    // Encryption
    std::cout << "Enc" << std::endl;
    clock_t start_enc = clock();
    Enc(e_1_a, e_2_a, e_3_a, A, P, m, c_1_a, c_2_a);
    clock_t end_enc = clock();

    time = static_cast<double>(end_enc - start_enc) / CLOCKS_PER_SEC * 1000.0;
    printf("Enc time %lf[ms]\n", time);
    
    // Decryption
    std::cout << "Dec" << std::endl;
    mp::int128_t *m_bar;
    m_bar = (mp::int128_t*)malloc(n_gd*sizeof(mp::int128_t));

    clock_t start_dec = clock();
    Dec(c_1_a, c_2_a, S, m_bar);
    clock_t end_dec = clock();
    time = static_cast<double>(end_dec - start_dec) / CLOCKS_PER_SEC * 1000.0;
    printf("Dec time %lf[ms]\n", time);

    // Add
    std::cout << "Add" << std::endl;
    Enc(e_1_b, e_2_b, e_3_b, A, P, m, c_1_b, c_2_b);

    clock_t start_add = clock();
    Add(c_1_a, c_2_a, c_1_b, c_2_b, c_1, c_2);
    clock_t end_add = clock();
    time = static_cast<double>(end_add - start_add) / CLOCKS_PER_SEC * 1000.0;
    printf("Add time %lf[ms]\n", time);

    // Enc Dec 検証
    std::cout << "Enc Dec Output" << std::endl;
    std::ofstream ofs_m("result/m.txt");
    std::ofstream ofs_m_bar("result/m_bar.txt");

    for (i = 0; i < n_gd; i++){
        ofs_m << m[i] << " ";
    }
    ofs_m << std::endl;
    ofs_m.close();

    for (i = 0; i < n_gd; i++){
        ofs_m_bar << m_bar[i] << " ";
    }
    ofs_m_bar << std::endl;
    ofs_m_bar.close();

    // Add 検証
    std::cout << "Add Output" << std::endl;
    for (i = 0; i < n_gd; i++){
        m[i] = m[i] + m[i];
    }

    Dec(c_1, c_2, S, m_bar);
    
    std::ofstream ofs_m_add("result/m_add.txt");
    std::ofstream ofs_m_bar_add("result/m_bar_add.txt");

    for (i = 0; i < n_gd; i++){
        ofs_m_add << m[i] << " ";
    }
    ofs_m_add << std::endl;
    ofs_m_add.close();

    for (i = 0; i < n_gd; i++){
        ofs_m_bar_add << m_bar[i] << " ";
    }
    ofs_m_bar_add << std::endl;
    ofs_m_bar_add.close();

    return 0;
}