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

	m_pBezier = new BezierPrefab({0,2,0}, {5,5,0}, {10,5,0}, {20,7,0}, 0.25f, 5, 4);
	
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
