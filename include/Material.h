#ifndef MATERIAL_H
#define MATERIAL_H

// DirectX
#include "SimpleShader.h"

// STD
#include <unordered_map>

class Material
{
private:
	SimpleVertexShader* vertexShader;
	SimplePixelShader* pixelShader;
	std::unordered_map<std::string, std::string> textures;

public:
	SimpleVertexShader* GetVertexShader() { return vertexShader; }
	SimplePixelShader*  GetPixelShader() { return pixelShader; }

	Material(SimpleVertexShader* vertShader, SimplePixelShader* pixelShader);
	~Material(void);

    /*
     * Pushes any changed variables to shaders
     */
	void WriteShaderInfo(void);

	void AddTexture(std::string key, std::string textureName)
    {
		textures.emplace(key, textureName);
	}
};

#endif
