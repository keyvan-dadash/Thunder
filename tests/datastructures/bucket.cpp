#include <string>


#include <gtest/gtest.h>

#include <thunder/datastructures/bucket.hpp>


TEST(BucketBasicOperations, CreateBucket) {
    //we should test this way with lambda expression bcs we got error when using normal mode EXPECT_NO_THROW
    //see https://stackoverflow.com/questions/41681243/compilation-error-using-googletests-expect-no-throw-with-stdarray
    EXPECT_NO_THROW((
        []{thunder::datastructures::bucket<int, int, std::allocator<std::pair<int, int>>, 4> map;}()
    ));
    EXPECT_NO_THROW((
        []{thunder::datastructures::bucket<char, char, std::allocator<std::pair<char, char>>, 4> map;}()
    ));
    EXPECT_NO_THROW((
        []{thunder::datastructures::bucket<float, float, std::allocator<std::pair<float, float>>, 4> map;}()
    ));
    EXPECT_NO_THROW((
        []{thunder::datastructures::bucket<std::string, std::string, std::allocator<std::pair<std::string, std::string>>, 4> map;}()
    ));

}