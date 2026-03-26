#ifndef SMIRNOVA_SEQUENCES_HPP
#define SMIRNOVA_SEQUENCES_HPP

#include <string>
#include <utility>

#include "../common/list.hpp"

namespace smirnova {

using Numbers = List< unsigned long long >;
using NamedSeq = std::pair< std::string, Numbers >;
using Sequences = List< NamedSeq >;
using Transposed = List< Numbers >;

}

#endif
