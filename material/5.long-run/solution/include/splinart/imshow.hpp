// SPDX-License-Identifier: BSD-3-Clause
// Copyright 2023 SPLINART TEAM. All rights reserved.

#pragma once

#include <opencv2/opencv.hpp>

#include <xtensor/xtensor.hpp>

namespace splinart
{

    /// Show the result image using the imshow function of OpenCV
    ///
    /// @param img The image to draw
    ///
    inline void imshow(xt::xtensor<double, 3>& img)
    {
        const int width  = static_cast<int>(img.shape(0));
        const int height = static_cast<int>(img.shape(1));
        const cv::Mat M(width, height, CV_64FC4, static_cast<void*>(img.data()));
        cv::imshow("splinart", M);
        cv::waitKey(0);
    }
}
