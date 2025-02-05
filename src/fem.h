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

#ifndef FERMI_H
#define FERMI_H

#include <array>
#include <cstdlib>

#include "algebra.h"
#include "node.h"

template <size_t N, size_t DIM>
class ShapeBase {
 public:
  using ShapeArray = std::array<std::array<double, N>, N>;
  using DShapeArray = std::array<std::array<std::array<double, N>, DIM>, N>;

  virtual constexpr std::array<Node, N> gaussPoints() const = 0;
  virtual constexpr std::array<double, N> weights() const = 0;
  virtual constexpr ShapeArray sh() const = 0;
  virtual constexpr DShapeArray dsh() const = 0;

  const std::string toString() const {
    std::ostringstream oss;
    oss << "Gauss Points:\n";
    for (const auto& gp : gaussPoints()) {
      oss << "  " << gp.toString() << "\n";
    }
    oss << "Weights:\n";
    for (const auto& w : weights()) {
      oss << "  " << w << "\n";
    }
    oss << "Shape Functions:\n";
    const auto& shape = sh();
    for (size_t i = 0; i < shape.size(); ++i) {
      oss << "  ";
      for (size_t j = 0; j < shape[i].size(); ++j) {
        oss << shape[i][j] << " ";
      }
      oss << "\n";
    }
    oss << "DShape Functions:\n";
    const auto& dShape = dsh();
    for (size_t i = 0; i < dShape.size(); ++i) {
      for (size_t j = 0; j < dShape[i].size(); ++j) {
        oss << "  ";
        for (size_t k = 0; k < dShape[i][j].size(); ++k) {
          oss << dShape[i][j][k] << " ";
        }
        oss << "\n";
      }
    }
    return oss.str();
  }

  virtual ~ShapeBase() = default;

  int computeInverseJacobian(Matrix<DIM>& ijac, double& det, std::vector<Node> nodes, size_t gp) const {
    DShapeArray dsh_ = dsh();

    size_t num_nodes = nodes.size();
    Matrix<DIM> jac;
    for (int i = 0; i < DIM; i++) {
      for (int j = 0; j < DIM; j++) {
        jac.data[i][j] = 0.0;
        for (int n = 0; n < num_nodes; n++) {
          jac.data[i][j] += dsh_[n][i][gp] * nodes[n].getCoor(j);
        }
      }
    }
    jac.inverse(ijac, det);
    return 0;
  }
};

class ShapeSegment2 : public ShapeBase<2, 1> {
 public:
  static constexpr size_t N = 2;
  static constexpr size_t DIM = 1;

  constexpr std::array<Node, N> gaussPoints() const override {
    return {{{-0.577350269189626, 0, 0}, {+0.577350269189626, 0, 0}}};
  }

  constexpr std::array<double, N> weights() const override { return {1.0, 1.0}; }

  constexpr ShapeArray sh() const override {
    std::array<std::array<double, N>, N> sh{};
    for (size_t gp = 0; gp < N; ++gp) {
      sh[0][gp] = +0.5 * (1.0 - gaussPoints()[gp].x);
      sh[1][gp] = +0.5 * (1.0 + gaussPoints()[gp].x);
    }
    return sh;
  }

  constexpr DShapeArray dsh() const override {
    std::array<std::array<std::array<double, N>, DIM>, N> ds{};
    for (size_t gp = 0; gp < N; ++gp) {
      ds[0][0][gp] = -0.5;
      ds[1][0][gp] = +0.5;
    }
    return ds;
  }
};

class ShapeQuad4 : public ShapeBase<4, 2> {
 public:
  static constexpr size_t NGP = 4;
  static constexpr size_t N = 4;
  static constexpr size_t DIM = 2;

  constexpr std::array<Node, N> gaussPoints() const override {
    return {{{-0.577350269189626, -0.577350269189626, 0},
             {-0.577350269189626, +0.577350269189626, 0},
             {+0.577350269189626, -0.577350269189626, 0},
             {+0.577350269189626, +0.577350269189626, 0}}};
  }

  constexpr std::array<double, N> weights() const override { return {1.0, 1.0, 1.0, 1.0}; }

  constexpr ShapeArray sh() const override {
    std::array<std::array<double, N>, N> sh{};
    for (size_t gp = 0; gp < NGP; ++gp) {
      sh[0][gp] = (1.0 - gaussPoints()[gp].x) * (1.0 - gaussPoints()[gp].y) * 0.25;
      sh[1][gp] = (1.0 - gaussPoints()[gp].x) * (1.0 + gaussPoints()[gp].y) * 0.25;
      sh[2][gp] = (1.0 + gaussPoints()[gp].x) * (1.0 - gaussPoints()[gp].y) * 0.25;
      sh[3][gp] = (1.0 + gaussPoints()[gp].x) * (1.0 + gaussPoints()[gp].y) * 0.25;
    }
    return sh;
  }

  constexpr DShapeArray dsh() const override {
    std::array<std::array<std::array<double, N>, DIM>, N> ds{};
    for (size_t gp = 0; gp < NGP; ++gp) {
      ds[0][0][gp] = -1.0 * (1.0 - gaussPoints()[gp].y) * 0.25;
      ds[0][1][gp] = -1.0 * (1.0 - gaussPoints()[gp].x) * 0.25;

      ds[1][0][gp] = -1.0 * (1.0 + gaussPoints()[gp].y) * 0.25;
      ds[1][1][gp] = +1.0 * (1.0 - gaussPoints()[gp].x) * 0.25;

      ds[2][0][gp] = +1.0 * (1.0 - gaussPoints()[gp].y) * 0.25;
      ds[2][1][gp] = -1.0 * (1.0 + gaussPoints()[gp].x) * 0.25;

      ds[3][0][gp] = +1.0 * (1.0 + gaussPoints()[gp].y) * 0.25;
      ds[3][1][gp] = +1.0 * (1.0 + gaussPoints()[gp].x) * 0.25;
    }
    return ds;
  }
};

#endif
