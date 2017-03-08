#pragma once
#include"IEnemyState.hpp"

class AttackState : public IEnemyState {
	void updateState(float timeStep) override;
	void toIdleState() override;
	void toAttackState() override;
	void toCooldownState() override;
	void look();			//detect player, if out of range - toCooldownState()
	void attack();
public:
	AttackState(EnemyStateManager* sm);
};