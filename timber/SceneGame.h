#pragma once
#include "Scene.h"
class SceneGame : public Scene
{
private:
	sf::Sprite spriteBackground;
	sf::Sprite spriteBee;
	sf::Sprite spriteCloud;

	sf::Sprite spritePlayer;
	sf::Sprite spritePlayer2;
public:
	SceneGame(SceneMgr& mgr) : Scene(mgr) {};

	// Scene을(를) 통해 상속됨
	virtual void Init() override;
	virtual void Release() override;
	virtual void Start() override;
	virtual void End() override;
	virtual void Update(float dt) override;
	virtual void Draw(sf::RenderWindow* window) override;
};

