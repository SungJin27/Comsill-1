#include "waterObj.h"
#include <math.h>

double waterObj::water_path(double target_x, double target_y, double line_x1, double line_y1, double line_x2, double line_y2) {
	double line_slope = (line_y2 - line_y1) / (line_x2 - line_x1);
	double dot_slope;

	while (1) {
		dot_slope = (target_y - line_y1) / (target_x - line_x1);

		if (fabs(line_slope - dot_slope) <= 0.01) break;

		if (target_y >= 718) break;

		target_y = target_y + 0.25;
	}

	return target_y;
}