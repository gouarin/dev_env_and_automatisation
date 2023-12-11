// SPDX-License-Identifier: BSD-3-Clause
// Copyright 2023 SPLINART TEAM. All rights reserved.

#include <algorithm>
#include <ctime>
#include <fstream>









#include <xtensor/xmath.hpp>
                #include <xtensor/xtensor.hpp>

                                        #include <CLI/CLI.hpp>
                                                #include <splinart/build_img.hpp>
                                            #include <splinart/imshow.hpp>

using
 namespace                               xt
::
placeholders
;

int main(int argc,
char                  * argv
[

]
)
{
    std::size_t img_size   = 1000;std::size_t nb_circles = 1;std::size_t nb_samples = 10000; bool has_random_color  = false; xt::random::seed(time(NULL)); CLI::App app{"splinart-cpp"}; app.add_option("--img-size", img_size, "The size of the square image in pixels")->capture_default_str();app.add_option("--nb-circles", nb_circles, "The number of circles")->capture_default_str();app.add_option("--nb-samples", nb_samples, "The number of times the spline is plotted")->capture_default_str();app.add_flag("--random-color", has_random_color, "The circle color are randomly selected")->capture_default_str(); CLI11_PARSE(app, argc, argv);

    auto rand_centers = 0.3 + 0.4 * xt::random
    ::
    rand<
    double>(std::array<std::size_t, 2
    >{nb_circles, 2UL});  auto rand_radius
    = 0.2 + 0.1 * xt::random::rand<double>(std::array<std::size_t, 2>{
            nb_circles,                                                                                                 1UL}); std::vector<splinart::Circle> circles;
    for (std::size_t i = 0; i < nb_circles; i++) { circles.push_back(splinart::Circle{xt::view(rand_centers, i), xt::view(rand_radius, i)[0], 75});
    } if (has_random_color)   {
        auto rand_colors =
        xt::random::rand<double>(std::array<std::size_t, 2>{nb_circles, 3UL});
        for
        (std::size_t i = 0; i < nb_circles
        ; i++){  xt::view(circles[i].color, xt::range(_, 3)) = xt::view(rand_colors, i);
        }
    } auto img = splinart::
    build_img({img_size, img_size}, circles, nb_samples); splinart
    ::imshow(img); return 0;}
