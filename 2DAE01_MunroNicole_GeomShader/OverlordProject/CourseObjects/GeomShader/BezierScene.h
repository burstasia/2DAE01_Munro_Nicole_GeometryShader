#pragma once
#include "Scenegraph/GameScene.h"

class BezierPrefab;

class BezierScene : public GameScene
{
public:
	BezierScene();
	virtual ~BezierScene();

protected:

	virtual void Initialize(const GameContext& gameContext);
	virtual void Update(const GameContext& gameContext);
	virtual void Draw(const GameContext& gameContext);

private:
	BezierPrefab * m_pBezier;

private:
	// -------------------------
	// Disabling default copy constructor and default 
	// assignment operator.
	// -------------------------
	BezierScene(const BezierScene &obj);
	BezierScene& operator=(const BezierScene& obj);
};

