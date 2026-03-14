#ifndef SMIRNOVA_SEQUENCES_HPP
#define SMIRNOVA_SEQUENCES_HPP

#include <string>
#include <utility>

#include "list.hpp"

namespace smirnova {

using Numbers = List< long long >;
using NamedSeq = std::pair< std::string, Numbers >;
using Sequences = List< NamedSeq >;
using Transposed = List< Numbers >;

Sequences readSequences();
void printNames(const Sequences& seqs);
void printNumbers(const Numbers& nums);
Transposed buildTransposed(const Sequences& seqs);
long long computeSum(const Numbers& nums);
void printSums(const Transposed& transposed);

}

#endif
