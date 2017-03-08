#include "CooldownState.hpp"
#include "EnemyStateManager.hpp"

void CooldownState::updateState(float timeStep) {
	look();
	cooldown();
}

void CooldownState::toIdleState() {
	stateManager->enemy->cooldownTimer.stop();
	stateManager->enemy->setAngle(stateManager->originalAngle);
	stateManager->toIdleState();
}

void CooldownState::toAttackState() {
	stateManager->enemy->rotate(stateManager->player->getX() + stateManager->enemy->getWidth() / 2, stateManager->player->getY() + stateManager->enemy->getHeight() / 2, *stateManager->level);
	stateManager->toAttackState();
}

void CooldownState::toCooldownState() {
	cout << "CooldownState -> CooldownState" << endl;
}

void CooldownState::cooldown() {
	stateManager->enemy->rotate(stateManager->player->getX() + Ship::SHIP_WIDTH / 2, stateManager->player->getY() + Ship::SHIP_HEIGHT / 2, *stateManager->level);
	if (stateManager->enemy->cooldownTimer.getTicks() >= stateManager->enemy->COOLDOWN_TIME)
		toIdleState();
}

void CooldownState::look() {
	if (checkCollision(&stateManager->enemy->getAttackRadar(), stateManager->player->getCollider()->getColliderRect(), stateManager->level->getWidth(), stateManager->level->getHeight())) {
		toAttackState();
	}
}

CooldownState::CooldownState(EnemyStateManager * sm) {
	stateManager = sm;
}
