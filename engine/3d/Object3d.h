#pragma once
#include "REngine.h"
#include "LightGroup.h"
#include "Model.h"

using namespace Microsoft::WRL;
class Object3d
{
private:
	std::unique_ptr<Model> model_ = nullptr;

	Vector3 pos_;
	Vector3 rot_;
	Vector3 scale_;

	WorldTransform WT_;
	//表示フラグ true 表示しない, false 表示する
	bool isVisble_ = false;

public:
	Object3d();
	~Object3d();
	//初期化
	void Init();
	//更新
	void Update();
	//描画
	void Draw();

public:
	//セッター
	void SetModel(std::unique_ptr<Model> model) { model_ = std::move(model); }
	void SetPos(const Vector3& pos) { pos_ = pos; }
	void SetScale(const Vector3& scale) { scale_ = scale; }
	void SetRot(const Vector3& rot) { rot_ = rot; }
	void SetIsVisible(bool flag) { isVisble_ = flag; }

public:
	//ゲッター
	WorldTransform GetTransform() { return WT_; }
	Vector3 GetPos() { return pos_; }
	Vector3 GetRot() { return rot_; }
	Vector3 GetScale() { return scale_; }
	Model* GetModel() { return model_.get(); }
	bool GetIsVisible() { return isVisble_; }
};

