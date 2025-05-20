/*****************************************************************************************
 * SPDX-License-Identifier: MIT
 * SPDX-FileCopyrightText: © 2022 Stephane Sanchez <stephane.sanchez@ut-capitole.fr>
 * Université Toulouse Capitole - Institut de Recherche en Informatique de Toulouse
 * Real Expression Artificial Life (REVA) Research Team
 * https://www.irit.fr/departement/calcul-intensif-simulation-optimisation/reva/
 *****************************************************************************************/

#ifndef RNGCONSTANTS_H
#define RNGCONSTANTS_H

/* --------------------------- */
/* Variables for random number */
/* generator ran3 (common /xaa/) */
#define MBIG 1000000000L
#define MSEED 161803398L
#define MZ 0L
#define FAC (1.0/MBIG)
#define FAC2 (FAC*FAC)
extern long* ma;  /* index 1..55 used; we will ignore index 0 */
extern long idum;
extern long seed;
extern int iff;  /* flag for initialization */
extern int inext;
extern int inextp;


#endif // RNGCONSTANTS_H
