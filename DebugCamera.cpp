#include "DebugCamera.h"
#include <DirectXMath.h>
#include <WinUser.h>

void DebugCamera::DebugCameraIni(HWND hwnd)
{
	input.MouseIni(hwnd);

	viewProjection.SetEyePos(0, 0, -200);
	viewProjection.SetTarget(0, 0, 0);
	viewProjection.SetUpVec(0, 1, 0);
	viewProjection.Ini(); 

	frontVec = { 0, 0, 0 };
	sideVec = { 0,0,0 };

	frontdist = 50;
}

void DebugCamera::Update(HWND hwnd)
{
	//マウスの情報の更新
	input.GetMouseState(hwnd);
	CameraMove();
	viewProjection.Update();
}

void DebugCamera::CameraMove()
{
	Vector3 proviUpVec = { 0,1,0 };
	float speed = 0.5f;
	//カメラが注視点座標より奥にいるとき
	if (viewProjection.up.y <= 0) {
		speed *= -1;
	}

	//プレイヤーの正面ベクトル
	frontVec = {
		viewProjection.target.x - viewProjection.eye.x,
		viewProjection.target.y - viewProjection.eye.y,
		viewProjection.target.z - viewProjection.eye.z
	};
	frontVec.normalize();

	sideVec = proviUpVec.cross(frontVec);
	sideVec.normalize();

	upVec = sideVec.cross(frontVec);
	upVec.normalize();

	//平行移動
	if (input.IsMouseDown(MOUSE_WHEEL)) {
		//上下に動かしたとき
		if (input.GetCursorMoveY() > 0) {
			cameraTrans -= upVec * speed;
			viewProjection.target.x -= upVec.x * speed;
			viewProjection.target.y -= upVec.y * speed;
			viewProjection.target.z -= upVec.z * speed;
		}
		else if (input.GetCursorMoveY() < 0) {
			cameraTrans += upVec * speed;
			viewProjection.target.x += upVec.x * speed;
			viewProjection.target.y += upVec.y * speed;
			viewProjection.target.z += upVec.z * speed;
		}
		//マウスカーソルを左右に動かしたとき
		if (input.GetCursorMoveX() > 0) {
			cameraTrans.x -= sideVec.x * speed;
			cameraTrans.z -= sideVec.z * speed;

			viewProjection.target.x -= sideVec.x * speed;
			viewProjection.target.z -= sideVec.z * speed;
		}
		else if (input.GetCursorMoveX() < 0) {
			cameraTrans.x += sideVec.x * speed;
			cameraTrans.z += sideVec.z * speed;

			viewProjection.target.x += sideVec.x * speed;
			viewProjection.target.z += sideVec.z * speed;
		}

	}
	//拡大縮小
	else if (!input.IsMouseDown(MOUSE_WHEEL)) {
		frontdist += -input.IsMouseWheel() * (frontdist * 0.001f);
	}
	//球面座標移動
	if (input.IsMouseDown(MOUSE_LEFT)) {
		//カメラが上を向いてるとき通常通りに座標を足す
		if (viewProjection.up.y >= 0) {
			moveDist += input.GetCursorMove() * 0.05f;
		}
		//カメラが逆さまになった時X.Z座標を反転させる
		else if (viewProjection.up.y <= 0) {
			moveDist.x -= input.GetCursorMoveX() * 0.05f;
			moveDist.y += input.GetCursorMoveY() * 0.05f;
			moveDist.z -= input.GetCursorMoveZ() * 0.05f;
		}
	}

	//カメラUP変換
	viewProjection.up = {
		0,
		cosf(moveDist.y),
		0
	};

	viewProjection.eye.x = -frontdist * sinf(moveDist.x) * cosf(moveDist.y) + cameraTrans.x;
	viewProjection.eye.y = frontdist * sinf(moveDist.y) + cameraTrans.y;
	viewProjection.eye.z = -frontdist * cosf(moveDist.x) * cosf(moveDist.y) + cameraTrans.z;
}

ViewProjection DebugCamera::GetViewProjection()
{
	return viewProjection;
}
