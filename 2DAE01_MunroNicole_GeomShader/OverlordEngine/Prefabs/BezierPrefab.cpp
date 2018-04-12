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
	float radius, int segs, int circleSides):
	m_Radius(radius),
	m_CircleSides(circleSides),
	m_SegmentCount(segs)
{
	m_pBezierMatComponentLeftTrack = new MaterialComponentBezier(P0, P1, P2, P3, radius, segs, circleSides, true, false, false, false, false);
	m_pBezierMatComponentRightTrack = new MaterialComponentBezier(P0, P1, P2, P3, radius, segs, circleSides, false, true, false, false, false);
	m_pBezierMatComponentBeams = new MaterialComponentBezier(P0, P1, P2, P3, radius, segs, circleSides, false, false, true, false, false);
	m_pBezierMatComponentSupportsLeft = new MaterialComponentBezier(P0, P1, P2, P3, radius, segs, circleSides, false, false, false, true, false);
	m_pBezierMatComponentSupportsRight = new MaterialComponentBezier(P0, P1, P2, P3, radius, segs, circleSides, false, false, false, false,true);
}


BezierPrefab::~BezierPrefab()
{
}


void BezierPrefab::Initialize(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
	AddComponent(m_pBezierMatComponentLeftTrack);
	AddComponent(m_pBezierMatComponentRightTrack);
	AddComponent(m_pBezierMatComponentBeams);
	AddComponent(m_pBezierMatComponentSupportsLeft);
	AddComponent(m_pBezierMatComponentSupportsRight);
}


void BezierPrefab::Update(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);


}

void BezierPrefab::Draw(const GameContext& gameContext)
{
	
}


