
#pragma once

#if !defined(PE_PE_HPP)
#define      PE_PE_HPP


#include "../src/std.hpp"

namespace pe {

#include "pe/Exceptions.hpp"
#include "pe/Const.hpp"
#include "pe/Struct.hpp"
#include "pe/HeadersTraits.hpp"
#include "pe/Aligner.hpp"
#include "pe/Detect.hpp"
#include "pe/Image.hpp"
#include "pe/HeadersUtils.hpp"
#include "pe/Utils.hpp"
#include "pe/parse/Export.hpp"
#include "pe/parse/Section.hpp"
#include "pe/parse/Tls.hpp"
#include "pe/parse/Bound.hpp"
#include "pe/parse/Relocation.hpp"
#include "pe/parse/Import.hpp"

} // namespace pe {

#endif //PE_PE_HPP
