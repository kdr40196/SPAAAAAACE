#pragma once

class Enemy;
class EnemyStateManager;

class IEnemyState {
protected:
	EnemyStateManager* stateManager;
public:
	virtual void updateState(float timeStep) = 0;
	virtual void toIdleState() = 0;				//rotate ship to original angle
	virtual void toAttackState() = 0;			//if prev state == idle - save original angle, rotate ship
	virtual void toCooldownState() = 0;			//start cooldown timer
};