#include <gtest/gtest.h>
#include "eurora/math/math.hpp"

using namespace eurora::math;

// Fixture for VectorOperations
class VectorOperationsTest : public ::testing::Test {
protected:
    fvec vecA;
    fvec vecB;

    void SetUp() override {
        vecA = {1.0f, 2.0f, 3.0f};
        vecB = {4.0f, 5.0f, 6.0f};
    }
};

// Test Add operation for Eigen backend
TEST_F(VectorOperationsTest, AddWithEigenBackend) {
    fvec result = Add<BackendType::Eigen>(vecA, vecB);
    EXPECT_FLOAT_EQ(result[0], 5.0f);
    EXPECT_FLOAT_EQ(result[1], 7.0f);
    EXPECT_FLOAT_EQ(result[2], 9.0f);
}

// Test Subtract operation for Eigen backend
TEST_F(VectorOperationsTest, SubtractWithEigenBackend) {
    fvec result = Subtract<BackendType::Eigen>(vecA, vecB);
    EXPECT_FLOAT_EQ(result[0], -3.0f);
    EXPECT_FLOAT_EQ(result[1], -3.0f);
    EXPECT_FLOAT_EQ(result[2], -3.0f);
}

// Test Add operation for Armadillo backend
TEST_F(VectorOperationsTest, AddWithArmadilloBackend) {
    fvec result = Add<BackendType::Armadillo>(vecA, vecB);
    EXPECT_FLOAT_EQ(result[0], 5.0f);
    EXPECT_FLOAT_EQ(result[1], 7.0f);
    EXPECT_FLOAT_EQ(result[2], 9.0f);
}

// Test Subtract operation for Armadillo backend
TEST_F(VectorOperationsTest, SubtractWithArmadilloBackend) {
    fvec result = Subtract<BackendType::Armadillo>(vecA, vecB);
    EXPECT_FLOAT_EQ(result[0], -3.0f);
    EXPECT_FLOAT_EQ(result[1], -3.0f);
    EXPECT_FLOAT_EQ(result[2], -3.0f);
}

// Test Add operation for NumCpp backend
TEST_F(VectorOperationsTest, AddWithNumCppBackend) {
    fvec result = Add<BackendType::NumCpp>(vecA, vecB);
    EXPECT_FLOAT_EQ(result[0], 5.0f);
    EXPECT_FLOAT_EQ(result[1], 7.0f);
    EXPECT_FLOAT_EQ(result[2], 9.0f);
}

// Test Subtract operation for NumCpp backend
TEST_F(VectorOperationsTest, SubtractWithNumCppBackend) {
    fvec result = Subtract<BackendType::NumCpp>(vecA, vecB);
    EXPECT_FLOAT_EQ(result[0], -3.0f);
    EXPECT_FLOAT_EQ(result[1], -3.0f);
    EXPECT_FLOAT_EQ(result[2], -3.0f);
}

// Test Add with mismatched vector sizes
TEST_F(VectorOperationsTest, AddWithSizeMismatch) {
    fvec vecC = {1.0f, 2.0f};  // Different size
    EXPECT_THROW(Add<BackendType::NumCpp>(vecA, vecC), std::invalid_argument);
}

// Test Subtract with mismatched vector sizes
TEST_F(VectorOperationsTest, SubtractWithSizeMismatch) {
    fvec vecC = {1.0f, 2.0f};  // Different size
    EXPECT_THROW(Subtract<BackendType::NumCpp>(vecA, vecC), std::invalid_argument);
}
