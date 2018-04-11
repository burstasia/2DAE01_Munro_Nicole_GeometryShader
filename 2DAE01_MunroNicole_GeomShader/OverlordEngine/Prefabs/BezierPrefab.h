#pragma once
#include "..\Scenegraph\GameObject.h"

class BezierPrefab: public GameObject
{
public:
	BezierPrefab(XMFLOAT3 P0, XMFLOAT3 P1, XMFLOAT3 P2, XMFLOAT3 P3);
	~BezierPrefab();

	void LoadEffect(const GameContext& gameContext);
	void InitializeBuffer(const GameContext& gameContext);
	void UpdateBuffer();
	void Update(const GameContext& gameContext);
	void Draw(const GameContext& gameContext);
	void SetNumSegments(int numSegs);
	void SetRightTrack(bool isRight);
	void SetLeftTrack(bool isLeft);

protected:

	virtual void Initialize(const GameContext& gameContext);

private:
	ID3D11Buffer * m_pVertexBuffer;

	std::vector<VertexBezier> m_Verts;
	ID3DX11Effect* m_pEffect;
	ID3DX11EffectTechnique* m_pTechnique;
	ID3D11InputLayout* m_pInputLayout;
	static ID3DX11EffectMatrixVariable *m_pWorldVar, *m_pWvpVar;

	ID3DX11EffectScalarVariable* m_pNumSegments;
	int m_NumSegments;

	ID3DX11EffectScalarVariable* m_pLeftTrack;
	bool m_LeftTrack;

	ID3DX11EffectScalarVariable* m_pRightTrack;
	bool m_RightTrack;

private:
	// -------------------------
	// Disabling default copy constructor and default 
	// assignment operator.
	// -------------------------
	BezierPrefab(const BezierPrefab& yRef);
	BezierPrefab& operator=(const BezierPrefab& yRef);

};

