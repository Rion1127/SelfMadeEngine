#include <random>
#include "GameScene2.h"
#include "GameScene.h"
#include "Easing.h"
#include "SceneManager.h"
#include "Collision.h"
GameScene2::~GameScene2()
{
	delete lightGroup;
}

void GameScene2::Ini()
{
	directX = DirectXCommon::GetInstance();
	winApi_ = WinAPI::GetInstance();
	input_ = DirectXInput::GetInstance();
	controller_ = Controller::GetInstance();
	textureM = TextureManager::GetInstance();
	sound_ = SoundManager::GetInstance();

	Object3d::Ini();
	//BoardObject::Ini();
	Sprite::StaticIni();
	//デバッグカメラ初期化
	debugCamera.DebugCameraIni();

	gameCamera.SetEyePos(Vector3(0, 8, -20));
	gameCamera.SetTarget(Vector3(0, 0, 0));
	gameCamera.Update();
	//useVP = &gameCamera;
	useVP = debugCamera.GetViewProjection();
	useVP->SetOriginalPos();

	lightGroup = LightGroup::Create();
	//light->SetLightDir()

	Object3d::SetLight(lightGroup);

	skyDome_ = Object3d::CreateOBJ_uniptr("sphere", false);
	worldTransform_.Ini();
	worldTransform_.position = { -1,0,0 };

	sphere_ = Object3d::CreateOBJ_uniptr("Earth", true);
	sphereWT_.Ini();
	sphereWT_.SetPosition(fighterPos[0], fighterPos[1], fighterPos[2]);

	floor_ = Object3d::CreateOBJ_uniptr("cube", false);
	floorWT_.Ini();
	floorWT_.position = { 0,-2,0 };
	floorWT_.scale = { 5,1.f,5 };

	//平行光源
	if (lightType_ == LIGHTTYPE::DIRECTION_) {
		lightGroup->SetDirLightActive(0, true);
		lightGroup->SetDirLightActive(1, false);
		lightGroup->SetDirLightActive(2, false);
	}
	//点光源
	else if (lightType_ == LIGHTTYPE::POINT_) {
		lightGroup->SetPointLightActive(0, true);
		lightGroup->SetPointLightActive(1, false);
		lightGroup->SetPointLightActive(2, false);
		pointLightPos[0] = 0.5f;
		pointLightPos[0] = 1.0f;
		pointLightPos[0] = 0.0f;
	}
	//スポットライト
	else if (lightType_ == LIGHTTYPE::SPOT_) {
		lightGroup->SetSpotLightActive(0, true);
	}

	//影
	lightGroup->SetCircleShadowActive(0, true);



}

void GameScene2::Update()
{

	//カメラ更新
	if (input_->PushKey(DIK_LCONTROL)) {
		debugCamera.Update();
	}
	gameCamera.Update();
	cameraUpdate();

	worldTransform_.rotation.y += 0.01f;
	sphereWT_.rotation.y += 0.01f;

	worldTransform_.Update(*useVP);
	sphereWT_.Update(*useVP);
	floorWT_.Update(*useVP);

	if (input_->TriggerKey(DIK_RETURN)) {
		lightType_++;
		if (lightType_ >= LIGHTTYPE::NUMEND_)lightType_ = 0;

		//平行光源
		if (lightType_ == LIGHTTYPE::DIRECTION_) {
			lightGroup->SetDirLightActive(0, true);
			lightGroup->SetDirLightActive(1, false);
			lightGroup->SetDirLightActive(2, false);
			//スポットライト無効化
			lightGroup->SetSpotLightActive(0, false);
		}
		//点光源
		else if (lightType_ == LIGHTTYPE::POINT_) {
			lightGroup->SetPointLightActive(0, true);
			lightGroup->SetPointLightActive(1, false);
			lightGroup->SetPointLightActive(2, false);
			pointLightPos[0] = 0.5f;
			pointLightPos[0] = 1.0f;
			pointLightPos[0] = 0.0f;
			//平行光源無効化
			lightGroup->SetDirLightActive(0, false);
			lightGroup->SetDirLightActive(1, false);
			lightGroup->SetDirLightActive(2, false);
		}
		//スポットライト
		else if (lightType_ == LIGHTTYPE::SPOT_) {
			lightGroup->SetSpotLightActive(0, true);
			//点光源
			lightGroup->SetPointLightActive(0, false);
			lightGroup->SetPointLightActive(1, false);
			lightGroup->SetPointLightActive(2, false);
		}
	}

	lightGroup->Update();

	sphereWT_.SetPosition(fighterPos[0], fighterPos[1], fighterPos[2]);

	//平行光源
	if (lightType_ == LIGHTTYPE::DIRECTION_) {
		DirectionalLightUpdate();
	}
	//点光源
	else if (lightType_ == LIGHTTYPE::POINT_) {
		PointLightUpdate();
	}
	//スポットライト
	else if (lightType_ == LIGHTTYPE::SPOT_) {
		SpotLightUpdate();
	}

	lightGroup->SetAmbientColor({ ambientColor0[0],ambientColor0[1] ,ambientColor0[2] });

	lightGroup->SetCircleShadowDir(0, { circleShadowDir[0],circleShadowDir[1], circleShadowDir[2] });
	lightGroup->SetCircleShadowCasterPos(0, { fighterPos[0],fighterPos[1], fighterPos[2] });
	lightGroup->SetCircleShadowAtten(0, { circleShadowAtten[0],circleShadowAtten[1], circleShadowAtten[2] });
	lightGroup->SetCircleShadowFactorAngle(0, { circleShadowFactorAngle[0],circleShadowFactorAngle[1] });

	if (input_->TriggerKey(DIK_0)) {
		SceneManager::Transition<GameScene>();
	}
}

void GameScene2::Draw()
{
	////////////////
	//3Dオブジェクト//
	////////////////
	Object3d::PreDraw();
	Object3d::SetBlend(BLEND_NORMAL);

	skyDome_->DrawOBJ(&worldTransform_);
	sphere_->DrawOBJ(&sphereWT_);
	floor_->DrawOBJ(&floorWT_);

	///////////////////
	//板状３Dオブジェクト//
	///////////////////
	//BoardObject::PreDraw();


	////////////
	//スプライト//
	////////////
	Sprite::PreDraw();

}

void GameScene2::CheckAllCollision()
{

}

void GameScene2::cameraUpdate()
{

}

void GameScene2::DirectionalLightUpdate()
{
	lightGroup->SetAmbientColor({ ambientColor0[0],ambientColor0[1] ,ambientColor0[2] });
	//0番目の平行光源


	lightGroup->SetDirLightDir(0, { lightDir0[0],lightDir0[1], lightDir0[2] });
	lightGroup->SetDirLightColor(0, { lightColor0[0],lightColor0[1] ,lightColor0[2] });
	//1番目の平行光源
	lightGroup->SetDirLightDir(1, { lightDir1[0],lightDir1[1], lightDir1[2] });
	lightGroup->SetDirLightColor(1, { lightColor1[0],lightColor1[1] ,lightColor1[2] });
	//2番目の平行光源
	lightGroup->SetDirLightDir(2, { lightDir2[0],lightDir2[1], lightDir2[2] });
	lightGroup->SetDirLightColor(2, { lightColor2[0],lightColor2[1] ,lightColor2[2] });

	ImGui::Begin("DirectionalLight");

	//ImGui::SetWindowPos(ImVec2(0, 0));
	//ImGui::SetWindowSize(ImVec2(500, 200));

	ImGui::ColorEdit3("ambientColor", ambientColor0, ImGuiColorEditFlags_Float);

	static int num0clicked = 0;
	if (ImGui::Button("Num 0 Light"))num0clicked++;
	if (num0clicked & 1) {
		lightGroup->SetDirLightActive(0, true);
		ImGui::SameLine();
		ImGui::Text("Num 0 Light ON!");
	}
	else lightGroup->SetDirLightActive(0, false);
	ImGui::SliderFloat3("lightDir0", lightDir0, -1.f, 1.0f);
	ImGui::ColorEdit3("lightColor0", lightColor0, ImGuiColorEditFlags_Float);

	static int num1clicked = 0;
	if (ImGui::Button("Num 1 Light"))num1clicked++;
	if (num1clicked & 1) {
		lightGroup->SetDirLightActive(1, true);
		ImGui::SameLine();
		ImGui::Text("Num 1 Light ON!");
	}
	else lightGroup->SetDirLightActive(1, false);

	ImGui::SliderFloat3("lightDir1", lightDir1, -1.f, 1.0f);
	ImGui::ColorEdit3("lightColor1", lightColor1, ImGuiColorEditFlags_Float);
	static int num2clicked = 0;
	if (ImGui::Button("Num 2 Light"))num2clicked++;
	if (num2clicked & 1) {
		lightGroup->SetDirLightActive(2, true);
		ImGui::SameLine();
		ImGui::Text("Num 2 Light ON!");
	}
	else lightGroup->SetDirLightActive(2, false);
	ImGui::SliderFloat3("lightDir2", lightDir2, -1.f, 1.0f);
	ImGui::ColorEdit3("lightColor2", lightColor2, ImGuiColorEditFlags_Float);

	ImGui::End();
}

void GameScene2::PointLightUpdate()
{
	lightGroup->SetPointLightPos(0, { pointLightPos[0],pointLightPos[1], pointLightPos[2] });
	lightGroup->SetPointLightColor(0, { pointLightColor[0],pointLightColor[1], pointLightColor[2] });
	lightGroup->SetPointLightAtten(0, { pointLightAtten[0],pointLightAtten[1], pointLightAtten[2] });

	ImGui::Begin("PointLight");
	/* ここに追加したいGUIを書く */

	//ImGui::SetWindowPos(ImVec2(0, 0));
	//ImGui::SetWindowSize(ImVec2(500, 200));

	ImGui::ColorEdit3("ambientColor", ambientColor0, ImGuiColorEditFlags_Float);

	ImGui::ColorEdit3("pointLightColor", pointLightColor, ImGuiColorEditFlags_Float);
	ImGui::SliderFloat3("pointLightPos", pointLightPos, -5.f, 5.0f);
	ImGui::SliderFloat3("point LightAtten", pointLightAtten, -5.f, 5.0f);

	ImGui::InputFloat3("CircleShadowDir", circleShadowDir);
	ImGui::InputFloat3("CircleShadowAtten", circleShadowAtten);
	ImGui::InputFloat2("CircleShadowFactorAngle", circleShadowFactorAngle);
	ImGui::InputFloat3("pos", fighterPos);

	ImGui::End();
}

void GameScene2::SpotLightUpdate()
{
	lightGroup->SetSpotLightDir(0, { spotlightDir[0],spotlightDir[1], spotlightDir[2] });
	lightGroup->SetSpotLightPos(0, { spotlightPos[0],spotlightPos[1], spotlightPos[2] });
	lightGroup->SetSpotLightColor(0, { spotLightColor[0],spotLightColor[1], spotLightColor[2] });
	lightGroup->SetSpotLightAtten(0, { spotLightAtten[0],spotLightAtten[1], spotLightAtten[2] });
	lightGroup->SetSpotLightFactorAngle(0, { spotLightFactorAngle[0],spotLightFactorAngle[1] });

	ImGui::Begin("SpotLight");
	/* ここに追加したいGUIを書く */

	//ImGui::SetWindowPos(ImVec2(0, 0));
	//ImGui::SetWindowSize(ImVec2(500, 200));

	ImGui::ColorEdit3("ambientColor", ambientColor0, ImGuiColorEditFlags_Float);

	ImGui::SliderFloat3("SpotLightDir", spotlightDir, -5.f, 5.0f);
	ImGui::ColorEdit3("spotLightColor", spotLightColor, ImGuiColorEditFlags_Float);
	ImGui::SliderFloat3("SpotLightPos", spotlightPos, -5.f, 10.0f);
	ImGui::SliderFloat3("SpotLightAtten", spotLightAtten, -5.f, 5.0f);
	ImGui::SliderFloat3("SpotLightFactorAngle", spotLightFactorAngle, -100.f, 100.0f);

	ImGui::InputFloat3("CircleShadowDir", circleShadowDir);
	ImGui::InputFloat3("CircleShadowAtten", circleShadowAtten);
	ImGui::InputFloat2("CircleShadowFactorAngle", circleShadowFactorAngle);
	ImGui::InputFloat3("pos", fighterPos);

	ImGui::End();
}

