/*
 *  This source code is part of Fermi: a finite element code
 *  to solve the neutron diffusion problem for nuclear reactor
 *  designs.
 *
 *  Copyright (C) - 2019 - Guido Giuntoli <gagiuntoli@gmail.com>
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as published
 *  by the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with this program. If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef ASSEMBLY_H
#define ASSEMBLY_H

#include <cassert>

#include "ellpack.h"
#include "mesh.h"

template <size_t DIM>
int assemblyA(Ellpack &A, Mesh<DIM> &mesh) {
  std::fill(A.vals.begin(), A.vals.end(), 0.0);
  for (const auto &elem : mesh.elements) {
    // std::cout << elem->toString() << std::endl;
    auto Ae = elem->computeElementMatrix();
    for (const auto &ae : Ae) {
      std::cout << ae << ",";
    }
    std::cout << std::endl;
    size_t n = elem->nodeIndexes.size();
    assert(Ae.size() == n * n);
    for (size_t i = 0; i < n; i++) {
      for (size_t j = 0; j < n; j++) {
        size_t row = elem->nodeIndexes[i];
        size_t col = elem->nodeIndexes[j];
        A.cols[row * A.non_zeros_per_row + j] = col;
        A.vals[row * A.non_zeros_per_row + j] += Ae[i * n + j];
      }
    }
  }
  return 0;
}

#endif
