#include <gtest/gtest.h>
#include <iostream>
#include "eurora/utils/exception.hpp"

using namespace eurora::utils;

TEST(ExceptionTest, BasicWhat) {
    Exception ex(ErrorCode::kIO_FileNotFound, "Test file not found", "dummy/path/file.cpp", 123);
    std::string whatMsg = ex.what();

    std::string expectedPrefix = GetErrorMessage(ErrorCode::kIO_FileNotFound);
    EXPECT_NE(whatMsg.find(expectedPrefix), std::string::npos) << "Missing error code message";
    EXPECT_NE(whatMsg.find("Test file not found"), std::string::npos) << "Missing description";
    EXPECT_NE(whatMsg.find("file.cpp"), std::string::npos) << "Missing or incorrect file name";
    EXPECT_NE(whatMsg.find("123"), std::string::npos) << "Missing line number";
    EXPECT_EQ(ex.error_code(), ErrorCode::kIO_FileNotFound);
}

TEST(ExceptionTest, ThrowErrorMacro) {
    try {
        EURORA_THROW_ERROR(ErrorCode::kMemory_OutOfMemory, "Memory exhausted");
        std::cout << "Expected Exception to be thrown" << std::endl;
    } catch (const Exception& ex) {
        std::string whatMsg = ex.what();
        EXPECT_NE(whatMsg.find(GetErrorMessage(ErrorCode::kMemory_OutOfMemory)), std::string::npos) << "Missing memory error message";
        EXPECT_NE(whatMsg.find("Memory exhausted"), std::string::npos) << "Missing description";
        EXPECT_EQ(ex.error_code(), ErrorCode::kMemory_OutOfMemory);
    } catch (...) {
        std::cout << "Expected Exception type" << std::endl;
    }
}

TEST(ExceptionTest, InvalidArgumentExceptionTest) {
    try {
        EURORA_THROW_EXCEPTION(InvalidArgumentException, "Parameter X must be positive");
        std::cout << "Expected InvalidArgumentException to be thrown" << std::endl;
    } catch (const InvalidArgumentException& ex) {
        std::string whatMsg = ex.what();
        EXPECT_NE(whatMsg.find("InvalidArgumentException:"), std::string::npos) << "Missing exception class prefix";
        EXPECT_NE(whatMsg.find("Parameter X must be positive"), std::string::npos) << "Missing detailed description";
        EXPECT_EQ(ex.error_code(), ErrorCode::kInvalidArgument);
    } catch (...) {
        std::cout << "Expected InvalidArgumentException type" << std::endl;
    }
}

TEST(ExceptionTest, FileNotFoundExceptionTest) {
    try {
        EURORA_THROW_EXCEPTION(FileNotFoundException, "Unable to open config file");
        std::cout << "Expected FileNotFoundException to be thrown" << std::endl;
    } catch (const FileNotFoundException& ex) {
        std::string whatMsg = ex.what();
        EXPECT_NE(whatMsg.find("FileNotFoundException:"), std::string::npos) << "Missing exception class prefix";
        EXPECT_NE(whatMsg.find("Unable to open config file"), std::string::npos) << "Missing detailed description";
        EXPECT_EQ(ex.error_code(), ErrorCode::kIO_FileNotFound);
    } catch (...) {
        std::cout << "Expected FileNotFoundException type" << std::endl;
    }
}
