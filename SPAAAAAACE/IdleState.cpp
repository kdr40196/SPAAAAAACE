#include "IdleState.hpp"
#include "EnemyStateManager.hpp"

void IdleState::updateState(float timeStep) {
	look();
	move(timeStep);
}

void IdleState::toIdleState() {
	cout << "IdleState -> IdleState transition." << endl;
}

void IdleState::toAttackState() {
	stateManager->originalAngle = stateManager->enemy->getAngle();
	stateManager->enemy->rotate(stateManager->player->getX() + stateManager->enemy->getWidth() / 2, stateManager->player->getY() + stateManager->enemy->getHeight() / 2, *stateManager->level);
	stateManager->toAttackState();
}

void IdleState::toCooldownState() {
	cout << "IdleState -> CooldownState transition." << endl;
}

void IdleState::look() {
	
	if (checkCollision(&stateManager->enemy->getAttackRadar(), stateManager->player->getCollider()->getColliderRect(), stateManager->level->getWidth(), stateManager->level->getHeight())) {
		toAttackState();
	}
}

void IdleState::move(float timeStep) {
	stateManager->enemy->move(timeStep, *stateManager->level, *stateManager->player);
}

IdleState::IdleState(EnemyStateManager * sm) {
	stateManager = sm;
}
