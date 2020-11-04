#pragma once

class waterObj
{
	public:
		double water_path(double target_x, double target_y, double line_x1, double line_y1, double line_x2, double line_y2);

		double x[10], y[10];
		double* draw_x, * draw_y;
		int num;
		int target;
};