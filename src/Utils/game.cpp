#include "Globals/math.h"
#include "Globals/globals.h"

bool worldToScreenPoint(Vector3 worldPoint, view_matrix_t viewProjMtx, Vector3& screenPoint) {
	// viewProjMtx.transpose();
	Vector4 clipPoint;
	clipPoint.x = viewProjMtx.matrix[0][0] * worldPoint.x + viewProjMtx.matrix[0][1] * worldPoint.y + viewProjMtx.matrix[0][2] * worldPoint.z + viewProjMtx.matrix[0][3];
	clipPoint.y = viewProjMtx.matrix[1][0] * worldPoint.x + viewProjMtx.matrix[1][1] * worldPoint.y + viewProjMtx.matrix[1][2] * worldPoint.z + viewProjMtx.matrix[1][3];
	clipPoint.z = viewProjMtx.matrix[2][0] * worldPoint.x + viewProjMtx.matrix[2][1] * worldPoint.y + viewProjMtx.matrix[2][2] * worldPoint.z + viewProjMtx.matrix[2][3];
	clipPoint.w = viewProjMtx.matrix[3][0] * worldPoint.x + viewProjMtx.matrix[3][1] * worldPoint.y + viewProjMtx.matrix[3][2] * worldPoint.z + viewProjMtx.matrix[3][3];
	if (clipPoint.w < 3) {
		return false;
	}
	else {
		float invW = 1.0f / clipPoint.w;
		screenPoint.x = (clipPoint.x * invW + 1.0f) * 0.5f * clientWidth;
		screenPoint.y = clientHeight - (clipPoint.y * invW + 1.0f) * 0.5f * clientHeight;
		screenPoint.z = clipPoint.w;
		return true;
	}
}

