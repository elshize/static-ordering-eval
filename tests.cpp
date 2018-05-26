// MIT License
//
// Copyright (c) 2018 Michal Siedlaczek
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

//! \file tests.hpp
//! \author Michal Siedlaczek
//! \copyright MIT License

#include <sstream>
#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "static-ordering-eval.hpp"

namespace {

TEST(load_ordering, empty)
{
    std::istringstream in("");
    auto idmap = load_idmap(in);
    EXPECT_EQ(idmap.size(), 0);
}

TEST(load_ordering, trailing_newline)
{
    std::istringstream in("T1\nT2\nT3\n");
    auto idmap = load_idmap(in);
    EXPECT_EQ(idmap["T1"], 0);
    EXPECT_EQ(idmap["T2"], 1);
    EXPECT_EQ(idmap["T3"], 2);
}

TEST(load_ordering, no_trailing_newline)
{
    std::istringstream in("T1\nT2\nT3");
    auto idmap = load_idmap(in);
    EXPECT_EQ(idmap["T1"], 0);
    EXPECT_EQ(idmap["T2"], 1);
    EXPECT_EQ(idmap["T3"], 2);
}

TEST(load_judgements, trailing_white)
{
    std::unordered_map<std::string, long> idmap;
    idmap["T1"] = 0;
    idmap["T2"] = 1;
    idmap["T3"] = 2;
    std::istringstream in("T1 2\nT1 1\nT3 5\nT3 6 ");
    auto judgements = load_judgements(in, idmap);
    EXPECT_EQ(judgements.size(), 2);
    EXPECT_EQ(judgements[0].id, 2);
    EXPECT_EQ(judgements[0].rank, 6);
    EXPECT_EQ(judgements[1].id, 0);
    EXPECT_EQ(judgements[1].rank, 2);
}

TEST(load_judgements, no_trailing_white)
{
    std::unordered_map<std::string, long> idmap;
    idmap["T1"] = 0;
    idmap["T2"] = 1;
    idmap["T3"] = 2;
    std::istringstream in("T1 2\nT1 1\nT3 5\nT3 6");
    auto judgements = load_judgements(in, idmap);
    EXPECT_EQ(judgements.size(), 2);
    EXPECT_EQ(judgements[0].id, 2);
    EXPECT_EQ(judgements[0].rank, 6);
    EXPECT_EQ(judgements[1].id, 0);
    EXPECT_EQ(judgements[1].rank, 2);
}

TEST(eval_ordering, test)
{
    std::istringstream sin("T1\nT2\nT3\nT4");
    std::istringstream jin("T1 2\nT1 1\nT3 5\nT3 6\nT4 2");
    double score = eval_ordering(jin, sin);
    EXPECT_NEAR(score, 0.5, 0.000001);
}

TEST(eval_ordering, test2)
{
    std::istringstream sin("T1\nT2\nT3\nT4\nT5\nT6");
    std::istringstream jin("T1 2\nT3 6\nT4 2\nT5 0");
    double score = eval_ordering(jin, sin);
    EXPECT_NEAR(score, 0.8, 0.000001);
}

};  // namespace

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
