#include "stdafx.h"
#include "BezierPrefab.h"
#include "../Content/ContentManager.h"
#include "../Components/TransformComponent.h"
#include "../Components/CameraComponent.h"
#include "../Base/GeneralStructs.h"
#include "../Scenegraph/GameObject.h"
#include "../Scenegraph/GameScene.h"
#include "../Helpers/EffectHelper.h"

ID3DX11EffectMatrixVariable* BezierPrefab::m_pWorldVar = nullptr;
ID3DX11EffectMatrixVariable* BezierPrefab::m_pWvpVar = nullptr;

BezierPrefab::BezierPrefab(XMFLOAT3 P0, XMFLOAT3 P1, XMFLOAT3 P2, XMFLOAT3 P3):
	m_pVertexBuffer(nullptr),
	m_pEffect(nullptr),
	m_pTechnique(nullptr),
	m_pInputLayout(nullptr)
{
	m_Verts.push_back({ P0, XMFLOAT4{1,1,1,1} });
	m_Verts.push_back({ P1, XMFLOAT4{ 1,1,1,1 } });
	m_Verts.push_back({ P2, XMFLOAT4{ 1,1,1,1 } });
	m_Verts.push_back({ P3, XMFLOAT4{ 1,1,1,1 } });
}


BezierPrefab::~BezierPrefab()
{
}


void BezierPrefab::Initialize(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);

	LoadEffect(gameContext);
	InitializeBuffer(gameContext);
	UpdateBuffer();

}
void BezierPrefab::LoadEffect(const GameContext& gameContext)
{
	m_pEffect = ContentManager::Load<ID3DX11Effect>(L"./Resources/Effects/Bezier.fx");
	m_pTechnique = m_pEffect->GetTechniqueByIndex(0);
	std::vector<ILDescription> descript;
	UINT inputLayoutSize;
	UINT inLayID;

	//*****************
	//Load Input Layout (TODO: EffectUtils::CreateInputLayout(...) +> use @ Material)
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "BEZIER", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	UINT numElements = sizeof(layout) / sizeof(layout[0]);

	D3DX11_PASS_DESC PassDesc;
	m_pTechnique->GetPassByIndex(0)->GetDesc(&PassDesc);
	auto result = gameContext.pDevice->CreateInputLayout(layout, numElements, PassDesc.pIAInputSignature, PassDesc.IAInputSignatureSize, &m_pInputLayout);
	EffectHelper::BuildInputLayout(gameContext.pDevice, m_pTechnique, &m_pInputLayout, descript, inputLayoutSize,inLayID);

	Logger::LogHResult(result, L"BezierPrefab::LoadEffect(...)");

	if (!m_pWorldVar) 
		m_pWorldVar = m_pEffect->GetVariableBySemantic("World")->AsMatrix();

	if (!m_pWvpVar)
		m_pWvpVar = m_pEffect->GetVariableBySemantic("WorldViewProjection")->AsMatrix();
}

void BezierPrefab::InitializeBuffer(const GameContext& gameContext)
{
	if (m_pVertexBuffer)
		SafeRelease(m_pVertexBuffer);

	//*************
	//VERTEX BUFFER
	D3D11_BUFFER_DESC vertexBuffDesc;
	vertexBuffDesc.BindFlags = D3D10_BIND_FLAG::D3D10_BIND_VERTEX_BUFFER;
	vertexBuffDesc.ByteWidth = sizeof(VertexPosCol) *m_Verts.size();
	vertexBuffDesc.CPUAccessFlags = D3D10_CPU_ACCESS_FLAG::D3D10_CPU_ACCESS_WRITE;
	vertexBuffDesc.Usage = D3D11_USAGE::D3D11_USAGE_DYNAMIC;
	vertexBuffDesc.MiscFlags = 0;
	gameContext.pDevice->CreateBuffer(&vertexBuffDesc, NULL, &m_pVertexBuffer);
}

void BezierPrefab::UpdateBuffer()
{


	auto size = m_Verts.size();

	if (size == 0)return;
	

	D3D11_MAPPED_SUBRESOURCE mappedResource;
	GetScene()->GetGameContext().pDeviceContext->Map(m_pVertexBuffer, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &mappedResource);
	memcpy(mappedResource.pData, m_Verts.data(), sizeof(VertexPosCol) * size);
	GetScene()->GetGameContext().pDeviceContext->Unmap(m_pVertexBuffer, 0);
}

void BezierPrefab::Update(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
}

void BezierPrefab::Draw(const GameContext& gameContext)
{
	//Set Shader Variables
	if (m_Verts.size() == 0)
		return;

	auto world = XMLoadFloat4x4(&GetTransform()->GetWorld());
	auto viewProjection = XMLoadFloat4x4(&gameContext.pCamera->GetViewProjection());

	m_pWorldVar->SetMatrix(reinterpret_cast<float*>(&world));
	XMMATRIX wvp = world * viewProjection;
	m_pWvpVar->SetMatrix(reinterpret_cast<float*>(&wvp));

	gameContext.pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
	gameContext.pDeviceContext->IASetInputLayout(m_pInputLayout);

	UINT offset = 0;
	UINT stride = sizeof(VertexPosCol);
	gameContext.pDeviceContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);

	D3DX11_TECHNIQUE_DESC techDesc;
	m_pTechnique->GetDesc(&techDesc);
	for (UINT p = 0; p < techDesc.Passes; ++p)
	{
		m_pTechnique->GetPassByIndex(p)->Apply(0, gameContext.pDeviceContext);
		gameContext.pDeviceContext->Draw(m_Verts.size(), 0);
	}
}