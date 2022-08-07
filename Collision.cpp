#include "Vector3.h"
#include <list>
#include <memory>
#include "WorldTransform.h"
#include "Player.h"
#include "PlayerBullet.h"
#include "Enemy.h"
#include "Collision.h"

void CheckAllCollisions(Player player_, Enemy enemy_)
{
	//»èÎÛAÆBÌÀW
	Vector3 posA, posB;

	//vC[eæŸ
	const std::list<std::unique_ptr<PlayerBullet>>& playerBullets = player_.GetBullets();

	posA.x = enemy_.GetPosition().x;
	posA.y = enemy_.GetPosition().y;
	posA.z = enemy_.GetPosition().z;

	//GÆvC[eÆÌœè»è
	for (const std::unique_ptr<PlayerBullet>& bullet : playerBullets) {
		//eÌÀW
		posB.x = bullet.get()->GetPosition().x;
		posB.y = bullet.get()->GetPosition().y;
		posB.z = bullet.get()->GetPosition().z;
		//AÆBÌ£
		Vector3 ABDistance = posB - posA;
		//eÆeÌð·»è
		

	}
}

void RayCollision(WorldTransform ray, WorldTransform obj)
{
}
