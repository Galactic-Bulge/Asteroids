#include "ResourceManager.h"

// STD
#include <iostream>
#include <fstream>
#include <stdexcept>

// DirectX
#include "DXMacros.h"
#include "WICTextureLoader.h"
#include "DDSTextureLoader.h"

ResourceManager::ResourceManager(void) {}

ResourceManager::~ResourceManager(void)
{
    // Meshes are stack-allocated and automatically cleaned up.

    // Cleanup Shaders
    for( auto& pair : shaders )
    {
        delete pair.second;
    }

    // Release Textures
	for ( auto& pair : textures)
    {
        ReleaseMacro( pair.second );
	}

	// Release SamplerStates
	for (auto& pair : samplers)
	{
		ReleaseMacro(pair.second);
	}

    // Release RasterizerStates.
    for(auto& pair : rasterizers)
    {
        ReleaseMacro(pair.second);
    }

    // Release DepthStencilStates.
    for(auto& pair : depthStencils)
    {
        ReleaseMacro(pair.second);
    }

	// Release Materials
	for (auto& pair : materials)
	{
		delete pair.second;
	}
}

/*
 * Register an ID3D11Device and ID3D11DeviceContext with the ResourceManager.
 * @param   device              The ID3D11Device to register.
 * @param   deviceContext       The ID3D11DeviceContext to register.
 */
void ResourceManager::RegisterDeviceAndContext( ID3D11Device* const device, ID3D11DeviceContext* const deviceContext )
{
    this->device = device;
    this->deviceContext = deviceContext;
}

/*
 * Register an ID3D11RenderView and ID3D11DepthStencilView with the ResourceManager.
 * @param   pRender             The ID3D11RenderTargetView to register.
 * @param   pDepthStencil       The ID3D11DepthStencilView to register.
 */
void ResourceManager::RegisterRenderTargetAndDepthStencilView(ID3D11RenderTargetView* pRender, ID3D11DepthStencilView* pDepthStencil)
{
    this->renderTargetView = pRender;
    this->depthStencilView = pDepthStencil;
}

/*
 * Register an IDXGISwapChain
 * @param   pSwapChain          The IDXGISwapChain to register.
 */
void ResourceManager::RegisterSwapChain(IDXGISwapChain* pSwapChain)
{
    this->swapChain = pSwapChain;
}

#pragma region Getters

Mesh* ResourceManager::GetMesh( const std::string& id )
{
    auto& iter = meshes.find( id );
    if( iter == meshes.cend() )
    {
        return nullptr;
    }

    return &iter->second;
}

ISimpleShader* ResourceManager::GetShader( const std::string& id )
{
    auto& iter = shaders.find( id );
    if( iter == shaders.cend() )
    {
        return nullptr;
    }

    return iter->second;
}

ID3D11ShaderResourceView* ResourceManager::GetTexture( const std::string& id )
{
    auto& iter = textures.find( id );
    if( iter == textures.cend() )
    {
        return nullptr;
    }

    return iter->second;
}

ID3D11SamplerState* ResourceManager::GetSamplerState( const std::string& id )
{
    auto& iter = samplers.find( id );
    if( iter == samplers.cend() )
    {
        return nullptr;
    }

    return iter->second;
}

ID3D11RasterizerState*      ResourceManager::GetRasterizerState(const std::string& id)
{
    auto& iter = rasterizers.find(id);
    if(iter == rasterizers.cend())
    {
        return nullptr;
    }
    return iter->second;
}

ID3D11DepthStencilState*    ResourceManager::GetDepthStencilState(const std::string& id)
{
    auto& iter = depthStencils.find(id);
    if(iter == depthStencils.cend())
    {
        return nullptr;
    }
    return iter->second;
}

Material* ResourceManager::GetMaterial(const std::string& id)
{
	auto& iter = materials.find(id);
	if (iter == materials.cend())
	{
		return nullptr;
	}

	return iter->second;
}

#pragma endregion

/*
 * Register an ID3D11ShaderResourceView* with the ResourceManager.
 * @param       id          The id to store the ID3D11ShaderResourceView* at.
 * @param       filename    The filename of the texture.
 * @return  A bool indicating if the operation was successful.
 */
bool ResourceManager::RegisterTexture( const std::string& id, const std::wstring& filename )
{
    // Bail if there is not a registered ID3D11Device or ID3D11DeviceContext
    if( !device || !deviceContext )
    {
        return false;
    }

    // Bail if there is already an ID3D11ShaderResourceView* stored at that id
    if( textures.find( id ) != textures.cend() )
    {
        return false;
    }

    ID3D11ShaderResourceView* pTexture;

    // Check if we are loading a DDS texture
    std::size_t index = filename.find(L".dds");
    if(index == std::string::npos)
    {
        DirectX::CreateWICTextureFromFile(device, deviceContext, filename.c_str(), 0, &pTexture);
    }
    else
    {
        DirectX::CreateDDSTextureFromFile(device, deviceContext, filename.c_str(), 0, &pTexture);
    }
    textures.emplace( id, pTexture );

    return true;
}


/*
 * Register an ID3D11SamplerState* with the ResourceManager.
 * @param       id          The id to store the ID3D11SamplerState* at.
 * @param       filename    The description used to create the ID3D11SamplerState.
 * @return  A bool indicating if the operation was successful.
 */
bool ResourceManager::RegisterSamplerState( const std::string& id, D3D11_SAMPLER_DESC pSamplerDesc )
{
    // Bail if there is not a registered ID3D11Device or ID3D11DeviceContext
    if( !device || !deviceContext )
    {
        return false;
    }

    // Bail if there is already an ID3D11SamplerState* stored at that id
    if( samplers.find( id ) != samplers.cend() )
    {
        return false;
    }

    // Create the ID3D11SamplerState*
    {
        ID3D11SamplerState* pSamplerState;
        device->CreateSamplerState( &pSamplerDesc, &pSamplerState );
        samplers.emplace( id, pSamplerState );
    }

    return true;
}

/*
 * Register an ID3D11RasterizerState with the ResourceManager.
 * @param       id                  The id to store the ID3D11RasterizerState* at.
 * @param       rasterizerDesc      The description used to instantiate the ID3D11RasterizerState.
 * @return A boolean indicating if the operation was successful.
 */
bool ResourceManager::RegisterRasterizerState(const std::string& id, D3D11_RASTERIZER_DESC rasterizerDesc)
{
    // Bail if there is not a registered ID3D11Device or ID3D11DeviceContext
    if(!device || !deviceContext)
    {
        return false;
    }

    // Bail if there is already an ID3D11RasterizerState* stored at that id
    if(rasterizers.find(id) != rasterizers.cend())
    {
        return false;
    }

    // Create the ID3D11RasterizerState*
    ID3D11RasterizerState* pRasterizerState;
    HR(device->CreateRasterizerState(&rasterizerDesc, &pRasterizerState));
    rasterizers.emplace(id, pRasterizerState);

    return true;
}

/*
 * Register an ID3D11DepthStencilState with the ResourceManager.
 * @param       id                  The id to store the ID3D11DepthStencilState at.
 * @param       depthStencilDesc    The description used to instantiate the ID3D11DepthStencilState.
 * @return A boolean indicating if the operation was successful.
 */
bool ResourceManager::RegisterDepthStencilState(const std::string& id, D3D11_DEPTH_STENCIL_DESC depthStencilDesc)
{
    // Bail if there is not a registered ID3D11Device or ID3D11DeviceContext
    if(!device || !deviceContext)
    {
        return false;
    }

    // Bail if there is already an ID3D11DepthStencilState* stored at that id
    if(depthStencils.find(id) != depthStencils.cend())
    {
        return false;
    }

    // Create the ID3D11DepthStencilState*
	ID3D11DepthStencilState* pDepthStencilState;
    HR(device->CreateDepthStencilState(&depthStencilDesc, &pDepthStencilState));
    depthStencils.emplace(id, pDepthStencilState);

    return true;
}

#pragma region JSON Methods

/*
 * JSON Parsing Methods:
 *  Responsible for registering the appropriate object based on the provided parameters.
 *  Note: JSON library will throw std::domain_error on missing key/value pairs.
 *
 *  Parameters:
 *      obj --  The JSON object to be parsed.
 *      parent -- The JSON array containing all other resources.
 */

using json = nlohmann::json;

void ResourceManager::ParseTexture(json obj, json parent)
{
    std::string id = obj["ID"];
    std::string filename = obj["Filename"];

    RegisterTexture(id, std::wstring(filename.begin(), filename.end()).c_str());
}

void ResourceManager::ParseMesh(json obj, json parent)
{
    std::string id = obj["ID"];
    std::string filename = obj["Filename"];

    RegisterMesh(id, filename);
}

void ResourceManager::ParseShader(json obj, json parent)
{
    std::string id = obj["ID"];
    std::string filename = obj["Filename"];
    std::string stage = obj["Stage"];

    // Register the appropriate type of shader.
    if(stage == "Vertex")
    {
        RegisterShader<SimpleVertexShader>(id, std::wstring(filename.begin(), filename.end()).c_str());
    }
    else if(stage == "Pixel")
    {
        RegisterShader<SimplePixelShader>(id, std::wstring(filename.begin(), filename.end()).c_str());
    }
}

void ResourceManager::ParseMaterial(json obj, json parent)
{
    std::string id = obj["ID"];
    std::string vShader = obj["VertexShader"];
    std::string pShader = obj["PixelShader"];

    // Attempt to locate the Vertex Shader.
    SimpleVertexShader* pVertex = static_cast<SimpleVertexShader*>(GetShader(vShader));
    if(!pVertex)
    {
        ParseShader(parent[vShader], parent);
        pVertex = static_cast<SimpleVertexShader*>(GetShader(vShader));

        if(!pVertex)
        {
            throw std::domain_error("Missing Vertex Shader!");
        }
    }

    // Attempt to locate the Pixel Shader.
    SimplePixelShader* pPixel = static_cast<SimplePixelShader*>(GetShader(pShader));
    if(!pPixel)
    {
        ParseShader(parent[pShader], parent);
        pPixel = static_cast<SimplePixelShader*>(GetShader(pShader));

        if(!pPixel)
        {
            throw std::domain_error("Missing Pixel Shader!");
        }
    }

    // Create the Material.
    Material* pMaterial = new Material(pVertex, pPixel);

    // Add Textures.
    auto arr = obj["Textures"];
    for(auto& texture : arr)
    {
        std::string key = texture["Key"];
        std::string textureName = texture["TextureName"];

        ID3D11ShaderResourceView* pTex = GetTexture(textureName);
        if(!pTex)
        {
            ParseTexture(parent[textureName], parent);
            pTex = GetTexture(textureName);

            if(!pTex)
            {
                delete pMaterial; // Cleanup the Material.
                throw std::domain_error("Missing Texture!");
            }
        }

        pMaterial->AddTexture(key, textureName);
    }

    // Finally register the Material.
    RegisterMaterial(id, pMaterial);
}

/*
 * Attempts to parse the provided JSON file for resources.
 * @param   filename    The string indicating the JSON file to be parsed.
 * @returns A boolean indicating if this operation was successful.
 */
bool ResourceManager::ParseJSONFile(const std::string& filename)
{
    std::ifstream filestream(filename);
    if(filestream.is_open())
    {
        json jsonObj;

        // Parse the JSON file.
        try
        {
            filestream >> jsonObj;
        }
        // JSON library will throw std::invalid_argument on improperly formatted JSON.
        catch(std::invalid_argument e)
        {
            std::cerr << filename << " " << e.what() << std::endl;
            return false;
        }

        // Iterate through the objects in the JSON array.
        for(auto& member : jsonObj)
        {
            try
            {
                // Attempt to discern the resource's type.
                std::string type = member["Type"];
                if(type == "Mesh")
                {
                    ParseMesh(member, jsonObj);
                }
                else if(type == "Texture")
                {
                    ParseTexture(member, jsonObj);
                }
                else if(type == "Shader")
                {
                    ParseShader(member, jsonObj);
                }
                else if(type == "Material")
                {
                    ParseMaterial(member, jsonObj);
                }
            }
            // JSON library will throw std::domain_error on missing values.
            catch(std::domain_error e)
            {
                std::cerr << "Error Parsing " << member << ": " << e.what() << std::endl;
                continue;
            }
        }

        return true; // We have finished parsing the file.
    }

    return false; // We failed to open th file.
}

#pragma endregion
