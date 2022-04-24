#pragma once
#include "Scene.h"
class SceneCharacter : public Scene
{
private:
	sf::Sprite spritePlayer;
	sf::Sprite spritePlayer2;
	sf::Sprite spriteBackground;
	sf::Sprite spriteDirection2;

public:
	SceneCharacter(SceneMgr& mgr) : Scene(mgr) {};

	// Scene��(��) ���� ��ӵ�
	virtual void Init() override;
	virtual void Release() override;
	virtual void Start() override;
	virtual void End() override;
	virtual void Update(float dt) override;
	virtual void Draw(sf::RenderWindow* window) override;
};

