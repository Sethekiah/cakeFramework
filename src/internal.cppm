// Copyright 2025 Caleb Whitmer
module cakeFramework:internal;
import :entity;
import :time;

import std;

/**
 * @private
 * @brief      The internal state of cakeFramework
 */
struct Internal {
	// Refresh all systems; should be called once per frame
	static void refresh(void) {
		Time::refresh_();
		
		Entity::root_->update_();
		if (Time::phyiscsFlag_)
			Entity::root_->physicsUpdate_();
		Entity::root_->graphicsUpdate_();
	}

	static void end(void) {
		Entity::root_->onGameEnd_();
	}
};
