// SPDX-License-Identifier: BSD-3-Clause
// Copyright 2023 SPLINART TEAM. All rights reserved.

#include <gtest/gtest.h>
#include <splinart/shapes.hpp>
#include <splinart/spline.hpp>
#include <xtensor/xio.hpp>

struct CircleParam
{
    xt::xtensor<double, 1> center;
    double radius;
};

class SplineTest : public testing::TestWithParam<std::tuple<CircleParam, std::size_t>>
{
};

TEST_P(SplineTest, spline)
{
    auto params = GetParam();

    auto center  = std::get<0>(params).center;
    auto radius  = std::get<0>(params).radius;
    auto npoints = std::get<1>(params);

    splinart::Circle circle(center, radius, npoints);

    auto y2s   = splinart::spline(circle.theta, circle.path);
    auto y_new = xt::zeros_like(circle.path);
    splinart::SplinT(circle.theta, circle.path, y2s, circle.theta, y_new);

    EXPECT_EQ(circle.path, y_new);
}

// clang-format off
INSTANTIATE_TEST_SUITE_P(spline,
                         SplineTest,
                         testing::Combine(testing::Values(CircleParam{{0, 0}, 1},
                                                          CircleParam{{0.5, 0.1}, 0.1},
                                                          CircleParam{{-0.5, 0.6}, 0.3}),
                                          testing::Values(11, 21, 31)));
// clang-format on
