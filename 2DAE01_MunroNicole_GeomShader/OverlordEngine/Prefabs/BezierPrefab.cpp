#include "stdafx.h"
#include "BezierPrefab.h"
#include "../Content/ContentManager.h"
#include "../Components/TransformComponent.h"
#include "../Components/CameraComponent.h"
#include "../Base/GeneralStructs.h"
#include "../Scenegraph/GameObject.h"
#include "../Scenegraph/GameScene.h"
#include "../Helpers/EffectHelper.h"
#include "../EngineMaterials/BezierMaterial.h"
#include "../Components/MaterialComponentBezier.h"

BezierPrefab::BezierPrefab(XMFLOAT3 P0, XMFLOAT3 P1, XMFLOAT3 P2, XMFLOAT3 P3, 
	float radius, int segs, int circleSides, bool left, bool right):
	m_Radius(radius),
	m_CircleSides(circleSides),
	m_SegmentCount(segs),
	m_IsLeft(left),
	m_IsRight(right)
{
	m_pBezierMatComponent = new MaterialComponentBezier(P0, P1, P2, P3, radius, segs, circleSides, left, right);
}


BezierPrefab::~BezierPrefab()
{
}


void BezierPrefab::Initialize(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
	AddComponent(m_pBezierMatComponent);

}


void BezierPrefab::Update(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);


}

void BezierPrefab::Draw(const GameContext& gameContext)
{
	
}


