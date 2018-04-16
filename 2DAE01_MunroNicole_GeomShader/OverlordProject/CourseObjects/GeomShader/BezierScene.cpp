#include "BezierScene.h"
#include "./Prefabs/BezierPrefab.h"
#include "./Components/TerrainComponent.h"
#include "./Prefabs/SkyBoxPrefab.h"

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

	/*m_pBezier_01 = new BezierPrefab({ 0, 2, 0 }, { 2.5f, 2, 0 }, { 7.5f, 4, 0 }, { 7.5f, 5, 0 }, 0.2f, 4, 6);
	m_pBezier_02 = new BezierPrefab({ 7.5f, 5, 0 }, { 7.5f, 6, 0 }, { 12.5, 7, 0 }, { 15, 8, 0 }, 0.2f, 4, 6);
	m_pBezier_03 = new BezierPrefab({ 15, 8, 0 }, { 17.5f, 9, 0 }, { 20, 10, 0 }, { 22.5, 11, 0 }, 0.2f, 4, 6);*/

	//m_pBezier_01 = new BezierPrefab({ 0,10.0f,0 }, { 2.5f,8,2.5f }, { 5,2.5,5 }, { 7.5,2.5,7.5 }, 0.2f, 5, 4);
	//m_pBezier_02 = new BezierPrefab({ -7.5,2.5,-7.5 }, { -5,2.5,-5 }, { -2.5f,8,-2.5f }, { 0,10.0f,0 }, 0.2f, 5, 4);

	//m_pBezier_01 = new BezierPrefab({0,9.0f,0}, {2.5f,9,0}, {5,7,0}, {7.5,5,0}, 0.2f, 5, 4);
	//m_pBezier_02 = new BezierPrefab({ -7.5,5,0 }, { -5,7,0 }, { -2.5f,9,0 }, { 0,9,0 }, 0.2f, 5, 4);

	//works best when z and x is mirrored for the connecting beziers
	m_pBezier_01 = new BezierPrefab({ 0,9.0f,0 }, { 2.5f,7,2.5f }, { 5,5,5 }, { 7.5,4,9 }, 0.2f, 5, 4);
	m_pBezier_02 = new BezierPrefab({ -7.5,10,-11 }, { -5,12,-5 }, { -2.5f,10.5,-2.5 }, { 0,9,0 }, 0.2f, 5, 4);
	//m_pBezier_03 = new BezierPrefab({ 15,9,0 }, { 17.5,11,0 }, { 20,12,0 }, { 22.5,10,0 }, 0.2f, 10, 4);
	AddChild(m_pBezier_01);
	AddChild(m_pBezier_02);
	//AddChild(m_pBezier_03);

	//terrain
	auto terrainGameObject = new GameObject();
	terrainGameObject->AddComponent(new TerrainComponent(L"./Resources/Terrain/Hawai_HeightMap_64x64x16.raw", L"./Resources/Terrain/Hawai_TexMap.dds", 64, 64, 64.0f, 64.0f, 10.0f));

	AddChild(terrainGameObject);

	//skyBox
	auto skybox = new SkyBoxPrefab();
	AddChild(skybox);
}

void BezierScene::Update(const GameContext & gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
}

void BezierScene::Draw(const GameContext & gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
}
