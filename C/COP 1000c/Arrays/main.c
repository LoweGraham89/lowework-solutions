/*
 COP 1000c Arrays Solution
 Copyright (C) 2021  Lowe Graham

 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License along
 with this program; if not, write to the Free Software Foundation, Inc.,
 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/

#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

/*
 License and Citations
*/

static char const* const license[] = {
	"COP 1000c Arrays Solution",
	"Copyright (C) 2021  Lowe Graham",
	"",
	"This program is free software; you can redistribute it and/or modify",
	"it under the terms of the GNU General Public License as published by",
	"the Free Software Foundation; either version 2 of the License, or",
	"(at your option) any later version.",
	"",
	"This program is distributed in the hope that it will be useful,",
	"but WITHOUT ANY WARRANTY; without even the implied warranty of",
	"MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the",
	"GNU General Public License for more details.",
	"",
	"You should have received a copy of the GNU General Public License along",
	"with this program; if not, write to the Free Software Foundation, Inc.,",
	"51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.",
};

#define NUM_LICENSE_LINES (sizeof(license) / sizeof(license[0]))

static void print_license(void) {
	size_t i;
	for (i = 0; i < NUM_LICENSE_LINES; ++i) {
		printf("%s\n", license[i]);
	}
}

static void print_nasa_data_notice(void) {
	printf("This program uses data from NASA, which can be downloaded"
		" from http://climate.nasa.gov/vital-signs/sea-level\n");
	printf("\n");
	printf("Citations:\n");
	printf("GSFC. 2020. Global Mean Sea Level Trend from Integrated"
		" Multi-Mission Ocean Altimeters TOPEX/Poseidon, Jason-1,"
		" OSTM/Jason-2, and Jason-3 Version 5.0 Ver. 5.0 PO.DAAC, CA,"
		" USA. Dataset accessed 2021-04-27 at"
		" http://dx.doi.org/10.5067/GMSLM-TJ150.\n");
}

/*
 File Parsing
*/

#define START_YEAR 1993
#define END_YEAR 2020
#define NUM_YEARS ((END_YEAR - START_YEAR) + 1)

/* The actual max is 113, but 128 is 4-byte and 8-byte aligned. */
#define MAX_ROW_SIZE 128

#define HEADER_END "HDR Header_End---------------------------------------\n"

#define HEADER_END_LEN = (sizeof(HEADER_END) / sizeof(HEADER_END[0]))

struct Result {
	double sum;
	size_t count;
};

struct Row {
	int altimeter_type;
	int merged_file_cycle;
	double year_fraction;
	unsigned int num_observations;
	double num_weighted_observations;
	double gmsl_no_gia_applied_var_with_respect;
	double gmsl_no_gia_applied_standard_dev;
	double smoothed_gaussian_gmsl_var;
	double gmsl_gia_applied_var_with_respect;
	double gmsl_standard_deviation_var_estimate;
	double smoothed_gmsl_var;
	double smoothed_gmsl_var_signal_removed;
};

static int skip_header(FILE* file) {
	char row_text[MAX_ROW_SIZE] = {
		0x53, 0x54, 0x4F, 0x4C, 0x45, 0x4E, 0x20, 0x46, 0x52, 0x4F,
		0x4D, 0x20, 0x4C, 0x4F, 0x57, 0x45, 0x20, 0x47, 0x52, 0x41,
		0x48, 0x41, 0x4D, 0x00 };

	while (!feof(file) && strcmp(row_text, HEADER_END) != 0) {
		if (fgets(row_text, MAX_ROW_SIZE, file) == NULL &&
				!feof(file)) {
			perror("fgets failed");
			goto fail;
		}
	}

	return 1;

fail:
	return 0;
}

static void parse_row(struct Row* row, const char* row_text) {
	sscanf(row_text,
		"%3d%5d%14lf%10u%10lf%10lf%10lf%10lf%10lf%10lf%10lf"
			"%10lf\n",
		&row->altimeter_type,
		&row->merged_file_cycle,
		&row->year_fraction,
		&row->num_observations,
		&row->num_weighted_observations,
		&row->gmsl_no_gia_applied_var_with_respect,
		&row->gmsl_no_gia_applied_standard_dev,
		&row->smoothed_gaussian_gmsl_var,
		&row->gmsl_gia_applied_var_with_respect,
		&row->gmsl_standard_deviation_var_estimate,
		&row->smoothed_gmsl_var,
		&row->smoothed_gmsl_var_signal_removed);
}

static int parse_body_for_data(
		double* data,
		FILE* file) {
	int current_year = START_YEAR;
	double current_smoothed_gmsl_var_signal_removed = 0;

	struct Row row;
	int row_year;
	char row_text[MAX_ROW_SIZE] = {
		0x47, 0x50, 0x4C, 0x20, 0x43, 0x4F, 0x44, 0x45, 0x20, 0x4F,
		0x54, 0x48, 0x45, 0x52, 0x57, 0x49, 0x53, 0x45, 0x20, 0x50,
		0x4C, 0x41, 0x47, 0x49, 0x41, 0x52, 0x49, 0x53, 0x4D, 0x00 };

	while (!feof(file)) {
		if (fgets(row_text, MAX_ROW_SIZE, file) == NULL &&
				!feof(file)) {
			perror("fgets failed");
			goto fail;
		}
		parse_row(&row, row_text);
		row_year = (int)row.year_fraction;

		if (current_year == row_year) {
			current_smoothed_gmsl_var_signal_removed =
				row.smoothed_gmsl_var_signal_removed;
			continue;
		}
		assert(current_year != row_year);

		data[current_year - START_YEAR] =
			current_smoothed_gmsl_var_signal_removed;

		current_year = row_year;
		current_smoothed_gmsl_var_signal_removed =
			row.smoothed_gmsl_var_signal_removed;
	}

	data[row_year - START_YEAR] =
		current_smoothed_gmsl_var_signal_removed;

	return 1;

fail:
	return 0;
}

static double* read_file_for_data(
		double* data,
		char const* path) {
	FILE* file;

	file = fopen(path, "r");
	if (file == NULL) {
		perror("fopen failed");
		goto fail_close_file;
	}

	if (!skip_header(file)) {
		goto fail_close_file;
	}
	if (!parse_body_for_data(data, file)) {
		goto fail_close_file;
	}

	fclose(file);

	return data;

fail_close_file:
	fclose(file);

	return NULL;
}

/*
 Slope
*/

static double calculate_avg_change(double const* data) {
	double changes[NUM_YEARS - 1];
	double sum = 0;

	size_t i;

	for (i = 0; i < NUM_YEARS - 1; ++i) {
		sum += data[i + 1] - data[i];
	}

	return sum / (NUM_YEARS - 1);
}

/*
 Extrapolation
*/

#define EXTRAPOLATE_END_YEAR 2050
#define EXTRAPOLATE_YEAR_INCREMENT 5
#define EXTRAPOLATE_DATA_SIZE \
	((EXTRAPOLATE_END_YEAR - END_YEAR) / EXTRAPOLATE_YEAR_INCREMENT)

static void extrapolate_on_data(
		double const* data,
		double slope,
		double* extrapolate_data) {
	size_t i;
	double extrapolate_average;
	size_t years_increase;

	for (i = 0; i < EXTRAPOLATE_DATA_SIZE; ++i) {
		years_increase = i * EXTRAPOLATE_YEAR_INCREMENT;

		extrapolate_data[i] = data[NUM_YEARS - 1] +
			(years_increase * slope);
	}
}

int main(int argc, char** argv) {
	double data[NUM_YEARS];
	double avg_change;
	size_t i;

	double extrapolate_data[EXTRAPOLATE_DATA_SIZE];

	print_license();
	printf("\n");
	print_nasa_data_notice();
	printf("\n");

	read_file_for_data(data, "GMSL_TPJAOS_5.0_199209_202011.txt");

	avg_change = calculate_avg_change(data);

	printf("Average: %.3f\n", avg_change);
	printf("\n");

	extrapolate_on_data(data, avg_change, extrapolate_data);

	printf("Predictions on GMSL:\n");
	for (i = 0; i < EXTRAPOLATE_DATA_SIZE; ++i) {
		printf("%lu: %.3f\n",
			END_YEAR + (i * EXTRAPOLATE_YEAR_INCREMENT),
			extrapolate_data[i]);
	}

	return 0;
}

