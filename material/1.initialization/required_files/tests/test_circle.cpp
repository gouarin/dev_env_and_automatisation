// SPDX-License-Identifier: BSD-3-Clause
// Copyright 2023 SPLINART TEAM. All rights reserved.

#include <gtest/gtest.h>
#include <splinart/shapes.hpp>
#include <xtensor/xio.hpp>

class CircleTest : public testing::TestWithParam<xt::xtensor<double, 2>>
{
};

TEST_P(CircleTest, circle)
{
    xt::xtensor<double, 2> expected_path = GetParam();

    auto c = splinart::Circle({0, 0}, 1., expected_path.shape(0));

    for (std::size_t i = 0; i < expected_path.size(); ++i)
    {
        EXPECT_NEAR(c.path[i], expected_path[i], 1e-15);
    }
}

// clang-format off
INSTANTIATE_TEST_SUITE_P(circle,
                         CircleTest,
                         testing::Values(xt::xtensor<double, 2>{{1, 0}, {1, 0}},
                                         xt::xtensor<double, 2>{{1, 0}, {0, 1}, {-1, 0}, {0, -1}, {1, 0}}));
// clang-format on
