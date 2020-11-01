#include <amylase/mod2>
#include <type_traits>

#include <gtest/gtest.h>

using namespace atcoder;
using ll = long long;

static_assert(internal::is_modint<amylase::mod2>::value, "");
static_assert(internal::is_static_modint<amylase::mod2>::value, "");
static_assert(!internal::is_dynamic_modint<amylase::mod2>::value, "");

static_assert(amylase::mod2::mod() == 2, "");

TEST(Mod2Test, Increment) {
    using sint = amylase::mod2;

    sint a;
    a = 0;
    ASSERT_EQ(1, (++a).val());
    ASSERT_EQ(0, (++a).val());
    ASSERT_EQ(1, (++a).val());
    ASSERT_EQ(0, (++a).val());
    a = 1;
    ASSERT_EQ(0, (--a).val());
    ASSERT_EQ(1, (--a).val());
    ASSERT_EQ(0, (--a).val());
    ASSERT_EQ(1, (--a).val());
}

TEST(Mod2Test, StaticUsage) {
    using mint = amylase::mod2;
    ASSERT_EQ(2, mint::mod());
    ASSERT_EQ(0, +mint(4));
    ASSERT_EQ(0, -mint(4));

    ASSERT_FALSE(mint(1) == mint(2));
    ASSERT_TRUE(mint(1) != mint(2));
    ASSERT_TRUE(mint(1) == mint(3));
    ASSERT_FALSE(mint(1) != mint(3));

    EXPECT_DEATH(mint(3).pow(-1), ".*");
}


TEST(Mod2Test, Constructor) {
    using mint = amylase::mod2;
    ASSERT_EQ(1, mint(true).val());
    ASSERT_EQ(1, mint((char)(3)).val());
    ASSERT_EQ(1, mint((signed char)(3)).val());
    ASSERT_EQ(1, mint((unsigned char)(3)).val());
    ASSERT_EQ(1, mint((short)(3)).val());
    ASSERT_EQ(1, mint((unsigned short)(3)).val());
    ASSERT_EQ(1, mint((int)(3)).val());
    ASSERT_EQ(1, mint((unsigned int)(3)).val());
    ASSERT_EQ(1, mint((long)(3)).val());
    ASSERT_EQ(1, mint((unsigned long)(3)).val());
    ASSERT_EQ(1, mint((long long)(3)).val());
    ASSERT_EQ(1, mint((unsigned long long)(3)).val());
    ASSERT_EQ(0, mint((signed char)(-10)).val());
    ASSERT_EQ(0, mint((short)(-10)).val());
    ASSERT_EQ(0, mint((int)(-10)).val());
    ASSERT_EQ(0, mint((long)(-10)).val());
    ASSERT_EQ(0, mint((long long)(-10)).val());

    ASSERT_EQ(0, (int(1) + mint(1)).val());
    ASSERT_EQ(0, (short(1) + mint(1)).val());

    mint m;
    ASSERT_EQ(0, m.val());
}
