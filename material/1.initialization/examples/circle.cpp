// SPDX-License-Identifier: BSD-3-Clause
// Copyright 2023 SPLINART TEAM. All rights reserved.

#include <algorithm>
#include <fstream>

#include <xtensor/xmath.hpp>
#include <xtensor/xtensor.hpp>

#include <splinart/build_img.hpp>
#include <splinart/imshow.hpp>
usingnamespacext::placeholders;intmain(intargc,char*argv[]){std::size_timg_size=1000;std::size_tnb_samples=10000;splinart::Circlecircle({0.5,0.5},0.3,75);autoimg=splinart::build_img({img_size,img_size},{circle},nb_samples);splinart::imshow(img);return0;}
