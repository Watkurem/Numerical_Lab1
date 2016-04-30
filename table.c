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

#include <stdio.h>
#include <stdlib.h>
#include "table.h"

/* print_divline
 *
 * As you may have guessed, this prints a horizontal line for a table. The line
 * in question has a complex structure: it has several sections, each of which
 * has it's own length. The characters that start and end the line, and the
 * character that makes up sections, and the character that connects the
 * sections have to be customized by passing them into function. To achieve
 * best results, you'll have to manually sync length of these sections and
 * sections that print_row will output before calling the functions.
 *
 * width: an array that contains the widths (or lengths) of line's sections in
 *    order;
 * n_width: number of sections;
 * first: pointer to character that should start the line;
 * filler: pointer to character that should fill the line sections;
 * connector: pointer to character that should connect sections;
 * last: pointer to character that should end the line.
 *
 * Pointers to characters are used so that multibyte encodings work (you can
 * pass string in).
 */
void print_divline(size_t width[],
                   size_t n_width,
                   char *first,
                   char *filler,
                   char *connector,
                   char *last){
	size_t i, j;
	printf(first);
	for(i = 0; i < n_width; i++){
		for(j = 0; j < width[i]; j++)
			printf(filler);
		if(i != n_width-1)
			printf(connector);
	}
	printf("%s\n", last);
}

/* print_row
 *
 * This prints out a row of a table. Row has a complex structure: it contains
 * several sections, each of which is a string. To achieve best results, you'll
 * have to manually sync length of these sections and sections that
 * print_divline will output before calling the functions.
 *
 * columns: an array of strings that are sections of the table row;
 * n_columns: number of table columns (and, obviously, number of sections in
 *    the row)
 * first: pointer to character that should start the line;
 * connector: pointer to character that should connect sections;
 * last: pointer to character that should end the line.
 *
 * Pointers to characters are used so that multibyte encodings work (you can
 * pass string in).
 */
void print_row(char *columns[],
               size_t n_columns,
               char *first,
               char *connector,
               char *last){
	size_t i;
	printf(first);
	for(i = 0; i < n_columns; i++){
		printf("%s", columns[i]);
		if(i != n_columns-1)
			printf(connector);
	}
	printf("%s\n", last);
}

/* print_table
 *
 * This prints out data in a pretty table. Number of rows, columns and their
 * contents are all customizable.
 *
 * header: an array of strings that are sections of the table row. This will be
 *    the first row of the table, and will also be the row that determines the
 *    width of each column;
 * n_columns: number of columns (and also number of elements in header);
 * rows: an array of arrays of strings (array of rows). Should have the same
 *    number of columns as header, with the same widths;
 * n_rows: number of rows.
 */
void print_table(char *header[],
                 size_t n_columns,
                 char ***rows,
                 size_t n_rows){
	size_t h, i;
	size_t width[n_columns];
	for(i = 0; i < n_columns; i++){
			/* mbstowcs is used so that extra characters will not be printed */
		width[i] = mbstowcs(NULL, header[i], 0);
	}
	print_divline(width, n_columns, "┏", "━", "┳", "┓");
	print_row(header, n_columns, "┃", "┃", "┃");
	print_divline(width, n_columns, "┡", "━", "╇", "┩");
	for(h = 0; h < n_rows; h++){
		print_row(rows[h], n_columns, "│", "│", "│");
		if(h != n_rows-1){
			print_divline(width, n_columns, "├", "─", "┼", "┤");
		}
	}
	print_divline(width, n_columns, "└", "─", "┴", "┘");
}
