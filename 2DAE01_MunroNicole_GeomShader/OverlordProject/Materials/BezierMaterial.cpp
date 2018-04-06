#include "Base\stdafx.h"


#include "BezierMaterial.h"
#include "Base/GeneralStructs.h"
#include "Diagnostics/Logger.h"
#include "Content/ContentManager.h"
#include "Graphics/TextureData.h"


ID3DX11EffectShaderResourceVariable* BezierMaterial::m_pDiffuseSRVvariable = nullptr;

BezierMaterial::BezierMaterial() : 
	Material(L"./Resources/Effects/SpikeyShader.fx"),
	m_pDiffuseTexture(nullptr)

{
}


BezierMaterial::~BezierMaterial()
{
}

void BezierMaterial::LoadEffectVariables()
{


}

void BezierMaterial::UpdateEffectVariables(const GameContext& gameContext, ModelComponent* pModelComponent)
{
	UNREFERENCED_PARAMETER(gameContext);
	UNREFERENCED_PARAMETER(pModelComponent);
}