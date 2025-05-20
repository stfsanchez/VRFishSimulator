/*****************************************************************************************
 * SPDX-License-Identifier: MIT
 * SPDX-FileCopyrightText: © 2022 Stephane Sanchez <stephane.sanchez@ut-capitole.fr>
 * Université Toulouse Capitole - Institut de Recherche en Informatique de Toulouse
 * Real Expression Artificial Life (REVA) Research Team
 * https://www.irit.fr/departement/calcul-intensif-simulation-optimisation/reva/
 *****************************************************************************************/

#ifndef FUNCTION_H
#define FUNCTION_H
double calcul_min(double *table, int number_of_rows);
double calcul_max(double *table, int number_of_rows);
double moyenne(double *table, int number_of_rows);
double moyenne_h(int *table, int number_of_rows);
double tirage_au_sort(double min, double max);
int calcul_max_i(int *table, int number_of_rows);
double* distribution_gaussienne(double* table, int nombre_de_valeurs, double min, double max);
double g_generator(double min, double max);
double* distribution_tau(double*table, int nombre_de_valeurs, double average, double min, double max);
double tau_generator(double average, double min, double max);
double in_between_distance(double length, double reference_time, double current_tau, double interval);
double calcul_min_1(double *table, int number_of_rows);
int calcul_min_index(double *table, int number_of_rows);
double modulo2(double value, double max, double min);
double calcul_moyenne(double *tableau, int n_values);
double moyenne_abs(double *table, int number_of_rows);
double c_average(int type, double gammapar, double gammaperp, double gammaz, char grandeur[100], int n_p);
#endif
