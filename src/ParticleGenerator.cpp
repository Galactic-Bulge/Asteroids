#include <ParticleGenerator.h>


ParticleGenerator::ParticleGenerator(Particle p, DirectX::XMFLOAT3 pos, float lt, float sr, float numRoots)
{
	init = false;
	lifeTime = lt;
	spawnRate = sr;
	position = pos;

	// Get necessary resources
	ResourceManager* rManager = ResourceManager::Instance();
	ID3D11Device* device = rManager->GetDevice();
	ID3D11DeviceContext* deviceContext = rManager->GetDeviceContext();

	generatorVS = dynamic_cast<SimpleVertexShader*>(rManager->GetShader("ParticleGeneratorVS"));
	generatorGS = dynamic_cast<SimpleGeometryShader*>(rManager->GetShader("ParticleGeneratorGS"));

	// Create Stream Output variables
	generatorGS->CreateCompatibleStreamOutBuffer(&readBuff, 1000000);
	generatorGS->CreateCompatibleStreamOutBuffer(&writeBuff, 1000000);

	// Create initial ROOT vertex buffer
	UINT stride = sizeof(Particle);
	UINT offset = 0;
	Particle vertices[] = { p };

	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof(Particle);
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;
	vbd.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA initialVertexData;
	initialVertexData.pSysMem = vertices;
	device->CreateBuffer(&vbd, &initialVertexData, &particleBuff);

	// Set constant variables
	generatorGS->SetFloat("dt", 0.0f);
	generatorGS->SetFloat("tt", 0.0f);
	generatorGS->SetFloat("lifeTime", lifeTime);
	generatorGS->SetFloat("spawnRate", spawnRate);
	generatorGS->SetFloat3("generatorPos", position);
	generatorGS->SetSamplerState("randomSampler", rManager->GetSamplerState("trilinear"));
	generatorGS->SetShaderResourceView("randomTexture", rManager->GetTexture("randomTexture"));

	// Activate shaders
	generatorVS->SetShader(true);
	generatorGS->SetShader(true);
	deviceContext->PSSetShader(0, 0, 0);
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);

	// Unbind vertex buffers (incase)
	ID3D11Buffer* unset = 0;
	deviceContext->IASetVertexBuffers(0, 1, &unset, &stride, &offset);

	deviceContext->IASetVertexBuffers(0, 1, &particleBuff, &stride, &offset);
	deviceContext->SOSetTargets(1, &writeBuff, &offset);
	deviceContext->Draw(1, 0);

	SimpleGeometryShader::UnbindStreamOutStage(deviceContext);
	deviceContext->GSSetShader(0, 0, 0);
	SwapBuffers();
}

ParticleGenerator::~ParticleGenerator() {
	particleBuff->Release();
	readBuff->Release();
	writeBuff->Release();
}

void ParticleGenerator::Update(Transform eTransform, float dt, float tt) {
	ResourceManager* rManager = ResourceManager::Instance();
	ID3D11DeviceContext* deviceContext = rManager->GetDeviceContext();
	UINT stride = sizeof(Particle);
	UINT offset = 0;
	generatorVS = dynamic_cast<SimpleVertexShader*>(rManager->GetShader("ParticleGeneratorVS"));
	generatorGS = dynamic_cast<SimpleGeometryShader*>(rManager->GetShader("ParticleGeneratorGS"));

	DirectX::XMMATRIX translationMat = DirectX::XMMatrixTranslation(position.x, position.y, position.z);
	DirectX::XMMATRIX rotationMat = DirectX::XMMatrixRotationRollPitchYaw(eTransform.GetRotation().x * 2, eTransform.GetRotation().y * 2, eTransform.GetRotation().z * 2);
	DirectX::XMFLOAT4X4 transformMat;
	DirectX::XMStoreFloat4x4(&transformMat, DirectX::XMMatrixTranspose(translationMat * rotationMat));
	DirectX::XMFLOAT3 finalPos = DirectX::XMFLOAT3(eTransform.GetTranslation().x + transformMat._14, eTransform.GetTranslation().y + transformMat._24, eTransform.GetTranslation().z + transformMat._34);

	// Set constant variables
	generatorGS->SetFloat("dt", dt);
	generatorGS->SetFloat("tt", tt);
	generatorGS->SetFloat3("generatorPos", finalPos);
	generatorGS->SetFloat("spawnRate", spawnRate);
	generatorGS->SetFloat("lifeTime", lifeTime);
	generatorGS->SetSamplerState("randomSampler", rManager->GetSamplerState("trilinear"));
	generatorGS->SetShaderResourceView("randomTexture", rManager->GetTexture("randomTexture"));

	// Activate shaders
	generatorVS->SetShader(true);
	generatorGS->SetShader(true);
	deviceContext->PSSetShader(0, 0, 0);

	// Unbind vertex buffers (incase)
	ID3D11Buffer* unset = 0;
	UINT unsetStride = 0;
	deviceContext->IASetVertexBuffers(0, 1, &unset, &stride, &offset);
	deviceContext->IASetVertexBuffers(0, 1, &readBuff, &stride, &offset);
	deviceContext->SOSetTargets(1, &writeBuff, &offset);
	deviceContext->DrawAuto();

	// Kill everything
	SimpleGeometryShader::UnbindStreamOutStage(deviceContext);
	deviceContext->GSSetShader(0, 0, 0);
	SwapBuffers();
}

void ParticleGenerator::SwapBuffers() {
	ID3D11Buffer* temp = readBuff;
	readBuff = writeBuff;
	writeBuff = temp;
}