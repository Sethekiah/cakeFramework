module cakeFramework:internal;
import :entity;
import :time;

import std;

// Refresh all systems; should be called once per frame
void refresh__(void) {
	Time::refresh_();
	
	Entity::root_->update_();
	if (Time::phyiscsFlag_)
		Entity::root_->physicsUpdate_();
	Entity::root_->graphicsUpdate_();
}