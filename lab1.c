/* Copyright 2016 Alexander Melnyk / Олександр Мельник
 *
 * This file is part of Numerical_Lab1 package.
 *
 * Numerical_Lab1 is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your option)
 * any later version.
 *
 * Numerical_Lab1 is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * Numerical_Lab1. If not, see <http://www.gnu.org/licenses/>.
 */

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include "table.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

/* Different initial values determined by lab variant. */
#define A (-31.3)
#define B (-4.9)
#define EPS1 0.01 /* 10^-2 */
#define EPS2 (EPS1*0.001) /* 10^-5 */
#define EPS3 (EPS2*0.001) /* 10^-8 */
#define EPS4 (EPS3*0.001) /* 10^-11 */
#define EPS5 (EPS4*0.001) /* 10^-14 */

/* This controls how much points should be used for second table. Is tied to
 * divisor used for 'h'.
 * This could be implemented in a much nicer way, but was done like this to
 * conform to the formula provided in lab manual more.
 */
#define I_START 0
#define I_END 10

 /* These control table output format */
#define ROWS1 5
#define COLUMNS1 4
#define ROWS2 (I_END-I_START+1)
#define COLUMNS2 3

 /* These two are used fairly heavily, so defines would be too unefficient */
static double x = A;
static const double h = (B - A)/10.0;

/* compute_sincos_by_series_eps
 *
 * Recursive computation of sine/cosine function for a given x using power
 * series. Formula is taken directly from lab manual and it's correctness is
 * not guaranteed.
 * This version uses 'eps' argument to determine series length. That is, the
 * series used for computation will be long enough to provide precision better
 * than 'eps'.
 *
 * x: as in sin(x) or cos(x). Should be from 0 to pi/4 inclusive;
 * eps: minimum required precision (as in 'error must be less than this');
 * n: pointer to counter used in recursion. Should be set to 0 before call.
 *    Will hold series length (number of terms) after computation finishes;
 * summand: pointer to summand (term). Each summand is computed recursively
 *    from previous one, and after computation this will hold the remainder
 *    term (which doubles as error value);
 * is_sin: boolean. If 1/true, sine series will be computed, otherwise cosine.
 *    This is used because formulas for sine/cosine series differ very little.
 *
 * returns: sine/cosine value.
 */
double compute_sincos_by_series_eps(double x,
                                    double eps,
                                    unsigned int *n,
                                    double *summand,
                                    char is_sin){
	if(!(*n))
		*summand = is_sin?x:1;
	else
		*summand *= -x * x / ((*n) * 2 * ((*n) * 2 + (is_sin?1:-1)));
	if(fabs(*summand) < eps)
		return 0;
	(*n)++;
	return *summand + compute_sincos_by_series_eps(x, eps, n, summand, is_sin);
}

/* compute_sincos_by_series_n
 *
 * Recursive computation of sine/cosine function for a given x using power
 * series. Formula is taken directly from lab manual and it's correctness is
 * not guaranteed.
 * This version uses 'n' argument to determine series length. That is, the
 * series used for computation will have 'n' terms.
 *
 * Consider using wrapper function that encapsulates ctr.
 *
 * x: as in sin(x) or cos(x). Should be from 0 to pi/4 inclusive;
 * n: series length (number of terms) for computation;
 * ctr: counter used in recursion. Should be 0 when it is called;
 * summand: pointer to summand (term). Each summand is computed recursively
 *    from previous one, and after computation this will hold the remainder
 *    term (which doubles as error value);
 * is_sin: boolean. If 1/true, sine series will be computed, otherwise cosine.
 *    This is used because formulas for sine/cosine series differ very little.
 *
 * returns: sine/cosine value.
 */
double compute_sincos_by_series_n(double x,
                                  unsigned int n,
                                  unsigned int ctr,
                                  double * summand,
                                  char is_sin){
	if(!ctr)
		*summand = is_sin?x:1;
	else
		*summand *= -x * x / (ctr * 2 * (ctr * 2 + (is_sin?1:-1)));
	if(ctr > n)
		return 0;
	return *summand + compute_sincos_by_series_n(x, n, ++ctr, summand, is_sin);
}

/* normalize_angle_for_cos
 *
 * This function takes any angle as an argument and returns it's normalized
 * (between 0 and pi/4 inclusive) version and additional information for use
 * with compute_sincos_by_series_* functions. Seeing as my lab task was to
 * compute cosine, this function normalizes only for cosine.
 * You should use this function before calling any of the
 * compute_sincos_by_series_*, so you can have normalized angle and is_sin
 * value generated for you automatically. This applies only if you want to get
 * cosine, of course, like I did.
 *
 * in_angle: angle to normalize in radians;
 * is_sin: pointer to boolean which will be set as a result of normalization.
 *    This should be then passed to compute_sincos_by_series_* functions;
 * negative: pointer to boolean which will be set as a result of normalization.
 *    If true, return value of compute_sincos_by_series_* functions should be
 *    negated to get correct result.
 *
 * returns: normalized (between 0 and pi/4 inclusive)angle.
 */
double normalize_angle_for_cos(double in_angle,
                               char *is_sin,
                               char *negative){
	*is_sin = 0;
	*negative = 0;
	double x = fmod(in_angle, 2*M_PI);
	if(x < 0)
		x += 2*M_PI;
	if(x > M_PI/2){
		*negative = 1;
		if(x > M_PI)
			if(x > (M_PI/2)*3){
				*negative = 0;
				x = M_PI-x+M_PI;
			} else
				x -= M_PI;
		else
			x = M_PI-x;
	}
	if(x > M_PI/4){
		*is_sin = 1;
		return M_PI/2 - x;
	} else
		return x;
}

/* compute_cos_by_series_eps
 *
 * Wrapper for compute_sincos_by_series_eps. Normalizes angle beforehand.
 *
 * x: as in sin(x) or cos(x);
 * eps: minimum required precision (as in 'error must be less than this');
 * n: Will hold series length (number of terms) after computation finishes;
 * remainder: Will hold the remainder term (which doubles as error value)
 *    after computation finishes;
 *
 * returns: sine/cosine value.
 */
double compute_cos_by_series_eps(double x,
                                 double eps,
                                 unsigned int *n,
                                 double *remainder){
	char negative;
	char is_sin;
	x = normalize_angle_for_cos(x, &is_sin, &negative);
	return (negative?-1:1)*compute_sincos_by_series_eps(x,
	                                                    eps,
	                                                    n,
	                                                    remainder,
	                                                    is_sin);
}

/* compute_cos_by_series_n
 *
 * Wrapper for compute_sincos_by_series_n. Normalizes angle beforehand.
 *
 * x: as in sin(x) or cos(x);
 * n: series length (number of terms) for computation;
 * remainder: Will hold the remainder term (which doubles as error value)
 *    after computation finishes;
 *
 * returns: sine/cosine value.
 */
double compute_cos_by_series_n(double x,
                               unsigned int n,
                               double *remainder){
	char negative;
	char is_sin;
	x = normalize_angle_for_cos(x, &is_sin, &negative);
	return (negative?-1:1)*compute_sincos_by_series_n(x,
	                                                  n,
	                                                  0,
	                                                  remainder,
	                                                  is_sin);
}

int main(){
	int j, i;
	double control = cos(x);
	unsigned int n, n8;
	double leftover;
	double answer;
	double epses[ROWS1] = {EPS1, EPS2, EPS3, EPS4, EPS5};
	char *header1[COLUMNS1] = {" eps ",
	                          " n ",
	                          "   Абсолютна похибка  ",
	                          "    Залишковий член   "};
	char ***rows;
	char *header2[3] = {"  Xi  ",
	                    "   Абсолютна похибка  ",
	                    "    Залишковий член   "};

	setlocale(LC_ALL, ""); /* For mbstowcs */

	/* Init output tables */
	rows = calloc(ROWS1, sizeof(char **));
	for(j = 0; j < ROWS1; j++){
		rows[j] = calloc(COLUMNS1, sizeof(char *));
		for(i = 0; i < COLUMNS1; i++){
			/* This here makes sure that there will be no overflows when initing
			 * headers yet a sane amount of memory will be used.
			 * mbstowcs is used so that extra bytes will not be allocated for the
			 * non-header rows (which, as far as my concept goes, should never
			 * contain non-ascii strings, while headers can be non-ascii)
			 */
			rows[j][i] = calloc(mbstowcs(NULL, header1[i], 0) + 1, 1);
		}
	}

	/* Fill output tables with computed data */
	for(j = 0; j < ROWS1; j++){
		leftover = 0;
		n = 0;
		answer = compute_cos_by_series_eps(x,
		                                   epses[j],
		                                   &n,
		                                   &leftover);
		if(epses[j] == 0.00000001){
			n8 = n-1;
		}

		/* This lines can become painful if headers are too short. You will need to
		 * fiddle around with size arg of calloc in init, or just put some large
		 * value there if memory usage is not that important. NOTE */
		sprintf(rows[j][0], "%.0e", epses[j]);
		sprintf(rows[j][1], "%3u", n);
		sprintf(rows[j][2], "%22.16e", fabs(answer-control));
		sprintf(rows[j][3], "%22.16e", fabs(leftover));
	}

	/* Print nice output tables */
	print_table(header1, COLUMNS1, rows, ROWS1);

	/* Free used memory */
	for(j = 0; j < ROWS1; j++){
		for(i = 0; i < COLUMNS1; i++){
			free(rows[j][i]);
		}
		free(rows[j]);
	}
	free(rows);

	/* Init output tables for second table */
	rows = calloc(ROWS2, sizeof(char **));
	for(j = 0; j < ROWS2; j++){
		rows[j] = calloc(COLUMNS2, sizeof(char *));
		for(i = 0; i < COLUMNS2; i++){
			/* This here, again, makes sure that there will be no overflows when
			 * initing headers yet a sane amount of memory will be used */
			rows[j][i] = calloc(mbstowcs(NULL, header2[i], 0) + 1, 1);
		}
	}

	/* Fill output tables with computed data */
	for(i = 0; i < ROWS2; i++){
		leftover = 0;
		n = 0;
		x = A + h*i;
		control = cos(x);

		sprintf(rows[i][0], "%6.2f", x);
		answer = compute_cos_by_series_n(x,
		                                 n8,
		                                 &leftover);

		/* Again, these can bring problems. NOTE */
		sprintf(rows[i][1], "%22.16e", fabs(answer-control));
		sprintf(rows[i][2], "%22.16e", fabs(leftover));
	}

	/* Print nice output tables */
	print_table(header2, COLUMNS2, rows, ROWS2);

	/* Free used memory */
	for(j = 0; j < ROWS2; j++){
		for(i = 0; i < COLUMNS2; i++){
			free(rows[j][i]);
		}
		free(rows[j]);
	}
	free(rows);

	/* I should've probably done error detection too, but this program can
	 * crash only if there was not enough memory, and if you don't have enough
	 * memory for something like this then you're clearly doing something
	 * really wrong.*/
	return 0;
}
