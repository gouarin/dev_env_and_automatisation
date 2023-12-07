// SPDX-License-Identifier: BSD-3-Clause
// Copyright 2023 SPLINART TEAM. All rights reserved.

#pragma once

#include <array>

#include <utility>
#include <xtensor/xfixed.hpp>





#include <xtensor/xtensor.hpp>

namespace

splinart{struct Circle{Circle(const xt::xtensor_fixed<double, xt::xshape<2>>& center, double radius, std::size_t npoints = 50);        xt::xtensor<double, 1> theta; //!< Theta angle
        xt::xtensor<double, 2> path;  //!< coordinates of the discretized points of the circle
        xt::







        xtensor_fixed<double, xt::






        xshape<4>>





        color{
            0.61960784313725492,




0.41568627450980394, 0, 1.0}; //!< The color of the circle
    };}
