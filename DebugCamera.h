#pragma once
#include "Vector3.h"
#include "DirectXInput.h"
#include "ViewProjection.h"
#include <DirectXMath.h>
using namespace DirectX;
class DebugCamera
{
public:
	void DebugCameraIni(HWND hwnd);
	void Update(HWND hwnd);


	ViewProjection GetViewProjection();

private:
	void CameraMove();

	ViewProjection viewProjection;

	Vector3 dist{};
	Vector3 moveDist{};	//球面座標
	Vector3 cameraTrans{};//平行移動座標

	//正面・横・上ベクトル
	Vector3 frontVec{};
	Vector3 sideVec{};
	Vector3 upVec{};

	MouseInput input;

	float frontdist;

};

