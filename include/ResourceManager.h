#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

// STD
#include <string>
#include <unordered_map>
#include <utility>

// DirectX
#include <d3d11.h>
#include "Mesh.h"
#include "SimpleShader.h"
#include "Material.h"

// DirectXTK
#include <SpriteFont.h>

// Singleton
#include "Singleton.h"

// JSON
#include "json.h"

class ResourceManager : public Singleton<ResourceManager>
{
private:
    std::unordered_map<std::string, Mesh> meshes;
    std::unordered_map<std::string, ISimpleShader*> shaders;
    std::unordered_map<std::string, ID3D11ShaderResourceView*> textures;
	std::unordered_map<std::string, ID3D11SamplerState*> samplers;
	std::unordered_map<std::string, ID3D11RasterizerState*> rasterizers;
	std::unordered_map<std::string, ID3D11DepthStencilState*> depthStencils;
	std::unordered_map<std::string, ID3D11BlendState*> blendStates;
	std::unordered_map<std::string, Material*> materials;
	std::unordered_map<std::string, ID3D11RenderTargetView*> renderTargetViews;
	std::unordered_map<std::string, DirectX::SpriteFont*> spritefonts;

    ID3D11Device*           device;
    ID3D11DeviceContext*    deviceContext;
    //ID3D11RenderTargetView* renderTargetView;
    ID3D11DepthStencilView* depthStencilView;
    IDXGISwapChain*         swapChain;

	int windowWidth;
	int windowHeight;

    /*
     * JSON Parsing Methods:
     *  Responsible for registering the appropriate object based on the provided parameters.
     *  Note: JSON library will throw std::domain_error on missing key/value pairs.
     *
     *  Parameters:
     *      obj --  The JSON object to be parsed.
     *      parent -- The JSON array containing all other resources.
     */
    void ParseTexture(nlohmann::json obj, nlohmann::json parent);
    void ParseMesh(nlohmann::json obj, nlohmann::json parent);
    void ParseMaterial(nlohmann::json obj, nlohmann::json parent);
    void ParseShader(nlohmann::json obj, nlohmann::json parent);
    void ParseSpriteFont(nlohmann::json obj, nlohmann::json parent);
public:
    ResourceManager(void);
    virtual ~ResourceManager(void);

    /*
     * Attempts to parse the provided JSON file for resources.
     * @param   filename    The string indicating the JSON file to be parsed.
     * @returns A boolean indicating if this operation was successful.
     */
    bool ParseJSONFile(const std::string& filename);

    // Getters
    Mesh*                           GetMesh(const std::string& id );
	Material*						GetMaterial(const std::string& id);
    ISimpleShader*                  GetShader(const std::string& id );
    ID3D11ShaderResourceView*       GetTexture(const std::string& id );
	ID3D11SamplerState*             GetSamplerState(const std::string& id);
    ID3D11RasterizerState*          GetRasterizerState(const std::string& id);
    ID3D11DepthStencilState*        GetDepthStencilState(const std::string& id);
	ID3D11BlendState*				GetBlendState(const std::string& id);
    DirectX::SpriteFont*            GetSpriteFont(const std::string& id);
    ID3D11Device*                   GetDevice() { return this->device; }
	ID3D11DeviceContext*            GetDeviceContext() { return this->deviceContext; }
    //ID3D11RenderTargetView*         GetRenderTargetView() { return this->renderTargetView; }
	ID3D11RenderTargetView*         GetRenderTargetView(const std::string& id);
	ID3D11DepthStencilView*         GetDepthStencilView() { return this->depthStencilView; }
    IDXGISwapChain*                 GetSwapChain() { return this->swapChain; }

	int								GetWindowHeight() { return this->windowHeight; }
	int								GetWindowWidth() { return this->windowWidth; }

	// Accessors
	void							SetWindowHeight(const int& height) { this->windowHeight = height; }
	void							SetWindowWidth(const int& width) { this->windowWidth = width; }

    /*
     * Register an ID3D11Device and ID3D11DeviceContext with the ResourceManager.
     * @param   device              The ID3D11Device to register.
     * @param   deviceContext       The ID3D11DeviceContext to register.
     */
    void RegisterDeviceAndContext( ID3D11Device* const device, ID3D11DeviceContext* const deviceContext );

	/*
	* Register an ID3D11RenderTargetView with the ResourceManager.
	* @param   id				   The name/key for the Render Target View
	* @param   pRTV				   The ID3D11RenderTargetView to register.
	*/
	void RegisterRenderTargetView(const std::string& id, ID3D11RenderTargetView* pRTV);

    /*
     * Register an ID3D11DepthStencilView with the ResourceManager.
     * @param   pDepthStencil       The ID3D11DepthStencilView to register.
     */
    void RegisterDepthStencilView(ID3D11DepthStencilView* pDepthStencil);

    /*
     * Register an IDXGISwapChain
     * @param   pSwapChain          The IDXGISwapChain to register.
     */
    void RegisterSwapChain(IDXGISwapChain* pSwapChain);

    /*
     * Register a 3D Mesh with the ResourceManager
     * @param       id      The id to store the Mesh at.
     * @param       args    The arguments passed to the Mesh constructor.
     * @return  A bool indicating if the operation was successful.
     */
    template <typename... Args>
    bool RegisterMesh(const std::string& id, Args&&... args)
    {
        // Bail if there is not a registered ID3D11Device or ID3D11DeviceContext
        if( !device || !deviceContext )
        {
            return false;
        }

        // Bail is there is already a Mesh stored at that id
        if( meshes.find( id ) != meshes.cend() )
        {
            return false;
        }

        // Construct the Mesh
        meshes.emplace
        ( 
            std::piecewise_construct,
            std::forward_as_tuple( id ),
            std::forward_as_tuple( args..., device )
        );

        return true;
    }

    /*
     * Register an ISimpleShader* with the ResourceManager.
     * @param       id          The id to store the ISimpleShader* at.
     * @param       filename    The filename of the compiled shader object.
     * @return  A bool indicating if the operation was successful.
     */
    template <typename T>
    bool RegisterShader(const std::string& id, LPCWSTR filename)
    {
        static_assert( std::is_base_of<ISimpleShader, T>::value, "Must be an instance of ISimpleShader!" );

        // Bail if there is not a registered ID3D11Device or ID3D11DeviceContext
        if( !device || !deviceContext )
        {
            return false;
        }

        // Bail if there is already an ISimpleShader stored at that id
        if( shaders.find( id ) != shaders.cend() )
        {
            return false;
        }

        // Create the ISimpleShader
        {
            T* shader = new T( device, deviceContext );
            shader->LoadShaderFile( filename );

            shaders.emplace( id, shader );
        }

        return true;
    }

	/*
	* Register an ISimpleShader* with the ResourceManager.
	* @param       id          The id to store the ISimpleShader* at.
	* @param       filename    The filename of the compiled shader object.
	* @return  A bool indicating if the operation was successful.
	*/
	template <typename T>
	bool RegisterShader(const std::string& id, LPCWSTR filename, SimpleGeometryShader* gShader)
	{
		static_assert(std::is_base_of<ISimpleShader, T>::value, "Must be an instance of ISimpleShader!");

		// Bail if there is not a registered ID3D11Device or ID3D11DeviceContext
		if (!device || !deviceContext)
		{
			return false;
		}

		// Bail if there is already an ISimpleShader stored at that id
		if (shaders.find(id) != shaders.cend())
		{
			return false;
		}

		// Create the ISimpleShader
		{
			gShader->LoadShaderFile(filename);

			shaders.emplace(id, gShader);
		}

		return true;
	}

    /*
     * Register a DirectX::Spritefont with the ResourceManager.
     * @param   id          The id to store the DirectX::SpriteFont at.
     * @param   filename    The name of the file containing the SpriteFont.
     */
    bool RegisterSpriteFont(const std::string& id, const std::wstring& filename);

    /*
     * Register an ID3D11ShaderResourceView* with the ResourceManager.
     * @param       id          The id to store the ID3D11ShaderResourceView* at.
     * @param       filename    The filename of the texture.
     * @return  A bool indicating if the operation was successful.
     */
    bool RegisterTexture(const std::string& id, const std::wstring& filename );

	/*
	* Register an ID3D11ShaderResourceView* with the ResourceManager.
	* @param       id          The id to store the ID3D11ShaderResourceView* at.
	* @param       srv		   The ID3D11ShaderResourceView to register.
	* @return  A bool indicating if the operation was successful.
	*/
	bool RegisterTexture(const std::string& id, ID3D11ShaderResourceView* srv);

	/*
	* Register an ID3D11SamplerState* with the ResourceManager.
	* @param       id          The id to store the ID3D11SamplerState* at.
	* @param       filename    The description used to create the ID3D11SamplerState.
	* @return  A bool indicating if the operation was successful.
	*/
	bool RegisterSamplerState(const std::string& id, D3D11_SAMPLER_DESC samplerDesc);

    /*
     * Register an ID3D11
	 State with the ResourceManager.
     * @param       id                  The id to store the ID3D11RasterizerState* at.
     * @param       rasterizerDesc      The description used to instantiate the ID3D11RasterizerState.
     * @return A boolean indicating if the operation was successful.
     */
    bool RegisterRasterizerState(const std::string& id, D3D11_RASTERIZER_DESC rasterizerDesc);

    /*
     * Register an ID3D11DepthStencilState with the ResourceManager.
     * @param       id                  The id to store the ID3D11DepthStencilState at.
     * @param       depthStencilDesc    The description used to instantiate the ID3D11DepthStencilState.
     * @return A boolean indicating if the operation was successful.
     */
    bool RegisterDepthStencilState(const std::string& id, D3D11_DEPTH_STENCIL_DESC depthStencilDesc);

	/*
	* Register an ID3D11BlendState with the ResourceManager.
	* @param       id                  The id to store the ID3D11BlendState at.
	* @param       blendDesc			The description used to instantiate the ID3D11BlendState.
	* @return A boolean indicating if the operation was successful.
	*/
	bool RegisterBlendState(const std::string& id, D3D11_BLEND_DESC blendDesc);

	/*
	* Register a material with the ResourceManager
	* @param       id        The id to store the Material at.
	* @param       material  The material to add.
	* @return  A bool indicating if the operation was successful.
	*/
	bool RegisterMaterial(const std::string& id, Material* material)
	{
		// Bail if there is not a registered ID3D11Device or ID3D11DeviceContext
		if (!device || !deviceContext)
		{
			return false;
		}

		// Bail is there is already a Material stored at that id
		if (materials.find(id) != materials.cend())
		{
			return false;
		}

		// Add the Mesh
		materials.emplace(id, material);

		return true;
	}
};

#endif
