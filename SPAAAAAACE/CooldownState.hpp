#pragma once
#include"IEnemyState.hpp"

class CooldownState : public IEnemyState {
	// Inherited via IEnemyState
	void updateState(float timeStep) override;
	void toIdleState() override;
	void toAttackState() override;
	void toCooldownState() override;
	void cooldown();
	void look();
public:
	CooldownState(EnemyStateManager* sm);
};