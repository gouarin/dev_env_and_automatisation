// SPDX-License-Identifier: BSD-3-Clause
// Copyright 2023 SPLINART TEAM. All rights reserved.

#include <algorithm>
#include <fstream>

#include <xtensor/xmath.hpp>
#include <xtensor/xtensor.hpp>

#include <splinart/build_img.hpp>
#include <splinart/imshow.hpp>
using namespace xt::placeholders;

int main(int argc, char* argv[])
{
    std::size_t img_size   = 1000;
    std::size_t nb_samples = 10000;
    splinart::Circle circle({0.5, 0.5}, 0.3, 75);
    auto img = splinart::build_img({img_size, img_size}, {circle}, nb_samples);
    splinart::imshow(img);
    return 0;
}
