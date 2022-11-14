#include <gtest/gtest.h>

#include <string>
#include <thunder/datastructures/bucket.hpp>

TEST(BucketBasicOperations, CreateBucket) {
  // we should test this way with lambda expression bcs we got error when using
  // normal mode EXPECT_NO_THROW see
  // https://stackoverflow.com/questions/41681243/compilation-error-using-googletests-expect-no-throw-with-stdarray
  EXPECT_NO_THROW(([] {
    thunder::datastructures::bucket<int, int,
                                    std::allocator<std::pair<int, int>>, 4>
        bucket;
  }()));
  EXPECT_NO_THROW(([] {
    thunder::datastructures::bucket<char, char,
                                    std::allocator<std::pair<char, char>>, 4>
        bucket;
  }()));
  EXPECT_NO_THROW(([] {
    thunder::datastructures::bucket<float, float,
                                    std::allocator<std::pair<float, float>>, 4>
        bucket;
  }()));
  EXPECT_NO_THROW(([] {
    thunder::datastructures::bucket<
        std::string, std::string,
        std::allocator<std::pair<std::string, std::string>>, 4>
        bucket;
  }()));
}

TEST(BucketBasicOperations, SetKeyValueAndReteriveKey) {
  {
    std::allocator<std::pair<int, int>> alloca;

    std::allocator_traits<decltype(alloca)> traits_;

    thunder::datastructures::bucket<int, int, decltype(alloca), 4> bucket;
    int a = 5, b = 6;

    traits_.construct(
        alloca, std::addressof(bucket.returnChangeableKeyValue(2)),
        std::piecewise_construct, std::forward_as_tuple(std::forward<int>(10)),
        std::forward_as_tuple(std::forward<int>(10)));

    EXPECT_TRUE(bucket.setKeyValueOnIndex(a, b, 2));
    EXPECT_EQ(5, bucket.returnKey(2));

    traits_.destroy(alloca, std::addressof(bucket.returnChangeableKeyValue(2)));

    traits_.construct(
        alloca, std::addressof(bucket.returnChangeableKeyValue(3)),
        std::piecewise_construct, std::forward_as_tuple(std::forward<int>(10)),
        std::forward_as_tuple(std::forward<int>(10)));

    EXPECT_TRUE(bucket.setKeyValueOnIndex(b, a, 3));
    EXPECT_EQ(6, bucket.returnKey(3));
    bucket.setSlotStatus(3, true);

    traits_.destroy(alloca, std::addressof(bucket.returnChangeableKeyValue(3)));
  };
}