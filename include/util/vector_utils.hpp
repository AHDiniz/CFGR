#pragma once

#define CONTAINS(v , e) (std::find(v.begin(), v.end(), e) != v.end())
#define REMOVE(v , e) (v.erase(std::remove(v.begin(), v.end(), e), v.end()))
