#include "ResourceManager.h"

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
