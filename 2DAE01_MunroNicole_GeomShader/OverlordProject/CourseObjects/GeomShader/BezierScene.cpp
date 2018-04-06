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

	m_pBezier = new BezierPrefab({0,10,0}, {2,10,0}, {4,10,0}, {6,10,0});

	//m_pBezier->Initialize(gameContext);

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
