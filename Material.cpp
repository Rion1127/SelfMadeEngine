#include <DirectXTex.h>
using namespace DirectX;
#include "Texture.h"
#include "Material.h"

const std::string kBaseDirectory = "Resources/";

Material* Material::Create(ID3D12Device* device)
{
	Material* instance = new Material;

	instance->Ini(device);

	return instance;
}

void Material::Ini(ID3D12Device* device)
{
	HRESULT result;
	//ヒープ設定
	D3D12_HEAP_PROPERTIES cbHeapProp{};
	cbHeapProp.Type = D3D12_HEAP_TYPE_UPLOAD;
	//リソース設定
	D3D12_RESOURCE_DESC cbResourceDesc{};
	cbResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	cbResourceDesc.Width = (sizeof(ConstBufferDataMaterial) + 0xff) & ~0xff;
	cbResourceDesc.Height = 1;
	cbResourceDesc.DepthOrArraySize = 1;
	cbResourceDesc.MipLevels = 1;
	cbResourceDesc.SampleDesc.Count = 1;
	cbResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	//定数バッファの生成
	result = device->CreateCommittedResource(
		&cbHeapProp,		//ヒープ設定
		D3D12_HEAP_FLAG_NONE,
		&cbResourceDesc,	//リソース設定
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBufferMaterial));
	assert(SUCCEEDED(result));
	//定数バッファのマッピング
	result = constBufferMaterial->Map(0, nullptr, (void**)&constMapMaterial);	//マッピング
	assert(SUCCEEDED(result));
	//値を書き込むと自動的に転送される
	constMapMaterial->color = XMFLOAT4(1, 1, 1, 1);
}

void Material::ChangeColor(float x, float y, float z, float w)
{
	//値を書き込むと自動的に転送される
	constMapMaterial->color = XMFLOAT4(x,y,z,w);
}

void Material::LoadTexture(const std::string& directoryPath)
{
	// テクスチャなし
	if (textureFilename_.size() == 0) {
		textureFilename_ = "white1x1.png";
	}

	HRESULT result = S_FALSE;

	// WICテクスチャのロード
	TexMetadata metadata{};
	ScratchImage scratchImg{};

	// ファイルパスを結合
	std::string filepath = kBaseDirectory + directoryPath + textureFilename_;

	// テクスチャ読み込み
	textureHandle_ = TextureManager::GetInstance()->LoadGraph(filepath);
}

void Material::ChangeColor(XMFLOAT4 color_)
{
	//値を書き込むと自動的に転送される
	constMapMaterial->color = XMFLOAT4(color_);
}

void Material::Draw(ID3D12GraphicsCommandList* commandList, UINT descriptorSize)
{
	TextureManager::GetInstance()->SetGraphicsDescriptorTable(commandList, descriptorSize);
	//定数バッファビュー(CBV)の設定コマンド
	commandList->SetGraphicsRootConstantBufferView(0, constBufferMaterial->GetGPUVirtualAddress());
}
