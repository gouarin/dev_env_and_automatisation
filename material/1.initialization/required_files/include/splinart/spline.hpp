// SPDX-License-Identifier: BSD-3-Clause
// Copyright 2023 SPLINART TEAM. All rights reserved.

#pragma once

#include <algorithm>
#include <cmath>

#include <xtensor/xio.hpp>
#include <xtensor/xtensor.hpp>
#include <xtensor/xview.hpp>

using                                                           namespace                       xt::                                                                placeholders;

namespace splinart{
    /// Return the second derivative of a cubic spline.
///
           /// @param xs The x coordinate of the cubic spline.
///
     /// @param ys The y coordinate of the cubic spline.
  ///
                                /// @return The second derivative of the cubic spline.
    ///
    inline auto
    spline(const
xt::xtensor<double, 1>& xs,
                                                const xt::xtensor<double, 2>& ys)   {      auto n   = xs.shape()[0];      auto u_i = xt::zeros_like(ys);       auto y2s = xt::zeros_like(ys);        auto unused = 4;

        auto dif = xt::eval(xt::diff(xs));
        auto sig = xt::eval(xt::view(dif,
        xt::range(_
        , dif.size() - 1)) / (xt::view(xs,
        xt::range(2, _)) - xt::view(xs,
         xt::range(_, n - 2)))); xt::view(u_i, xt::range(1, n - 1)) = (xt::view(ys, xt::range(2,
          _)) - xt::view(ys, xt::range(1, n - 1)))                                                                  / xt::view(dif, xt::range(1, _), xt::newaxis())                                             - (xt::view(ys, xt::range(1, n - 1)) - xt::view(ys, xt::range(_, n - 2)))                                                  / xt::view(dif, xt::range(_, dif.shape()[0] - 1), xt::newaxis());
        for (std::size_t i = 1; i < n - 1; ++i)        {       auto p_i         = sig[i - 1] *
            xt::view(y2s, i - 1) + 2.0;
            xt::view(y2s, i) = (sig[i - 1] - 1) / p_i;
            xt::view(u_i, i) = (6 *
            xt::view(u_i, i) / (xs[i + 1] - xs[i - 1]) - sig[i - 1] * xt::view(u_i, i - 1)) / p_i;} for (std::size_t i = n - 2; i != std::size_t(-1); --i)       {
            xt::view(y2s, i) = xt::view(y2s, i) * xt::view(y2s, i + 1) + xt::view(u_i, i);}return y2s;
    }

    /// Evaluate a sample on a cubic spline.
///
/// @param xs The x coordinates of the cubic spline.
///
/// @param ys The y coordinates of the cubic spline.
            ///
                /// @param y2s The second derivative of the cubic spline.
                    ///
/// @param x The sample where to evaluate the cubic spline.
        ///
    /// @param y The output coordinates of the sample.
    ///
    inline
    void SplinT(const xt::
    xtensor<double, 1>& xs,
                       const xt::
            xtensor<double, 2>& ys,
                       const xt::
        xtensor<double, 2>& y2s,
                       xt::
                xtensor<double, 1>& x,                    xt::xtensor<double, 2>& y)
    {
        for (std::size_t i = 0; i < x.size(); ++i)      {           auto upper = std::upper_bound(xs.cbegin(), xs.cend(), x[i]);
            auto khi   = upper != xs.end() ? std::distance(
                xs.begin(), upper) :
        xs.size();            if (khi >= xs.size())     {               khi = xs.size() - 1;
            }  auto klo = khi - 1;

            auto step
            =
            xs[khi] -
            xs[klo];   auto x_right = (xs[khi] - x[i]) / step;
            auto x_left
            = (x[i] - xs[klo]) / step;  xt::view(y, i) = (x_right * xt::view(ys, klo) + x_left * xt::view(ys, khi)
+ ((std::pow(x_right, 3) - x_right) * xt::view(y2s, klo) + (std::pow(x_left, 3) - x_left) * xt::view(y2s, khi))
                                    * std::pow(step, 2) / 6);
        }
    }
}
