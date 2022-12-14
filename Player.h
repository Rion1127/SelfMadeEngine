#pragma once
#include "PlayerBullet.h"
#include <list>
#include <memory>
#include <d3d12.h>
#include "ViewProjection.h"
#include "DirectXInput.h"
#include "Object3d.h"
class Player
{
public:
	Player();
	~Player();

	void Ini(ID3D12Device* device);

	void Update(ID3D12Device* device,ViewProjection viewProjection);
	//当たり判定を検出したら呼び出される関数
	void OnCollision();

	void Draw(uint32_t graph);

	const std::list<std::unique_ptr<PlayerBullet>>& GetBullets() { return bullets; }
private:
	void Move();

	void Shot(ID3D12Device* device, ViewProjection viewProjection);

	WorldTransform worldTransform;
	Object3d model_{};

	Controller* cInput;

	std::list<std::unique_ptr<PlayerBullet>> bullets;



	//弾を打ち出す間隔
	const float maxCoolTime = 10;
	float cooltime = maxCoolTime;

};

