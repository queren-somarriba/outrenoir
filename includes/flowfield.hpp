#pragma once

#include <iostream>
#include <vector>
#include "noise.hpp"

std::vector<vec2>	makeGrid(const meridiansData& data);

void			makeSegments(vec2 start, const meridiansData& data, const std::vector<vec2>& grid, std::vector<vec2>& allSegments);