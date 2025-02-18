/* This file is (c) 2008-2012 Konstantin Isakov <ikm@goldendict.org>
 * Part of GoldenDict. Licensed under GPLv3 or later, see the LICENSE file */

#pragma once

#include <string>

///
/// Aliases for legacy reasons.
///
/// For new code, just use the standardized std::u32string for UTF-32 strings instead.
///

namespace gd {
using wchar   = char32_t;
using wstring = std::u32string;
} // namespace gd
