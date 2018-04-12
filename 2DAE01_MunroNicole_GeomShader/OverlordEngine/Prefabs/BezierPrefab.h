#pragma once
#include "..\Scenegraph\GameObject.h"

class BezierMaterial;
class MaterialComponentBezier;

class BezierPrefab: public GameObject
{
public:
	BezierPrefab(XMFLOAT3 P0, XMFLOAT3 P1, XMFLOAT3 P2, XMFLOAT3 P3, 
		float radius, int segs, int circleSides);
	~BezierPrefab();

	
protected:
	virtual void Update(const GameContext& gameContext) override;
	virtual void Draw(const GameContext& gameContext) override;
	virtual void Initialize(const GameContext& gameContext) override;

private:

	float m_Radius;
	int m_SegmentCount;
	int m_CircleSides;
	bool m_IsLeft;
	bool m_IsRight;

	MaterialComponentBezier* m_pBezierMatComponentLeftTrack;
	MaterialComponentBezier* m_pBezierMatComponentRightTrack;
	MaterialComponentBezier* m_pBezierMatComponentBeams;
	MaterialComponentBezier* m_pBezierMatComponentSupportsLeft;
	MaterialComponentBezier* m_pBezierMatComponentSupportsRight;
private:
	// -------------------------
	// Disabling default copy constructor and default 
	// assignment operator.
	// -------------------------
	BezierPrefab(const BezierPrefab& yRef);
	BezierPrefab& operator=(const BezierPrefab& yRef);

};

