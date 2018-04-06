#pragma once

#include "Graphics\Material.h"

class TextureData;

class BezierMaterial : public Material
{
public:
	BezierMaterial();
	~BezierMaterial();

protected:
	virtual void LoadEffectVariables();
	virtual void UpdateEffectVariables(const GameContext& gameContext, ModelComponent* pModelComponent);

	TextureData* m_pDiffuseTexture;
	static ID3DX11EffectShaderResourceVariable* m_pDiffuseSRVvariable;

private:
	// -------------------------
	// Disabling default copy constructor and default 
	// assignment operator.
	// -------------------------
	BezierMaterial(const BezierMaterial &obj);
	BezierMaterial& operator=(const BezierMaterial& obj);
};

