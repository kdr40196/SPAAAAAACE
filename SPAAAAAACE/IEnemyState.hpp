#pragma once

class IEnemyState {
public:
	void toIdleState();
	void toAttackState();
	void toCooldownState();
};