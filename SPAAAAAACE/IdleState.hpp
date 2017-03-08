#pragma once
#include"IEnemyState.hpp"

class IdleState : public IEnemyState {
	void updateState(float timeStep) override;
	void toIdleState() override;
	void toAttackState() override;
	void toCooldownState() override;
	void look();			//check for player detection
	void move(float timeStep);			//simply move the current enemy
public:
	IdleState(EnemyStateManager* sm);
};