
#include "Util.h"
#include "Camera.h"
#include "WorldTransform.h"


WorldTransform::WorldTransform()
{
	position_ = { 0,0,0 };
	scale_ = { 1,1,1 };
	rotation_ = { 0,0,0 };

	constBuffTransform_ = CreateBuff(constMapTransform_);
}

void WorldTransform::SetScale(float x, float y, float z)
{
	scale_ = { x,y,z };
}

void WorldTransform::SetRotation(float x, float y, float z)
{
	rotation_ = { x,y,z };
}

void WorldTransform::SetPosition(float x, float y, float z)
{
	position_ = { x,y,z };
}

void WorldTransform::AddScale(float x, float y, float z)
{
	scale_.x += x;
	scale_.y += y;
	scale_.z += z;
}

void WorldTransform::AddRotation(float x, float y, float z)
{
	rotation_.x += x;
	rotation_.y += y;
	rotation_.z += z;
}

void WorldTransform::AddPosition(float x, float y, float z)
{
	position_.x += x;
	position_.y += y;
	position_.z += z;
}



void WorldTransform::Update(uint32_t isBillboard)
{
	XMMATRIX matScale, matRot, matTrans;

	//スケール、回転、平行移動行列の計算
	matScale = XMMatrixScaling(scale_.x, scale_.y, scale_.z);
	matRot = XMMatrixIdentity();
	matRot *= XMMatrixRotationZ(rotation_.z);
	matRot *= XMMatrixRotationX(rotation_.x);
	matRot *= XMMatrixRotationY(rotation_.y);
	matTrans = XMMatrixTranslation(
		position_.x, position_.y, position_.z);

	
	//クォータニオン
	/*q.w += 0.005f;

	matRot = q.UpdateMatrix();*/

	//ワールド行列の合成
	matWorld_ = XMMatrixIdentity();	//変形をリセット
	//ビルボード
	if (isBillboard == 1) {
		matWorld_ *= Camera::scurrent_.matBillboard_;
	}
	else if (isBillboard == 2) {
		matWorld_ *= Camera::scurrent_.matBillboardY_;
	}

	matWorld_ *= matScale;			//ワールド行列にスケーリングを反映
	matWorld_ *= matRot;				//ワールド行列に開店を反映
	matWorld_ *= matTrans;			//ワールド行列に平行移動を反映

	//親オブジェクトがあれば
	if (parent_) {
		//親オブジェクトのワールド行列を掛ける
		matWorld_ *= parent_->matWorld_;
	}

	//定数バッファへデータ転送
	constMapTransform_->mat = matWorld_;
	constMapTransform_->viewProj = Camera::scurrent_.GetMatView() * Camera::scurrent_.GetMatProjection();
	constMapTransform_->cameraPos = {
		Camera::scurrent_.eye_.x,
		Camera::scurrent_.eye_.y,
		Camera::scurrent_.eye_.z
	};
}

