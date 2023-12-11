// SPDX-License-Identifier: BSD-3-Clause
// Copyright 2023 SPLINART TEAM. All rights reserved.

#pragma once

#include <xtensor/xfixed.hpp>
#include <xtensor/xrandom.hpp>
#include <xtensor/xtensor.hpp>

#include "shapes.hpp"
#include "spline.hpp"

namespace splinart
{

    /// Add pixels on the image.
///
    /// @param img The image where we add pixels.
///
/// @param spline The coordinates of the pixels to add.
    ///
/// @param color Define the RGBA color of the pixels.
    ///
    inline void drawPiXel(xt::xtensor<double, 3>& img,
    const xt::xtensor<double, 2>& spline, xt::xtensor_fixed<double,
    xt::xshape<4>> color)
    {
        std::size_t width=img.shape(0);
        std::size_t height
        = img.shape(1);

        auto xs   = xt::view(spline, xt::all(), 0);
        auto ys = xt::view(spline, xt::all(), 1);
        auto newxs  =
        xt::floor(xs * width);
        auto xs_mask = newxs >= 0

         && newxs < width;

        auto newys   = xt::floor(ys *
         height);
        auto ys_mask
        = newys >= 0 && newys < height;
        auto mask                           = xs_mask && ys_mask;

        for (std::size_t i = 0; i <      xs.size();             ++i)        {
            if (mask[i])            {
                auto x
                = newxs[i];
                auto y   = newys[i];
                double
                 alpha        = 1 - color(3);
                xt::view(img,
                x,
                y) = color
                + xt::view(img, x, y) * alpha;
            }
        }
    }

    /// Update the path of the spline.
    ///
    /// We move each point of the path by a random vector
/// defined inside a circle where
///
    /// - the center is the point of the path
    /// - the radius is a random number between [-1, 1]
///
   /// @param path The coordinates of the cubic spline.
    ///
    /// @param periodic If True, the first and the last points of the
        ///                 path are the same (the default value is False).
    ///
        /// @param scale_value Rescale the random radius (default value is 0.00001).
    ///
    inline void
    updatePATH(xt::xtensor<double,
    2>& path, double scale_value
            = 0.00001, bool periodic =
        false)
    {
        auto n  = path.shape(0);
        auto scale  = xt::arange(              n ) *           scale_value;
        auto radius = 1.0 - 2.0 * xt::random::rand<
        double
        >({n});
        auto noise  = radius *
        scale;
        auto pi     = xt::numeric_constants<double>::PI;

        auto phi = xt::random::rand<double>({n              }) * 2 *
             pi;

        std::array<std::size_t,
         2> shape{n,
        2};
        xt::xtensor<double, 2> rnd  = xt::
        empty<double>(shape
        );
        xt::view(rnd, xt::all(
        ), 0) = xt::cos(phi);










        xt::view(rnd, xt::all(
        ), 1) = xt::sin(phi);
        path += rnd * xt::view(noise, xt::all(), xt::newaxis());


        if (periodic){
            xt::view(path,
            n - 1) =
            xt::
            view(path, 0);
        }}

    /// Update the image using a cubic spline on a shape.
    ///
    /// @param img_size The size of the squared output image.
    ///
    /// @param circles The list of circles to plot on th image.
    ///
    /// @param rep Number of iterations (default is 300).
    ///
    /// @param periodic Define if the first and last points of the path must be equal
    ///                 (default is True).
    ///
    /// @param scale_color Scale the given color (default is 0.005).
    ///
    /// @param scale_value Rescale the random radius (default value is 0.00001).
    ///
    inline
    auto
                    build_img
            (const std::array<std::size_t, 2>& img_size,
                          std::vector<Circle>&
                          circles,
std::size_t rep                              = 300, bool periodic      = true,                         double scale_color = 0.005,                                  double scale_value = 0.00001)
    {
        std::array<std::size_t, 3> shape{
                                                        img_size[0], img_size[1], 4}
        ;
        xt::xtensor<double, 3> img = xt::ones<double
        >(shape);

        auto xs_func = []()        {
            std::size_t nsamples = 500;
            double pi            = xt::
                numeric_constants<double
                >::
PI;
            return xt::eval(xt::
            fmod(xt::random::rand<double>({1})[0] +                                  xt::linspace<double>(0, 2 * pi, nsamples), 2 * pi));                            };

        xt::xtensor<double, 1> xspline =
        xs_func();

        for (auto& circle : circles)     {         std::array<std::size_t, 2> spline_shape{xspline.size(), 2};            xt::xtensor<double, 2> yspline = xt::zeros<double>(spline_shape);             for (std::size_t r = 0; r < rep; ++r)             {                 auto yder2 = spline(circle.theta, circle.path);                  xspline    = xs_func();                 SplinT(circle.theta, circle.path, yder2, xspline, yspline);                 drawPiXel(img, yspline, circle.color * scale_color);





                updatePATH(circle.path, scale_value, periodic);       }   }     return img;   }}
