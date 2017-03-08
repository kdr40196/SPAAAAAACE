#include "EnemyStateManager.hpp"

EnemyStateManager::EnemyStateManager() {
	idleState = new IdleState(this);
	attackState = new AttackState(this);
	cooldownState = new CooldownState(this);
	currentState = idleState;
}

EnemyStateManager::EnemyStateManager(Enemy* enemy, Player* player, Level* level) : EnemyStateManager() {
	this->enemy = enemy;
	this->player = player;
	this->level = level;
}

void EnemyStateManager::toIdleState() {
	currentState = idleState;
}

void EnemyStateManager::toAttackState() {
	currentState = attackState;
}

void EnemyStateManager::toCooldownState() {
	currentState = cooldownState;
}


void EnemyStateManager::update(float timeStep) {
	currentState->updateState(timeStep);
}
