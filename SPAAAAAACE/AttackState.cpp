#include "AttackState.hpp"
#include "EnemyStateManager.hpp"

void AttackState::updateState(float timeStep) {
	look();
	attack();
}

void AttackState::toIdleState() {
	look();
	attack();
}

void AttackState::toAttackState() {
	cout << "AttackState -> AttackState transition" << endl;
}

void AttackState::toCooldownState() {
	stateManager->enemy->cooldownTimer.start();
	stateManager->toCooldownState();
}

void AttackState::look() {
	if (!checkCollision(&stateManager->enemy->getAttackRadar(), stateManager->player->getCollider()->getColliderRect(), stateManager->level->getWidth(), stateManager->level->getHeight())) {
		toCooldownState();
	}
}

void AttackState::attack() {
	stateManager->enemy->rotate(stateManager->player->getX() + Ship::SHIP_WIDTH / 2, stateManager->player->getY() + Ship::SHIP_HEIGHT / 2, *stateManager->level);
	stateManager->enemy->attack(*stateManager->player, *stateManager->level);
}

AttackState::AttackState(EnemyStateManager* sm) {
	stateManager = sm;
}
