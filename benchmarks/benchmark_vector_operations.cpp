#include <chrono>
#include <iostream>
#include <random>
#include <vector>

#include "eurora/math/math.hpp"
#include "eurora/utils/enum_utils.hpp"

namespace chrono = std::chrono;
using namespace eurora::math;

fvec generate_random_vector(size_t size) {
    fvec vec(1, size);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dist(0.0f, 1.0f);

    for (size_t i = 0; i < size; ++i) {
        vec[i] = dist(gen);
    }
    return vec;
}

template <BackendType backend>
void test_performance(const std::string& operation, size_t vector_size, int iterations) {
    fvec vec1 = generate_random_vector(vector_size);
    fvec vec2 = generate_random_vector(vector_size);

    auto start = chrono::high_resolution_clock::now();

    for (int i = 0; i < iterations; ++i) {
        if (operation == "Add") {
            Add<backend>(vec1, vec2);
        } else if (operation == "Subtract") {
            Subtract<backend>(vec1, vec2);
        }
    }

    auto end      = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(end - start).count();

    std::cout << "Backend: " << eurora::utils::ToString(backend) << ", Operation: " << operation << ", Vector Size: " << vector_size
              << ", Iterations: " << iterations << ", Time (microseconds): " << duration << std::endl;
}

int main() {
    const std::vector<size_t> vector_sizes = {1000, 10000, 100000};
    const int iterations                   = 100;

    for (size_t size : vector_sizes) {
        test_performance<BackendType::NumCpp>("Add", size, iterations);
        test_performance<BackendType::Eigen>("Add", size, iterations);
        test_performance<BackendType::Armadillo>("Add", size, iterations);

        test_performance<BackendType::NumCpp>("Subtract", size, iterations);
        test_performance<BackendType::Eigen>("Subtract", size, iterations);
        test_performance<BackendType::Armadillo>("Subtract", size, iterations);

        std::cout << std::endl;
    }

    return 0;
}
