#include "fruity.h"

void
fruity_foreach(FruityConst2D a, int rows, int cols,
		FruityRowFuncConst row_func,
		FruityColFuncConst col_func,
		void* userdata)
{
	for (int i = 0; i < rows; ++i) {
		for (int j = 0; j < cols; ++j)
			if (col_func)
				col_func(a, i, j, userdata);
		if (row_func)
			row_func(a, i, userdata);
	}
}

void
fruity_transform(FruityMutable2D a, int rows, int cols,
		FruityRowFuncMutable row_func,
		FruityColFuncMutable col_func,
		void* userdata)
{
	for (int i = 0; i < rows; ++i) {
		for (int j = 0; j < cols; ++j)
			if (col_func)
				col_func(a, i, j, userdata);
		if (row_func)
			row_func(a, i, userdata);
	}
}

