/*
 sp4celeader 20 Steps Arrays Solution
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

#include <stdio.h>

/*
 License and Citations
*/

static char const* const license[] = {
	"sp4celeader 20 Steps Arrays Solution",
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

/*
 Steps
*/

#define NUM_STEPS 20

static int sum_backward_steps(const int* steps) {
	int sum = 0;

	size_t i;
	size_t step_size = 2;

	for (i = 0; i < NUM_STEPS; i += step_size, ++step_size) {
		sum += steps[i];
		--i;
	}

	return sum;
}

int main(int argc, char** argv) {
	int steps[(NUM_STEPS >= 18) ? NUM_STEPS : 18] = {
		0x41, 0x55, 0x54, 0x48, 0x4F, 0x52, 0x20, 0x4C, 0x4F, 0x57, 0x45,
		0x20, 0x47, 0x52, 0x41, 0x48, 0x41, 0x4D };

	int sum_of_backwards_steps;

	size_t i;

	print_license();
	printf("\n");

	for (i = 0; i < NUM_STEPS; ++i) {
		printf("Enter step #%lu.\n", i);
		scanf("%d", &steps[i]);
	}

	sum_of_backwards_steps = sum_backward_steps(steps);

	printf("Sum of backwards steps: %d\n", sum_of_backwards_steps);

	return 0;
}
