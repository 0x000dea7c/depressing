#pragma once

#include <cstdint>

// For better portability and clarity.

using b32 = std::uint32_t;	// To avoid potential memory alignment issues.
using i32 = std::int32_t;
using u32 = std::uint32_t;
using f32 = float;		// You'd like to use std::float32_t, but it causes problems with GLM.
using f64 = double;
