/*
 *  This source code is part of Fermi: a finite element code
 *  to solve the neutron diffusion problem for nuclear reactor
 *  designs.
 *
 *  Copyright (C) - 2019 - Guido Giuntoli <gagiuntoli@gmail.com>
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "fermi.hpp"

int fem_inigau(void) {
  int i, j, gp;

  xp_segm_2 = (double **)calloc(2, sizeof(double *));
  wp_segm_2 = (double *)calloc(2, sizeof(double));
  sh_segm_2 = (double **)calloc(2, sizeof(double *));
  ds_segm_2 = (double ***)calloc(2, sizeof(double **));
  for (i = 0; i < 2; i++) ds_segm_2[i] = (double **)calloc(1, sizeof(double *));
  for (i = 0; i < 2; i++) {
    xp_segm_2[i] = (double *)calloc(1, sizeof(double));
    sh_segm_2[i] = (double *)calloc(2, sizeof(double));
    for (j = 0; j < 1; j++) ds_segm_2[i][j] = (double *)calloc(2, sizeof(double));
  }
  xp_segm_2[0][0] = -0.577350269189626;
  wp_segm_2[0] = +1.0;
  xp_segm_2[1][0] = +0.577350269189626;
  wp_segm_2[1] = +1.0;
  for (gp = 0; gp < 2; gp++) {
    sh_segm_2[0][gp] = +0.5 * (1.0 - xp_segm_2[gp][0]);
    ds_segm_2[0][0][gp] = -0.5;

    sh_segm_2[1][gp] = +0.5 * (1.0 + xp_segm_2[gp][0]);
    ds_segm_2[1][0][gp] = +0.5;
  }

  xp_tria_3 = (double **)calloc(3, sizeof(double *));
  wp_tria_3 = (double *)calloc(3, sizeof(double));
  sh_tria_3 = (double **)calloc(3, sizeof(double *));
  ds_tria_3 = (double ***)calloc(3, sizeof(double **));
  for (i = 0; i < 3; i++) ds_tria_3[i] = (double **)calloc(2, sizeof(double *));
  for (i = 0; i < 3; i++) {
    xp_tria_3[i] = (double *)calloc(2, sizeof(double));
    sh_tria_3[i] = (double *)calloc(3, sizeof(double));
    for (j = 0; j < 2; j++) ds_tria_3[i][j] = (double *)calloc(3, sizeof(double));
  }
  xp_tria_3[0][0] = +0.166666666666666;
  xp_tria_3[0][1] = +0.166666666666666;
  wp_tria_3[0] = +0.166666666666666;

  xp_tria_3[1][0] = +0.666666666666666;
  xp_tria_3[1][1] = +0.166666666666666;
  wp_tria_3[1] = +0.166666666666666;

  xp_tria_3[2][0] = +0.166666666666666;
  xp_tria_3[2][1] = +0.666666666666666;
  wp_tria_3[2] = +0.166666666666666;

  for (gp = 0; gp < 3; gp++) {
    sh_tria_3[0][gp] = +1.0 - xp_tria_3[gp][0] - xp_tria_3[gp][1];
    ds_tria_3[0][0][gp] = -1.0;
    ds_tria_3[0][1][gp] = -1.0;

    sh_tria_3[1][gp] = +xp_tria_3[gp][0];
    ds_tria_3[1][0][gp] = +1.0;
    ds_tria_3[1][1][gp] = +0.0;

    sh_tria_3[2][gp] = +xp_tria_3[gp][1];
    ds_tria_3[2][0][gp] = +0.0;
    ds_tria_3[2][1][gp] = +1.0;
  }

  xp_quad_4 = (double **)calloc(4, sizeof(double *));
  wp_quad_4 = (double *)calloc(4, sizeof(double));
  sh_quad_4 = (double **)calloc(4, sizeof(double *));
  ds_quad_4 = (double ***)calloc(4, sizeof(double **));

  for (i = 0; i < 4; i++) ds_quad_4[i] = (double **)calloc(2, sizeof(double *));
  for (i = 0; i < 4; i++) {
    xp_quad_4[i] = (double *)calloc(2, sizeof(double));
    sh_quad_4[i] = (double *)calloc(4, sizeof(double));
    for (j = 0; j < 2; j++) ds_quad_4[i][j] = (double *)calloc(4, sizeof(double));
  }

  xp_quad_4[0][0] = -0.577350269189626;
  xp_quad_4[0][1] = -0.577350269189626;
  wp_quad_4[0] = +1.0;
  xp_quad_4[1][0] = +0.577350269189626;
  xp_quad_4[1][1] = -0.577350269189626;
  wp_quad_4[1] = +1.0;
  xp_quad_4[2][0] = +0.577350269189626;
  xp_quad_4[2][1] = +0.577350269189626;
  wp_quad_4[2] = +1.0;
  xp_quad_4[3][0] = -0.577350269189626;
  xp_quad_4[3][1] = +0.577350269189626;
  wp_quad_4[3] = +1.0;
  for (gp = 0; gp < 4; gp++) {
    sh_quad_4[0][gp] = (1.0 - xp_quad_4[gp][0]) * (1.0 - xp_quad_4[gp][1]) * 0.25;
    ds_quad_4[0][0][gp] = -1.0 * (1.0 - xp_quad_4[gp][1]) * 0.25;
    ds_quad_4[0][1][gp] = -1.0 * (1.0 - xp_quad_4[gp][0]) * 0.25;

    sh_quad_4[1][gp] = (1.0 + xp_quad_4[gp][0]) * (1.0 - xp_quad_4[gp][1]) * 0.25;
    ds_quad_4[1][0][gp] = +1.0 * (1.0 - xp_quad_4[gp][1]) * 0.25;
    ds_quad_4[1][1][gp] = -1.0 * (1.0 + xp_quad_4[gp][0]) * 0.25;

    sh_quad_4[2][gp] = (1.0 + xp_quad_4[gp][0]) * (1.0 + xp_quad_4[gp][1]) * 0.25;
    ds_quad_4[2][0][gp] = +1.0 * (1.0 + xp_quad_4[gp][1]) * 0.25;
    ds_quad_4[2][1][gp] = +1.0 * (1.0 + xp_quad_4[gp][0]) * 0.25;

    sh_quad_4[3][gp] = (1.0 - xp_quad_4[gp][0]) * (1.0 + xp_quad_4[gp][1]) * 0.25;
    ds_quad_4[3][0][gp] = -1.0 * (1.0 + xp_quad_4[gp][1]) * 0.25;
    ds_quad_4[3][1][gp] = +1.0 * (1.0 - xp_quad_4[gp][0]) * 0.25;
  }

  xp_tetra_4 = (double **)calloc(4, sizeof(double *));
  wp_tetra_4 = (double *)calloc(4, sizeof(double));
  sh_tetra_4 = (double **)calloc(4, sizeof(double *));
  ds_tetra_4 = (double ***)calloc(4, sizeof(double **));
  for (i = 0; i < 4; i++) ds_tetra_4[i] = (double **)calloc(3, sizeof(double *));
  for (i = 0; i < 4; i++) {
    xp_tetra_4[i] = (double *)calloc(3, sizeof(double));
    sh_tetra_4[i] = (double *)calloc(4, sizeof(double));
    for (j = 0; j < 3; j++) ds_tetra_4[i][j] = (double *)calloc(4, sizeof(double));
  }
  xp_tetra_4[0][0] = +0.138196601125011;
  xp_tetra_4[0][1] = +0.138196601125011;
  xp_tetra_4[0][2] = +0.138196601125011;
  wp_tetra_4[0] = +0.041666666666666;

  xp_tetra_4[1][0] = +0.585410196624968;
  xp_tetra_4[1][1] = +0.138196601125011;
  xp_tetra_4[1][2] = +0.138196601125011;
  wp_tetra_4[1] = +0.041666666666666;

  xp_tetra_4[2][0] = +0.138196601125011;
  xp_tetra_4[2][1] = +0.585410196624968;
  xp_tetra_4[2][2] = +0.138196601125011;
  wp_tetra_4[2] = +0.041666666666666;

  xp_tetra_4[3][0] = +0.138196601125011;
  xp_tetra_4[3][1] = +0.138196601125011;
  xp_tetra_4[3][2] = +0.585410196624968;
  wp_tetra_4[3] = +0.041666666666666;

  for (gp = 0; gp < 4; gp++) {
    sh_tetra_4[0][gp] = 1.0 - xp_tetra_4[gp][0] - xp_tetra_4[gp][1] - xp_tetra_4[gp][2];
    ds_tetra_4[0][0][gp] = -1.0;
    ds_tetra_4[0][1][gp] = -1.0;
    ds_tetra_4[0][2][gp] = -1.0;

    sh_tetra_4[1][gp] = xp_tetra_4[gp][0];
    ds_tetra_4[1][0][gp] = +1.0;
    ds_tetra_4[1][1][gp] = +0.0;
    ds_tetra_4[1][2][gp] = +0.0;

    sh_tetra_4[2][gp] = xp_tetra_4[gp][1];
    ds_tetra_4[2][0][gp] = +0.0;
    ds_tetra_4[2][1][gp] = +1.0;
    ds_tetra_4[2][2][gp] = +0.0;

    sh_tetra_4[3][gp] = xp_tetra_4[gp][2];
    ds_tetra_4[3][0][gp] = +0.0;
    ds_tetra_4[3][1][gp] = +0.0;
    ds_tetra_4[3][2][gp] = +1.0;
  }

  xp_prism_6 = (double **)calloc(6, sizeof(double *));
  wp_prism_6 = (double *)calloc(6, sizeof(double));
  sh_prism_6 = (double **)calloc(6, sizeof(double *));
  ds_prism_6 = (double ***)calloc(6, sizeof(double **));
  for (i = 0; i < 6; i++) ds_prism_6[i] = (double **)calloc(3, sizeof(double *));
  for (i = 0; i < 6; i++) {
    xp_prism_6[i] = (double *)calloc(3, sizeof(double));
    sh_prism_6[i] = (double *)calloc(6, sizeof(double));
    for (j = 0; j < 3; j++) ds_prism_6[i][j] = (double *)calloc(6, sizeof(double));
  }
  xp_prism_6[0][0] = +0.166666666666666;
  xp_prism_6[0][1] = +0.166666666666666;
  xp_prism_6[0][2] = -0.577350269189626;  //+0.166666666666666;
  wp_prism_6[0] = +0.166666666666666;

  xp_prism_6[1][0] = +0.666666666666666;
  xp_prism_6[1][1] = +0.166666666666666;
  xp_prism_6[1][2] = -0.577350269189626;  //+0.166666666666666;
  wp_prism_6[1] = +0.166666666666666;

  xp_prism_6[2][0] = +0.166666666666666;
  xp_prism_6[2][1] = +0.666666666666666;
  xp_prism_6[2][2] = -0.577350269189626;  //+0.166666666666666;
  wp_prism_6[2] = +0.166666666666666;

  xp_prism_6[3][0] = +0.166666666666666;
  xp_prism_6[3][1] = +0.166666666666666;
  xp_prism_6[3][2] = +0.577350269189626;  //+0.666666666666666;
  wp_prism_6[3] = +0.166666666666666;

  xp_prism_6[4][0] = +0.666666666666666;
  xp_prism_6[4][1] = +0.166666666666666;
  xp_prism_6[4][2] = +0.577350269189626;  //+0.666666666666666;
  wp_prism_6[4] = +0.166666666666666;

  xp_prism_6[5][0] = +0.166666666666666;
  xp_prism_6[5][1] = +0.666666666666666;
  xp_prism_6[5][2] = +0.577350269189626;  //+0.666666666666666;
  wp_prism_6[5] = +0.166666666666666;

  for (gp = 0; gp < 6; gp++) {
    sh_prism_6[0][gp] = (1.0 - xp_prism_6[gp][0] - xp_prism_6[gp][1]) * (1.0 - xp_prism_6[gp][2]) * 0.5;
    ds_prism_6[0][0][gp] = -1.0 * (1.0 - xp_prism_6[gp][2]) * 0.5;
    ds_prism_6[0][1][gp] = -1.0 * (1.0 - xp_prism_6[gp][2]) * 0.5;
    ds_prism_6[0][2][gp] = -1.0 * (1.0 - xp_prism_6[gp][0] - xp_prism_6[gp][1]) * 0.5;

    sh_prism_6[1][gp] = xp_prism_6[gp][0] * (1.0 - xp_prism_6[gp][2]) * 0.5;
    ds_prism_6[1][0][gp] = +1.0 * (1.0 - xp_prism_6[gp][2]) * 0.5;
    ds_prism_6[1][1][gp] = +0.0;
    ds_prism_6[1][2][gp] = -1.0 * xp_prism_6[gp][0] * 0.5;

    sh_prism_6[2][gp] = xp_prism_6[gp][1] * (1.0 - xp_prism_6[gp][2]) * 0.5;
    ds_prism_6[2][0][gp] = +0.0;
    ds_prism_6[2][1][gp] = +1.0 * (1.0 - xp_prism_6[gp][2]) * 0.5;
    ds_prism_6[2][2][gp] = -1.0 * xp_prism_6[gp][1] * 0.5;

    sh_prism_6[3][gp] = (1.0 - xp_prism_6[gp][0] - xp_prism_6[gp][1]) * (1.0 + xp_prism_6[gp][2]) * 0.5;
    ds_prism_6[3][0][gp] = -1.0 * (1.0 + xp_prism_6[gp][2]) * 0.5;
    ds_prism_6[3][1][gp] = -1.0 * (1.0 + xp_prism_6[gp][2]) * 0.5;
    ds_prism_6[3][2][gp] = +1.0 * (1.0 - xp_prism_6[gp][0] - xp_prism_6[gp][1]) * 0.5;

    sh_prism_6[4][gp] = xp_prism_6[gp][0] * (1.0 + xp_prism_6[gp][2]) * 0.5;
    ds_prism_6[4][0][gp] = +1.0 * (1.0 + xp_prism_6[gp][2]) * 0.5;
    ds_prism_6[4][1][gp] = +0.0;
    ds_prism_6[4][2][gp] = +1.0 * xp_prism_6[gp][0] * 0.5;

    sh_prism_6[5][gp] = xp_prism_6[gp][1] * (1.0 + xp_prism_6[gp][2]) * 0.5;
    ds_prism_6[5][0][gp] = +0.0;
    ds_prism_6[5][1][gp] = +1.0 * (1.0 + xp_prism_6[gp][2]) * 0.5;
    ds_prism_6[5][2][gp] = +1.0 * xp_prism_6[gp][1] * 0.5;
  }

  xp_hexa_8 = (double **)calloc(8, sizeof(double *));
  wp_hexa_8 = (double *)calloc(8, sizeof(double));
  sh_hexa_8 = (double **)calloc(8, sizeof(double *));
  ds_hexa_8 = (double ***)calloc(8, sizeof(double **));
  for (i = 0; i < 8; i++) ds_hexa_8[i] = (double **)calloc(3, sizeof(double *));
  for (i = 0; i < 8; i++) {
    xp_hexa_8[i] = (double *)calloc(3, sizeof(double));
    sh_hexa_8[i] = (double *)calloc(8, sizeof(double));
    for (j = 0; j < 3; j++) ds_hexa_8[i][j] = (double *)calloc(8, sizeof(double));
  }
  xp_hexa_8[0][0] = -0.577350269189626;
  xp_hexa_8[0][1] = -0.577350269189626;
  xp_hexa_8[0][2] = -0.577350269189626;
  wp_hexa_8[0] = +1.0;

  xp_hexa_8[1][0] = +0.577350269189626;
  xp_hexa_8[1][1] = -0.577350269189626;
  xp_hexa_8[1][2] = -0.577350269189626;
  wp_hexa_8[1] = +1.0;

  xp_hexa_8[2][0] = +0.577350269189626;
  xp_hexa_8[2][1] = +0.577350269189626;
  xp_hexa_8[2][2] = -0.577350269189626;
  wp_hexa_8[2] = +1.0;

  xp_hexa_8[3][0] = -0.577350269189626;
  xp_hexa_8[3][1] = +0.577350269189626;
  xp_hexa_8[3][2] = -0.577350269189626;
  wp_hexa_8[3] = +1.0;

  xp_hexa_8[4][0] = -0.577350269189626;
  xp_hexa_8[4][1] = -0.577350269189626;
  xp_hexa_8[4][2] = +0.577350269189626;
  wp_hexa_8[4] = +1.0;

  xp_hexa_8[5][0] = +0.577350269189626;
  xp_hexa_8[5][1] = -0.577350269189626;
  xp_hexa_8[5][2] = +0.577350269189626;
  wp_hexa_8[5] = +1.0;

  xp_hexa_8[6][0] = +0.577350269189626;
  xp_hexa_8[6][1] = +0.577350269189626;
  xp_hexa_8[6][2] = +0.577350269189626;
  wp_hexa_8[6] = +1.0;

  xp_hexa_8[7][0] = -0.577350269189626;
  xp_hexa_8[7][1] = +0.577350269189626;
  xp_hexa_8[7][2] = +0.577350269189626;
  wp_hexa_8[7] = +1.0;

  for (gp = 0; gp < 8; gp++) {
    sh_hexa_8[0][gp] = (1.0 - xp_hexa_8[gp][0]) * (1.0 - xp_hexa_8[gp][1]) * (1.0 - xp_hexa_8[gp][2]) * 0.125;
    ds_hexa_8[0][0][gp] = -1.0 * (1.0 - xp_hexa_8[gp][1]) * (1.0 - xp_hexa_8[gp][2]) * 0.125;
    ds_hexa_8[0][1][gp] = -1.0 * (1.0 - xp_hexa_8[gp][0]) * (1.0 - xp_hexa_8[gp][2]) * 0.125;
    ds_hexa_8[0][2][gp] = -1.0 * (1.0 - xp_hexa_8[gp][0]) * (1.0 - xp_hexa_8[gp][1]) * 0.125;

    sh_hexa_8[1][gp] = (1.0 + xp_hexa_8[gp][0]) * (1.0 - xp_hexa_8[gp][1]) * (1.0 - xp_hexa_8[gp][2]) * 0.125;
    ds_hexa_8[1][0][gp] = +1.0 * (1.0 - xp_hexa_8[gp][1]) * (1.0 - xp_hexa_8[gp][2]) * 0.125;
    ds_hexa_8[1][1][gp] = -1.0 * (1.0 + xp_hexa_8[gp][0]) * (1.0 - xp_hexa_8[gp][2]) * 0.125;
    ds_hexa_8[1][2][gp] = -1.0 * (1.0 + xp_hexa_8[gp][0]) * (1.0 - xp_hexa_8[gp][1]) * 0.125;

    sh_hexa_8[2][gp] = (1.0 + xp_hexa_8[gp][0]) * (1.0 + xp_hexa_8[gp][1]) * (1.0 - xp_hexa_8[gp][2]) * 0.125;
    ds_hexa_8[2][0][gp] = +1.0 * (1.0 + xp_hexa_8[gp][1]) * (1.0 - xp_hexa_8[gp][2]) * 0.125;
    ds_hexa_8[2][1][gp] = +1.0 * (1.0 + xp_hexa_8[gp][0]) * (1.0 - xp_hexa_8[gp][2]) * 0.125;
    ds_hexa_8[2][2][gp] = -1.0 * (1.0 + xp_hexa_8[gp][0]) * (1.0 + xp_hexa_8[gp][1]) * 0.125;

    sh_hexa_8[3][gp] = (1.0 - xp_hexa_8[gp][0]) * (1.0 + xp_hexa_8[gp][1]) * (1.0 - xp_hexa_8[gp][2]) * 0.125;
    ds_hexa_8[3][0][gp] = -1.0 * (1.0 + xp_hexa_8[gp][1]) * (1.0 - xp_hexa_8[gp][2]) * 0.125;
    ds_hexa_8[3][1][gp] = +1.0 * (1.0 - xp_hexa_8[gp][0]) * (1.0 - xp_hexa_8[gp][2]) * 0.125;
    ds_hexa_8[3][2][gp] = -1.0 * (1.0 - xp_hexa_8[gp][0]) * (1.0 + xp_hexa_8[gp][1]) * 0.125;

    sh_hexa_8[4][gp] = (1.0 - xp_hexa_8[gp][0]) * (1.0 - xp_hexa_8[gp][1]) * (1.0 + xp_hexa_8[gp][2]) * 0.125;
    ds_hexa_8[4][0][gp] = -1.0 * (1.0 - xp_hexa_8[gp][1]) * (1.0 + xp_hexa_8[gp][2]) * 0.125;
    ds_hexa_8[4][1][gp] = -1.0 * (1.0 - xp_hexa_8[gp][0]) * (1.0 + xp_hexa_8[gp][2]) * 0.125;
    ds_hexa_8[4][2][gp] = +1.0 * (1.0 - xp_hexa_8[gp][0]) * (1.0 - xp_hexa_8[gp][1]) * 0.125;

    sh_hexa_8[5][gp] = (1.0 + xp_hexa_8[gp][0]) * (1.0 - xp_hexa_8[gp][1]) * (1.0 + xp_hexa_8[gp][2]) * 0.125;
    ds_hexa_8[5][0][gp] = +1.0 * (1.0 - xp_hexa_8[gp][1]) * (1.0 + xp_hexa_8[gp][2]) * 0.125;
    ds_hexa_8[5][1][gp] = -1.0 * (1.0 + xp_hexa_8[gp][0]) * (1.0 + xp_hexa_8[gp][2]) * 0.125;
    ds_hexa_8[5][2][gp] = +1.0 * (1.0 + xp_hexa_8[gp][0]) * (1.0 - xp_hexa_8[gp][1]) * 0.125;

    sh_hexa_8[6][gp] = (1.0 + xp_hexa_8[gp][0]) * (1.0 + xp_hexa_8[gp][1]) * (1.0 + xp_hexa_8[gp][2]) * 0.125;
    ds_hexa_8[6][0][gp] = +1.0 * (1.0 + xp_hexa_8[gp][1]) * (1.0 + xp_hexa_8[gp][2]) * 0.125;
    ds_hexa_8[6][1][gp] = +1.0 * (1.0 + xp_hexa_8[gp][0]) * (1.0 + xp_hexa_8[gp][2]) * 0.125;
    ds_hexa_8[6][2][gp] = +1.0 * (1.0 + xp_hexa_8[gp][0]) * (1.0 + xp_hexa_8[gp][1]) * 0.125;

    sh_hexa_8[7][gp] = (1.0 - xp_hexa_8[gp][0]) * (1.0 + xp_hexa_8[gp][1]) * (1.0 + xp_hexa_8[gp][2]) * 0.125;
    ds_hexa_8[7][0][gp] = -1.0 * (1.0 + xp_hexa_8[gp][1]) * (1.0 + xp_hexa_8[gp][2]) * 0.125;
    ds_hexa_8[7][1][gp] = +1.0 * (1.0 - xp_hexa_8[gp][0]) * (1.0 + xp_hexa_8[gp][2]) * 0.125;
    ds_hexa_8[7][2][gp] = +1.0 * (1.0 - xp_hexa_8[gp][0]) * (1.0 + xp_hexa_8[gp][1]) * 0.125;
  }

  return 0;
}

int fem_caljac(double **coor, double ***ds, int npe, int gp, int dim, double **jac) {
  int i, j, n;
  if (!jac || !coor || !ds) return 1;
  for (i = 0; i < dim; i++) {
    for (j = 0; j < dim; j++) {
      jac[i][j] = 0.0;
      for (n = 0; n < npe; n++) {
        jac[i][j] += ds[n][i][gp] * coor[n][j];
      }
    }
  }
  return 0;
}

int fem_invjac(double **jac, int dim, double **ijac, double *det) {
  if (!jac || !ijac) return 1;
  if (dim == 1) {
    *det = jac[0][0];
    ijac[0][0] = 1.0 / (*det);
    return 0;
  } else if (dim == 2) {
    double c00, c01, c10, c11;
    (*det) = jac[0][0] * jac[1][1] - jac[1][0] * jac[0][1];
    c00 = +jac[1][1];
    c01 = -jac[1][0];
    c10 = -jac[0][1];
    c11 = +jac[0][0];
    ijac[0][0] = c00 / (*det);
    ijac[0][1] = c10 / (*det);
    ijac[1][0] = c01 / (*det);
    ijac[1][1] = c11 / (*det);
    return 0;
  } else if (dim == 3) {
    double c00, c01, c02, c10, c11, c12, c20, c21, c22;
    c00 = +jac[1][1] * jac[2][2] - jac[2][1] * jac[1][2];
    c01 = -jac[1][0] * jac[2][2] + jac[2][0] * jac[1][2];
    c02 = +jac[1][0] * jac[2][1] - jac[2][0] * jac[1][1];
    c10 = -jac[0][1] * jac[2][2] + jac[2][1] * jac[0][2];
    c11 = +jac[0][0] * jac[2][2] - jac[2][0] * jac[0][2];
    c12 = -jac[0][0] * jac[2][1] + jac[2][0] * jac[0][1];
    c20 = +jac[0][1] * jac[1][2] - jac[1][1] * jac[0][2];
    c21 = -jac[0][0] * jac[1][2] + jac[1][0] * jac[0][2];
    c22 = +jac[0][0] * jac[1][1] - jac[1][0] * jac[0][1];

    (*det) = jac[0][0] * c00 + jac[0][1] * c01 + jac[0][2] * c02;
    ijac[0][0] = c00 / (*det);
    ijac[0][1] = c10 / (*det);
    ijac[0][2] = c20 / (*det);
    ijac[1][0] = c01 / (*det);
    ijac[1][1] = c11 / (*det);
    ijac[1][2] = c21 / (*det);
    ijac[2][0] = c02 / (*det);
    ijac[2][1] = c12 / (*det);
    ijac[2][2] = c22 / (*det);
    return 0;
  }
  return 1;
}

int fem_calder(double **ijac, int nsh, int dim, int gp, double ***oder, double **der) {
  int i, j, sh;
  if (!ijac || !oder || !der) return 1;
  for (sh = 0; sh < nsh; sh++) {
    for (i = 0; i < dim; i++) {
      der[sh][i] = 0.0;
      for (j = 0; j < dim; j++) {
        der[sh][i] += ijac[i][j] * oder[sh][j][gp];
      }
    }
  }
  return 0;
}

int fem_calshp(int npe, int dim, double ***sh) {
  switch (dim) {
    case 1:
      break;
    case 2:
      switch (npe) {
        case 3:
          *sh = sh_tria_3;
          break;
        case 4:
          *sh = sh_quad_4;
          break;
        default:
          return 1;
          break;
      }
      break;
    case 3:
      switch (npe) {
        case 4:
          *sh = sh_tetra_4;
          break;
        case 6:
          *sh = sh_prism_6;
          break;
        case 8:
          *sh = sh_hexa_8;
          break;
        default:
          return 1;
          break;
      }
      break;
    default:
      return 1;
      break;
  }
  return 0;
}

int fem_calode(int npe, int dim, double ****oder) {
  switch (dim) {
    case 1:
      break;
    case 2:
      switch (npe) {
        case 3:
          *oder = ds_tria_3;
          break;
        case 4:
          *oder = ds_quad_4;
          break;
        default:
          return 1;
          break;
      }
      break;
    case 3:
      switch (npe) {
        case 4:
          *oder = ds_tetra_4;
          break;
        case 6:
          *oder = ds_prism_6;
          break;
        case 8:
          *oder = ds_hexa_8;
          break;
        default:
          return 1;
          break;
      }
      break;
    default:
      return 1;
      break;
  }
  return 0;
}

int fem_calwei(int npe, int dim, double **wp) {
  switch (dim) {
    case 1:
      break;
    case 2:
      switch (npe) {
        case 3:
          *wp = wp_tria_3;
          return 0;
        case 4:
          *wp = wp_quad_4;
          return 0;
        default:
          return 1;
      }
      break;
    case 3:
      switch (npe) {
        case 4:
          *wp = wp_tetra_4;
          return 0;
        case 6:
          *wp = wp_prism_6;
          return 0;
        case 8:
          *wp = wp_hexa_8;
          return 0;
        default:
          return 1;
      }
    default:
      return 1;
  }
  return 1;
}

int fem_calare(double **pts, int npe, int dim, double *area) {
  int d;
  double v1[3], v2[3], v3[3], vr[3], mod;
  *area = 0.0;
  if (dim == 1) {
    return 0;
  } else if (dim == 2) {
    return 0;
  } else if (dim == 3) {
    if (npe == 3) {
      for (d = 0; d < 3; d++) {
        v1[d] = pts[1][d] - pts[0][d];
        v2[d] = pts[2][d] - pts[0][d];
      }
      fem_vcross(v1, v2, vr);
      fem_vecmod(vr, 3, &mod);
      *area += mod;
      return 0;
    } else if (npe == 4) {
      for (d = 0; d < 3; d++) {
        v1[d] = pts[1][d] - pts[0][d];
        v2[d] = pts[2][d] - pts[0][d];
        v3[d] = pts[3][d] - pts[0][d];
      }
      fem_vcross(v1, v2, vr);
      fem_vecmod(vr, 3, &mod);
      *area += mod;
      fem_vcross(v3, v2, vr);
      fem_vecmod(vr, 3, &mod);
      *area += mod;
      return 0;
    }
  }
  return 1;
}

int fem_vecmod(double *vec, int n, double *mod) {
  int d;
  if (!vec || !mod) return 1;
  *mod = 0.0;
  for (d = 0; d < n; d++) *mod += pow(vec[d], 2);
  *mod = sqrt(*mod);
  return 0;
}

int fem_vcross(double *v1, double *v2, double *vr) {
  if (!v1 || !v2 || !vr) return 1;
  vr[0] = v1[1] * v2[2] - v2[1] * v1[2];
  vr[1] = v1[0] * v2[2] - v2[2] * v1[0];
  vr[2] = v1[0] * v2[1] - v2[0] * v1[1];
  return 0;
}

int fem_dotdsh(int i, int j, double **derivs, int dim, double *p) {
  int d;

  *p = 0.0;
  for (d = 0; d < dim; d++) {
    *p += derivs[i][d] * derivs[j][d];
  }
  return 0;
}
