#include <filesystem>

#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>

using namespace testing;
namespace fs = std::filesystem;

std::string generateCompilerCommand(const fs::path& file, const fs::path& incDir, const fs::path& libDir)
{
    std::stringstream command;

    command << "g++ "
            << "-std=c++20 "
            << "-Wall -Wextra -Wpedantic "
            << "-I" << incDir.string() << " "
            << file.string() << " "
            << "-lNFSE-lib "
            << "-L" << libDir << " "
            << "2>/dev/null"
            ;


    return command.str();
}

AssertionResult attemptCompile(const fs::path& file, const fs::path& incDir, const fs::path& libDir)
{
    const std::string& command = generateCompilerCommand(file, incDir, libDir);
    int result = std::system(command.c_str());

    if (result)
    {
        return AssertionFailure() << file << " cannot be compiled";
    }
    else
    {
        return AssertionSuccess() << file << " can be compiled";;
    }

}

TEST(CompileTest_Suite, Compile_Success_Test)
{
    std::initializer_list<std::string> files =
    {
        "polymorphicvalue.cpp"
    };

    fs::path curdir = fs::current_path();
    fs::path testFilesRoot = fs::weakly_canonical(curdir / "../unittest/compiletest/expectSuccess/");
    fs::path incDir = fs::weakly_canonical(curdir / "../src/");
    fs::path libDir = fs::weakly_canonical(curdir);

    for (const std::string& filename : files)
    {
        EXPECT_TRUE(attemptCompile(testFilesRoot / filename, incDir, libDir));
    }
}

TEST(CompileTest_Suite, Compile_Fail_Test)
{
    std::string file = "file:///home/blue-chameleon/Codes/NoFancyShitExtensions/unittest/compiletest/expectSuccess/polymorphicvalue.cpp.cpp";

    std::initializer_list<std::string> files =
    {
        "polymorphicvalue.cpp"
    };

    fs::path curdir = fs::current_path();
    fs::path testFilesRoot = fs::weakly_canonical(curdir / "../unittest/compiletest/expectFail/");
    fs::path incDir = fs::weakly_canonical(curdir / "../src/");
    fs::path libDir = fs::weakly_canonical(curdir);

    for (const std::string& filename : files)
    {
        EXPECT_FALSE(attemptCompile(testFilesRoot / filename, incDir, libDir));
    }
}
