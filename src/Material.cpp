#include "Material.h"
#include "ResourceManager.h"

Material::Material(SimpleVertexShader* vertShader, SimplePixelShader* pixelShader)
{
	this->vertexShader = vertShader;
	this->pixelShader = pixelShader;
}


Material::~Material()
{
}

void Material::WriteShaderInfo()
{
	ResourceManager* pManager = ResourceManager::Instance();

	for (auto pair : textures)
	{
		pixelShader->SetShaderResourceView(pair.first, pManager->GetTexture(pair.second));
	}
	for (auto pair : textures)
	{
		vertexShader->SetShaderResourceView(pair.first, pManager->GetTexture(pair.second));
	}
}
