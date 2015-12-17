#include <ParticleSystem.h>

#include <AttackComponent.h>

ParticleSystem::ParticleSystem() {
	ResourceManager*	 rManager	   = ResourceManager::Instance();
	ID3D11Device*		 device		   = rManager->GetDevice();
	ID3D11DeviceContext* deviceContext = rManager->GetDeviceContext();

	particleVS = dynamic_cast<SimpleVertexShader*>(rManager->GetShader("ParticleVS"));
	particleGS = dynamic_cast<SimpleGeometryShader*>(rManager->GetShader("ParticleGS"));
	particlePS = dynamic_cast<SimplePixelShader*>(rManager->GetShader("ParticlePS"));

	rManager->RegisterShader<SimpleGeometryShader>("ParticleGeneratorGS", L"ParticleGeneratorGS.cso", new SimpleGeometryShader(device, deviceContext, true, false));

	// Random stuff
	unsigned randomTextureWidth = 1024;
	srand((unsigned)time(0));
	std::vector<float> data(randomTextureWidth * 4);
	for (unsigned int i = 0; i < randomTextureWidth * 4; i++) {
		data[i] = rand() / (float)RAND_MAX * 2.0f - 1.0f;
	}

	D3D11_TEXTURE1D_DESC textureDesc;
	textureDesc.ArraySize = 1;
	textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	textureDesc.MipLevels = 1;
	textureDesc.MiscFlags = 0;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.Width = 100;

	D3D11_SUBRESOURCE_DATA randData;
	randData.pSysMem = (void*)&data[0];
	randData.SysMemPitch = randomTextureWidth * sizeof(float) * 4;
	randData.SysMemSlicePitch = 0;
	device->CreateTexture1D(&textureDesc, &randData, &randomTexture);

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	srvDesc.Format = textureDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE1D;
	srvDesc.Texture1D.MipLevels = 1;
	srvDesc.Texture1D.MostDetailedMip = 0;
	device->CreateShaderResourceView(randomTexture, &srvDesc, &randomSRV);

	// Create particle blendstate
	D3D11_BLEND_DESC blendDesc;
	ZeroMemory(&blendDesc, sizeof(D3D11_BLEND_DESC));
	blendDesc.AlphaToCoverageEnable = false;
	blendDesc.IndependentBlendEnable = false;
	blendDesc.RenderTarget[0].BlendEnable = true;
	blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	D3D11_DEPTH_STENCIL_DESC depthDesc;
	ZeroMemory(&depthDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));
	depthDesc.DepthEnable = true;
	depthDesc.DepthFunc = D3D11_COMPARISON_LESS;
	depthDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;

	rManager->RegisterTexture("randomTexture", randomSRV);
	rManager->RegisterBlendState("particleBlendState", blendDesc);
}

ParticleSystem::~ParticleSystem(void) {
	randomTexture->Release();
}

void ParticleSystem::Update(EntityManager* pManager, float dt, float tt) {
	ResourceManager*		 rManager = ResourceManager::Instance();
	Camera*					 mainCamera = CameraManager::Instance()->GetActiveCamera();
	ID3D11DeviceContext*	 deviceContext = rManager->GetDeviceContext();
	ID3D11BlendState*		 particleBlendState = rManager->GetBlendState("particleBlendState");

	float factor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	UINT particleStride = sizeof(Particle);
	UINT particleOffset = 0;

	std::vector<GameEntity> entitiesWithParticleComponents = pManager->EntitiesWithComponents<ParticleComponent>();
	DirectX::XMFLOAT3 entityPosition; 

	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);

	for (auto& gE : entitiesWithParticleComponents) {
		Transform entityTransform = pManager->GetComponent<TransformComponent>(gE)->transform;

		for (ParticleGenerator* pGen : pManager->GetComponent<ParticleComponent>(gE)->GetGenerators()) {
			pGen->Update(entityTransform, dt, tt);
		}

		deviceContext->OMSetBlendState(particleBlendState, factor, 0xffffffff);

		DirectX::XMFLOAT4X4 rotation = pManager->GetComponent<TransformComponent>(pManager->EntitiesWithComponents<AttackComponent>()[0])->transform.GetWorldMatrix();
		rotation._11 = 1.0f;
		rotation._22 = 1.0f;
		rotation._33 = 1.0f;

		for (ParticleGenerator* pGen : pManager->GetComponent<ParticleComponent>(gE)->GetGenerators()) {
			particleVS->SetFloat("lifeTime", pGen->lifeTime);
			particleGS->SetMatrix4x4("world", DirectX::XMFLOAT4X4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1));
			particleGS->SetMatrix4x4("view", mainCamera->GetViewMatrix());
			particleGS->SetMatrix4x4("projection", mainCamera->GetProjectionMatrix());

			particleVS->SetShader(true);
			particleGS->SetShader(true);
			particlePS->SetShader(true);

			deviceContext->IASetVertexBuffers(0, 1, &pGen->readBuff, &particleStride, &particleOffset);
			deviceContext->DrawAuto();

			deviceContext->GSSetShader(0, 0, 0);
		}

		deviceContext->OMSetBlendState(0, factor, 0xffffffff);
	}
}