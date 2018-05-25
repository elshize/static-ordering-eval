// MIT License
//
// Copyright (c) 2018 Michał Siedlaczek
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

//! \file       static-ordering-eval.cpp
//! \author     Michal Siedlaczek
//! \copyright  MIT License

#include <string>
#include <CLI/CLI.hpp>
#include "static-ordering-eval.hpp"

int main(int argc, char** argv)
{
    std::string hfile, sfile;
    CLI::App app{"Evaluate a static document ordering against TREC judgements."};
    app.add_option("judgements", hfile, "File with human judgements", false)
        ->required();
    app.add_option("ordering", sfile, "Ordering to evaluate", false)
        ->required();

    CLI11_PARSE(app, argc, argv);

    std::ifstream jin(hfile);
    std::ifstream sin(sfile);
    double score = eval_ordering(jin, sin);
    std::cout << score << std::endl;

    return 0;
}
