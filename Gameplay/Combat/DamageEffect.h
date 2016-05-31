#pragma once
#include "SpecialMove.h"
#include "Attack.h"
#include "DamageNumber.h"

namespace jrc
{
	class DamageEffect
	{
	public:
		DamageEffect(const SpecialMove& move, const AttackUser& user, const DamageNumber& number, 
			bool toleft, int32_t damage, int32_t target_id, uint16_t delay);

		void apply(Mob& mob) const;
		bool expired() const;
		bool update();

		int32_t get_target() const;
		DamageNumber get_number() const;

	private:
		const SpecialMove& move;

		AttackUser user;
		DamageNumber number;
		int32_t damage;
		bool toleft;
		int32_t target;
		uint16_t delay;
	};
}