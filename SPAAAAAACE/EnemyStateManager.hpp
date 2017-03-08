#pragma once
#include"IEnemyState.hpp"
#include"IdleState.hpp"
#include"AttackState.hpp"
#include"CooldownState.hpp"
#include"enemy.hpp"
#include"player.hpp"


class EnemyStateManager {
	IEnemyState* currentState;
	IdleState* idleState;
	AttackState* attackState;
	CooldownState* cooldownState;
public:
	Enemy* enemy;
	Player* player;
	Level* level;
	float originalAngle;
	EnemyStateManager();
	EnemyStateManager(Enemy* enemy, Player* player, Level* level);
	void toIdleState();
	void toAttackState();
	void toCooldownState();
	void update(float timeStep);
};