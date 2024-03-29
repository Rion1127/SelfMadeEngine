#include "GameScene.h"
#include "Easing.h"
#include "SceneManager.h"
#include "Collision.h"
#include "ParticleManager.h"
#include "Camera.h"

GameScene::~GameScene()
{

}

void GameScene::Ini()
{
	controller_ = Controller::GetInstance();
	sound_ = SoundManager::GetInstance();

	obj_ = MakeUniquePtr<Object3d>();
	obj_->SetModel(Model::CreateOBJ_uniptr("gumiShip"));

	lightManager_ = std::make_shared<LightManager>();
	Model::SetLight(lightManager_->GetLightGroup());
}

void GameScene::Update()
{
	CameraUpdate();

	obj_->Update();

	lightManager_->DebugUpdate();
	ParticleManager::GetInstance()->Update();
}

void GameScene::Draw()
{
	PipelineManager::PreDraw("Sprite", TRIANGLELIST);

	////////////////
	//3Dオブジェクト//
	////////////////
	PipelineManager::PreDraw("Object3D", TRIANGLELIST);
	obj_->Draw();

	PipelineManager::PreDraw("Toon", TRIANGLELIST);


	PipelineManager::PreDraw("assimp", TRIANGLELIST);

	////////////
	//スプライト//
	////////////
	PipelineManager::PreDraw("Sprite", TRIANGLELIST);


	PipelineManager::PreDraw("Particle", POINTLIST);
	ParticleManager::GetInstance()->Draw();
}

void GameScene::CameraUpdate()
{

	debugCamera_.Update();
	Camera::scurrent_ = debugCamera_.GetCamera();

	Camera::scurrent_->Update(CameraMode::LookAT);


}
