#include "stdafx.h"
#include "MaterialComponentBezier.h"
#include "Components.h"
#include "../Base/GeneralStructs.h"
#include "../Scenegraph/GameObject.h"
#include "../Scenegraph/GameScene.h"
#include "../Helpers/EffectHelper.h"
#include "../EngineMaterials/BezierMaterial.h"

ID3DX11EffectMatrixVariable* MaterialComponentBezier::m_pWorldVar = nullptr;
ID3DX11EffectMatrixVariable* MaterialComponentBezier::m_pWvpVar = nullptr;


MaterialComponentBezier::MaterialComponentBezier(XMFLOAT3 P0, XMFLOAT3 P1, XMFLOAT3 P2, XMFLOAT3 P3,
	float radius, int segs, int circleSides, bool left, bool right) :
	m_pTechnique(nullptr),
	m_pInputLayout(nullptr),
	m_pVertexBuffer(nullptr)
{
	m_pBezierMaterial = new BezierMaterial(radius, circleSides, segs, left, right);
	m_Verts.push_back({P0, P1, P2, P3});

}


MaterialComponentBezier::~MaterialComponentBezier()
{
	SafeDelete(m_pBezierMaterial);
}


void MaterialComponentBezier::LoadEffect(const GameContext & gameContext)
{
	m_pBezierMaterial->Initialize(gameContext);

	m_pTechnique = m_pBezierMaterial->m_pEffect->GetTechniqueByIndex(0);

	std::vector<ILDescription> description;
	UINT inputLayoutSize;
	UINT inLayID;

	EffectHelper::BuildInputLayout(gameContext.pDevice, m_pTechnique, &m_pInputLayout, description, inputLayoutSize, inLayID);

	m_pWorldVar = m_pBezierMaterial->m_pEffect->GetVariableBySemantic("World")->AsMatrix();
	m_pWvpVar = m_pBezierMaterial->m_pEffect->GetVariableBySemantic("WorldViewProjection")->AsMatrix();
}

void MaterialComponentBezier::InitializeBuffer(const GameContext & gameContext)
{
	if (m_pVertexBuffer)
		SafeRelease(m_pVertexBuffer);

	//*************
	//VERTEX BUFFER
	D3D11_BUFFER_DESC vertexBuffDesc;
	vertexBuffDesc.BindFlags = D3D10_BIND_FLAG::D3D10_BIND_VERTEX_BUFFER;
	vertexBuffDesc.ByteWidth = sizeof(VertexBezier) *m_Verts.size();
	vertexBuffDesc.CPUAccessFlags = D3D10_CPU_ACCESS_FLAG::D3D10_CPU_ACCESS_WRITE;
	vertexBuffDesc.Usage = D3D11_USAGE::D3D11_USAGE_DYNAMIC;
	vertexBuffDesc.MiscFlags = 0;
	gameContext.pDevice->CreateBuffer(&vertexBuffDesc, NULL, &m_pVertexBuffer);
}

void MaterialComponentBezier::UpdateBuffer()
{

}

void MaterialComponentBezier::Update(const GameContext & gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);

}

void MaterialComponentBezier::Draw(const GameContext & gameContext)
{
	//Set Shader Variables
	if (m_Verts.size() == 0)
		return;

	

	if (m_IsInitialized)
	{
		auto world = XMLoadFloat4x4(&m_pGameObject->GetTransform()->GetWorld());
		auto view = XMLoadFloat4x4(&gameContext.pCamera->GetView());
		auto projection = XMLoadFloat4x4(&gameContext.pCamera->GetProjection());

		if (m_pWorldVar)
			m_pWorldVar->SetMatrix(reinterpret_cast<float*>(&world));

		if (m_pWvpVar)
		{
			XMMATRIX wvp = world * view*projection;
			m_pWvpVar->SetMatrix(reinterpret_cast<const float*>(&(wvp)));
		}

		m_pBezierMaterial->UpdateEffectVariables(gameContext, nullptr); 
	}

	/*auto world = XMLoadFloat4x4(&GetTransform()->GetWorld());
	auto viewProjection = XMLoadFloat4x4(&gameContext.pCamera->GetViewProjection());

	m_pWorldVar->SetMatrix(reinterpret_cast<float*>(&world));
	XMMATRIX wvp = world * viewProjection;
	m_pWvpVar->SetMatrix(reinterpret_cast<float*>(&wvp));*/

	gameContext.pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
	gameContext.pDeviceContext->IASetInputLayout(m_pInputLayout);

	UINT offset = 0;
	UINT stride = sizeof(VertexBezier);
	gameContext.pDeviceContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);

	D3DX11_TECHNIQUE_DESC techDesc;
	m_pTechnique->GetDesc(&techDesc);
	for (UINT p = 0; p < techDesc.Passes; ++p)
	{
		m_pTechnique->GetPassByIndex(p)->Apply(0, gameContext.pDeviceContext);
		gameContext.pDeviceContext->Draw(m_Verts.size(), 0);
	}
}


void MaterialComponentBezier::Initialize(const GameContext & gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);

	LoadEffect(gameContext);
	InitializeBuffer(gameContext);
	UpdateBuffer();
}
