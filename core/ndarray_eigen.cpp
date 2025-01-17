#include "eurora/core/ndarray_eigen.hpp"
#include <gtest/gtest.h>
#include <memory>
#include <vector>

using namespace eurora::core;

class NDArrayTest : public ::testing::Test {
protected:
    std::shared_ptr<NDArrayEigen<int>> array;

    void SetUp() override { array = std::make_shared<NDArrayEigen<int>>(); }
};

// TEST_F(NDArrayTest, CreateAndAccess) {
//     std::vector<size_t> dimensions = {3, 3};
//     array->Create(dimensions);

//     EXPECT_EQ(array->Size(), 9);
//     EXPECT_EQ(array->Dimensions(), dimensions);

//     // Verify data access
//     for (size_t i = 0; i < array->Size(); ++i) {
//         (*array)[i] = static_cast<int>(i);
//     }

//     for (size_t i = 0; i < array->Size(); ++i) {
//         EXPECT_EQ((*array)[i], static_cast<int>(i));
//     }
// }

// TEST_F(NDArrayTest, Reshape) {
//     std::vector<size_t> initial_dimensions = {2, 3, 2};
//     array->Create(initial_dimensions);

//     EXPECT_EQ(array->Dimensions(), initial_dimensions);

//     std::vector<size_t> new_dimensions = {3, 4};
//     array->Reshape(new_dimensions);

//     EXPECT_EQ(array->Dimensions(), new_dimensions);
//     EXPECT_EQ(array->Size(), 12);
// }

// TEST_F(NDArrayTest, Squeeze) {
//     std::vector<size_t> dimensions = {1, 5, 1, 10};
//     array->Create(dimensions);

//     EXPECT_EQ(array->Dimensions(), dimensions);

//     array->Squeeze();

//     std::vector<size_t> squeezed_dimensions = {5, 10};
//     EXPECT_EQ(array->Dimensions(), squeezed_dimensions);
// }

// TEST_F(NDArrayTest, CalculateOffset) {
//     std::vector<size_t> dimensions = {3, 4, 5};
//     array->Create(dimensions);

//     std::vector<size_t> indices = {2, 1, 3};
//     size_t offset               = array->CalculateOffset(indices);

//     EXPECT_EQ(offset, 2 * 20 + 1 * 5 + 3);  // 3D array offset formula
// }

// TEST_F(NDArrayTest, CalculateIndex) {
//     std::vector<size_t> dimensions = {3, 4, 5};
//     array->Create(dimensions);

//     size_t offset               = 38;  // Corresponds to indices {2, 1, 3}
//     std::vector<size_t> indices = array->CalculateIndex(offset);

//     EXPECT_EQ(indices, std::vector<size_t>({2, 1, 3}));
// }

// TEST_F(NDArrayTest, Subarray) {
//     std::vector<size_t> dimensions = {4, 4};
//     array->Create(dimensions);

//     // Fill the array
//     for (size_t i = 0; i < array->Size(); ++i) {
//         (*array)[i] = static_cast<int>(i);
//     }

//     std::vector<size_t> start = {1, 1};
//     std::vector<size_t> size  = {2, 2};

//     auto subarray = array->Subarray(start, size);

//     EXPECT_EQ(subarray->Dimensions(), size);

//     // Verify subarray values
//     EXPECT_EQ((*subarray)[0], (*array)[5]);
//     EXPECT_EQ((*subarray)[3], (*array)[10]);
// }

// TEST_F(NDArrayTest, ReshapeView) {
//     std::vector<size_t> dimensions = {2, 6};
//     array->Create(dimensions);

//     // Fill the array
//     for (size_t i = 0; i < array->Size(); ++i) {
//         (*array)[i] = static_cast<int>(i);
//     }

//     std::vector<size_t> new_dimensions = {3, 4};
//     auto reshaped_view                 = array->ReshapeView(new_dimensions);

//     EXPECT_EQ(reshaped_view->Dimensions(), new_dimensions);

//     // Verify data remains consistent
//     for (size_t i = 0; i < array->Size(); ++i) {
//         EXPECT_EQ((*array)[i], (*reshaped_view)[i]);
//     }
// }

// TEST_F(NDArrayTest, BoundaryConditions) {
//     std::vector<size_t> dimensions = {3, 3};
//     array->Create(dimensions);

//     EXPECT_THROW(array->Size(3), std::runtime_error);  // Dimension out of range

//     EXPECT_THROW(array->CalculateOffset({3, 2}), std::runtime_error);  // Invalid index

//     EXPECT_THROW(array->CalculateIndex(9), std::runtime_error);  // Offset out of bounds
// }
