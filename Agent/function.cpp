/*****************************************************************************************
 * SPDX-License-Identifier: MIT
 * SPDX-FileCopyrightText: © 2022 Stephane Sanchez <stephane.sanchez@ut-capitole.fr>
 * Université Toulouse Capitole - Institut de Recherche en Informatique de Toulouse
 * Real Expression Artificial Life (REVA) Research Team
 * https://www.irit.fr/departement/calcul-intensif-simulation-optimisation/reva/
 *****************************************************************************************/

#include "function.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define _USE_MATH_DEFINES
#include <math.h>

/**
 * @brief
 * Tire au sort un chiffre entre min et max
 * @return double
 */
double tirage_au_sort(double min, double max)
{
    double random_number;
    random_number = min + (((double)rand() / RAND_MAX) * fabs(max - min));
    return random_number;
}

/**
 * @brief
 * Calcul la moyenne des valeurs dans un tableau "table" de doubles
 * @param table tableau 1D dont il faut calculer la moyenne
 * @param number_of_rows Nombre de valeurs dans le tableau
 * @return double
 */
double moyenne(double *table, int number_of_rows)
{
    double somme = 0;
    for (int i = 0; i < number_of_rows; i++)
    {
        somme = somme + table[i];
    }
    double moyenne_table;
    moyenne_table = somme / number_of_rows;
    return moyenne_table;
}
double moyenne_abs(double *table, int number_of_rows)
{
    double somme = 0;
    for (int i = 0; i < number_of_rows; i++)
    {
        somme = somme + fabs(table[i]);
    }
    double moyenne_table;
    moyenne_table = somme / number_of_rows;
    return moyenne_table;
}
double calcul_moyenne(double *tableau, int n_values)
{
    double somme = 0;
    for (int i = 0; i < n_values; i++)
    {
        somme = somme + tableau[i];
    }
    double moyenne_table;
    moyenne_table = somme / n_values;
    return moyenne_table;
}

/**
 * @brief
 * Calcul de la moyenne d'un tableau d'entiers
 * @param table tableau 1D dont il faut calculer la moyenne
 * @param number_of_rows Nombre de valeurs dans le tableau
 * @return double
 */
double moyenne_h(int *table, int number_of_rows)
{
    double somme = 0;
    for (int i = 0; i < number_of_rows; i++)
    {
        somme = somme + table[i];
    }
    double moyenne_table;
    moyenne_table = somme / number_of_rows;
    return moyenne_table;
}

/**
 * @brief Rend la valeurs maximale d'un tableau double
 *
 * @param table tableau 1D dont il faut calculer la valeur maximale
 * @param number_of_rows Nombre de valeurs dans le tableau
 * @return double
 */
double calcul_max(double *table, int number_of_rows)
{
    double max = table[0];
    for (int i = 1; i < number_of_rows; i++)
    {
        if (table[i] > max)
        {
            max = table[i];
        }
    }
    return max;
}

/**
 * @brief Rend la valeurs maximale d'un tableau entier
 *
 * @param table tableau 1D dont il faut calculer la valeur maximale
 * @param number_of_rows Nombre de valeurs dans le tableau
 * @return double
 */
int calcul_max_i(int *table, int number_of_rows)
{
    int max = table[0];
    for (int i = 1; i < number_of_rows; i++)
    {
        if (table[i] > max)
        {
            max = table[i];
        }
    }
    return max;
}

/**
 * @brief Rend la valeurs minimale d'un tableau double
 *
 * @param table tableau 1D dont il faut calculer la valeur minimale
 * @param number_of_rows Nombre de valeurs dans le tableau
 * @return double
 */
double calcul_min(double *table, int number_of_rows)
{
    double min;
    min = table[0];
    for (int i = 1; i < number_of_rows; i++)
    {
        if (table[i] < min)
        {
            min = table[i];
        }
    }
    return min;
}

/**
 * @brief Renvoie un tableau de nombre_de_valeurs valeurs aléatoires distribuées selon une distribution gaussienne
 *
 * @param table Tableau 1D vide qui sera rempli avec les valeurs aléatoires
 * @param nombre_de_valeurs Nombre de valeurs aléatoires nécessaires
 * @return double*  Rend le tableau
 */
double *distribution_gaussienne(double *table, int nombre_de_valeurs, double min, double max)
{
    double a, b, g;
    for (int i = 0; i < nombre_de_valeurs; i++)
    {
        a = tirage_au_sort(min, max);
        b = tirage_au_sort(min, max);
        g = sqrt(-2 * log(a)) * cos(2 * M_PI * b);
        table[i] = g;
    }
    return table;
}

double g_generator(double min, double max)
{
    double a, b, g;
    a = min + (((double)rand() / RAND_MAX) * fabs(max - min));
    b = min + (((double)rand() / RAND_MAX) * fabs(max - min));
    g = sqrt(-2 * log(a)) * cos(2 * M_PI * b);

    return g;
}

/**
 * @brief
 * Fonction qui rend un tableau de valeurs aléatoires distribuées selon tau
 * @param table tableau vide 1D à rentrer
 * @param nombre_de_valeurs nombre de valeurs qui vont constituer le tableau
 * @param average valeurs moyenne des valeurs (fixe)
 * @return double* Rend le tableau rempli de valeurs aléatoires
 */
double *distribution_tau(double *table, int nombre_de_valeurs, double average, double min, double max)
{
    double x_1, x_2;
    for (int n = 0; n < nombre_de_valeurs; n++)
    {
        x_1 = -log(tirage_au_sort(min, max));
        x_2 = -log(tirage_au_sort(min, max));
        table[n] = average * (x_1 + x_2) / 2;
    }
    return table;
}
double tau_generator(double average, double min, double max)
{
    double x_1, x_2, tau_generated;
    x_1 = -log(min + (((double)rand() / RAND_MAX) * fabs(max - min)));
    x_2 = -log(min + (((double)rand() / RAND_MAX) * fabs(max - min)));
    tau_generated = average * (x_1 + x_2) / 2;

    return tau_generated;
}
double in_between_distance(double length, double reference_time, double current_tau, double interval)
{
    double L;
    L = length * (1 - exp(-interval / reference_time)) / (1 - exp(current_tau / reference_time));
    return L;
}
double calcul_min_1(double *table, int number_of_rows)
{

    double min = table[0];

    for (int i = 1; i < number_of_rows; i++)
    {
        if (table[i] < min)
        {
            min = table[i];
        }
    }
    return min;
}
int calcul_min_index(double *table, int number_of_rows)
{
    int index = 0;
    ;
    double min;
    min = table[0];

    for (int i = 1; i < number_of_rows; i++)
    {
        if (table[i] < min)
        {
            min = table[i];
            index = i;
        }
    }
    return index;
}

double c_average(int type, double gammapar, double gammaperp, double gammaz, char grandeur[100], int n_p)
{

    char avg[300], lines[200], grandeurs[100], grandeurs_D[100];
    double average, Average;

    if (type < 3)
    {
        if (type == 0)
        {
            sprintf(avg, "../../../../../stage_bis/fichierstxt/distributions/poissons/five/averages_%d_par%lf_perp%lf_z%lf.txt", n_p, gammapar, gammaperp, gammaz);
        } /// 1: due pesci, né 2D né 3D
        else if (type == 1)
        {
            sprintf(avg, "../../../../../stage_bis/fichierstxt/distributions/poissons/five/averages_%d_par%lf_perp%lf_z%lf.txt", 2, gammapar, gammaperp, gammaz);
        }

        // 2: valore aleatorio di 5 pesci, né 2d né 3d
        else if (type == 2)
        {
            sprintf(avg, "../../../../../Desktop/programmes/distributions/averages_%d_par%lf_perp%lf_z%lf.txt", n_p, gammapar, gammaperp, 0.0);
        }
        printf("%s\n", avg);
        FILE *av = fopen(avg, "r");
        if (NULL == av)
        {
            perror("8 opening database");
        }

        fgets(lines, 200, av);
        sscanf(lines, "%s %lf", grandeurs, &Average);
        // printf("line: %s  grandeur: %s\n", grandeurs, grandeur);

        if (strcmp(grandeur, grandeurs) == 0)
        {
            average = Average;
        }
        while (fgets(lines, 200, av) != NULL)
        {
            sscanf(lines, "%s %lf", grandeurs, &Average);
            // printf("line: %s  grandeur: %s\n", grandeurs, grandeur);

            if (strcmp(grandeur, grandeurs) == 0)
            {
                average = Average;
            }
        }

        fclose(av);
    }
    // 3:valore NON ALEATORIO, 2D
    else
    {
        if (type == 3)
        {
            sprintf(avg, "../../../../../stage_bis/fichierstxt/distributions/poissons/five/averages_%d_par%lf_perp%lf_z%lf.txt", n_p, gammapar, gammaperp, gammaz);
            sprintf(grandeurs_D, "%s2D", grandeur);
        }
        // 4: valore ALEATORIO, 2D
        if (type == 4)
        {
            sprintf(avg, "../../../../../Desktop/programmes/distributions/averages_%d_par%lf_perp%lf_z%lf.txt", n_p, gammapar, gammaperp, 0.0);
            sprintf(grandeurs_D, "%s2D", grandeur);
        }
        // 5: valore non aleatorio, 2 pesci 3D
        if (type == 5)
        {
            sprintf(avg, "../../../../../stage_bis/fichierstxt/distributions/poissons/five/averages_%d_par%lf_perp%lf_z%lf.txt", 2, gammapar, gammaperp, gammaz);
            sprintf(grandeurs_D, "%s3D", grandeur);
        }

        // 3D, valore non aleatorio

        if (type == 6)
        {
            sprintf(avg, "../../../../../stage_bis/fichierstxt/distributions/poissons/five/averages_%d_par%lf_perp%lf_z%lf.txt", n_p, gammapar, gammaperp, gammaz);
            sprintf(grandeurs_D, "%s3D", grandeur);
        }
        // 4: valore ALEATORIO, 3D
        if (type == 7)
        {
            sprintf(avg, "../../../../../Desktop/programmes/distributions/averages_%d_par%lf_perp%lf_z%lf.txt", n_p, gammapar, gammaperp, 0.0);
            sprintf(grandeurs_D, "%s3D", grandeur);
        }
        // 5: valore non aleatorio, 2 pesci 3D
        if (type == 8)
        {
            sprintf(avg, "../../../../../stage_bis/fichierstxt/distributions/poissons/five/averages_%d_par%lf_perp%lf_z%lf.txt", 2, gammapar, gammaperp, gammaz);
            sprintf(grandeurs_D, "%s3D", grandeur);
        }

        else if (type == 9)
        {
            sprintf(avg, "../../../../../stage_bis/fichierstxt/distributions/poissons/five/averages_%d_par%lf_perp%lf_z%lf.txt", 2, gammapar, gammaperp, gammaz);
            sprintf(grandeurs_D, "%s2D", grandeur);
        }
        else if (type == 10)
        {
            sprintf(avg, "../../../../../stage_bis/fichierstxt/distributions/poissons/five/averages_%d_par%lf_perp%lf_z%lf.txt", 2, gammapar, gammaperp, gammaz);
            sprintf(grandeurs_D, "%s3D", grandeur);
        }

        FILE *av_2 = fopen(avg, "r");
        if (NULL == av_2)
        {
            perror("8 opening database");
        }

        fgets(lines, 200, av_2);
        sscanf(lines, "%s %lf", grandeurs, &Average);
        // printf("line: %s  grandeur: %s\n", grandeurs, grandeurs_D);

        if (strcmp(grandeurs_D, grandeurs) == 0)
        {
            average = Average;
        }
        while (fgets(lines, 200, av_2) != NULL)
        {
            sscanf(lines, "%s %lf", grandeurs, &Average);
            // printf("line: %s  grandeur: %s\n", grandeurs, grandeur);

            if (strcmp(grandeurs_D, grandeurs) == 0)
            {
                average = Average;
            }
        }

        fclose(av_2);
    }
    return average;
}

double modulo2(double value, double max, double min)

{
    if (value < min)
    {
        do
        {
            value = value + (max - min);

        } while (value < min);
    }
    if (value >= max)
    {
        do
        {
            value = value - (max - min);

        } while (value >= max);
    }

    return value;
}

