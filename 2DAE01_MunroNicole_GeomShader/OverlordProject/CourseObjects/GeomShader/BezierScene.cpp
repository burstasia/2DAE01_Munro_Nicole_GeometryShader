#include "BezierScene.h"
#include "./Prefabs/BezierPrefab.h"


BezierScene::BezierScene():
	GameScene(L"BezierScene")
{
}


BezierScene::~BezierScene()
{
}

void BezierScene::Initialize(const GameContext & gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);

	m_pBezier = new BezierPrefab({0,2,2}, {2,5,5}, {4,3,0}, {6,1,0}, 0.75f, 5, 3, true, false);
	
	//m_pBezier->Initialize(gameContext);
//	m_pBezier->SetLeftTrack(true);
//	m_pBezier->SetRightTrack(false);
	//m_pBezier->SetNumSegments(5);
	AddChild(m_pBezier);
}

void BezierScene::Update(const GameContext & gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
}

void BezierScene::Draw(const GameContext & gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
}
