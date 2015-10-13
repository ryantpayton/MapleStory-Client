/////////////////////////////////////////////////////////////////////////////
// This file is part of the Journey MMORPG client                           //
// Copyright © 2015 SYJourney                                               //
//                                                                          //
// This program is free software: you can redistribute it and/or modify     //
// it under the terms of the GNU Affero General Public License as           //
// published by the Free Software Foundation, either version 3 of the       //
// License, or (at your option) any later version.                          //
//                                                                          //
// This program is distributed in the hope that it will be useful,          //
// but WITHOUT ANY WARRANTY; without even the implied warranty of           //
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            //
// GNU Affero General Public License for more details.                      //
//                                                                          //
// You should have received a copy of the GNU Affero General Public License //
// along with this program.  If not, see <http://www.gnu.org/licenses/>.    //
//////////////////////////////////////////////////////////////////////////////
#pragma once
#include "Packetcreator.h"

namespace Net
{
	void Packetcreator::init(Session* ses)
	{
		session = ses;
	}

	void Packetcreator::c_login(string account, string pass)
	{
		OutPacket p = OutPacket(LOGIN);
		p.writestr(account);
		p.writestr(pass);
		session->dispatch(&p);
	}

	void Packetcreator::accept_tos()
	{
		OutPacket p = OutPacket(ACCEPT_TOS);
		p.writech(1);
		session->dispatch(&p);
	}

	void Packetcreator::pong()
	{
		OutPacket p = OutPacket(PONG);
		session->dispatch(&p);
	}

	void Packetcreator::serverlrequest()
	{
		OutPacket p = OutPacket(SERVER_REQUEST);
		session->dispatch(&p);
	}

	void Packetcreator::charlrequest(char world, char channel)
	{
		OutPacket p = OutPacket(CHARL_REQ);
		p.writech(0);
		p.writech(world);
		p.writech(channel);
		session->dispatch(&p);
	}

	void Packetcreator::deletechar(string pic, int cid)
	{
		OutPacket p = OutPacket(DELETE_CHAR);
		p.writestr(pic);
		p.writeint(cid);
		session->dispatch(&p);
	}

	void Packetcreator::selectchar(int cid)
	{
		OutPacket p = OutPacket(SELECT_CHAR);
		p.writeint(cid);
		p.writestr("68-5D-43-F8-B8-6C, 7A-79-19-8B-31-3F");
		p.writestr("685D43F8_B86C7A79");
		session->dispatch(&p);
	}

	void Packetcreator::registerpic(int cid, string pic)
	{
		OutPacket p = OutPacket(REGISTER_PIC);
		p.writeint(cid);
		p.writestr("68-5D-43-F8-B8-6C, 7A-79-19-8B-31-3F");
		p.writestr("685D43F8_B86C7A79");
		p.writestr(pic);
		session->dispatch(&p);
	}

	void Packetcreator::selectcharpic(int cid, string pic)
	{
		OutPacket p = OutPacket(SELECT_CHAR_PIC);
		p.writestr(pic);
		p.writeint(cid);
		p.writestr("68-5D-43-F8-B8-6C, 7A-79-19-8B-31-3F");
		p.writestr("685D43F8_B86C7A79");
		session->dispatch(&p);
	}

	void Packetcreator::playerlogin(int cid)
	{
		OutPacket p = OutPacket(PLAYER_LOGIN);
		p.writeint(cid);
		session->dispatch(&p);
	}

	void Packetcreator::checkcharname(string name)
	{
		OutPacket p = OutPacket(CHECK_CHARNAME);
		p.writestr(name);
		session->dispatch(&p);
	}

	void Packetcreator::createchar(string name, int job, int face, int hair, int hairc, int skin, int top, int bot, int shoes, int weapon, bool female)
	{
		OutPacket p = OutPacket(CREATE_CHAR);
		p.writestr(name);
		p.writeint(job);
		p.writeint(face);
		p.writeint(hair);
		p.writeint(hairc);
		p.writeint(skin);
		p.writeint(top);
		p.writeint(bot);
		p.writeint(shoes);
		p.writeint(weapon);
		p.writech(female);
		session->dispatch(&p);
	}

	/*void Packetcreator::writemoves(packet* p, vector<movefragment> moves)
	{
		p->writech(moves.size());

		for (vector<movefragment>::iterator mvit = moves.begin(); mvit != moves.end(); ++mvit)
		{
			p->writech(mvit->command);
			p->writesh(mvit->xpos);
			p->writesh(mvit->ypos);
			p->writesh(mvit->xpps);
			p->writesh(mvit->ypps);
			p->writesh(mvit->unk);
			p->writech(mvit->newstate);
			p->writesh(mvit->duration);
		}
	}*/

	/*void Packetcreator::moveplayer(vector<movefragment> moves)
	{
		OutPacket p = OutPacket(MOVE_PLAYER);
		p.writelg(0);
		p.writech(0);
		writemoves(&p, moves);
		session->dispatch(&p);
	}*/

	void Packetcreator::changemap(bool died, int targetid, string targetpname, bool usewheel)
	{
		OutPacket p = OutPacket(CHANGEMAP);
		p.writech(died);
		p.writeint(targetid);
		p.writestr(targetpname);
		p.writech(0);
		p.writesh(usewheel ? 1 : 0);
		session->dispatch(&p);
	}

	/*void Packetcreator::close_attack(attackinfo attack)
	{
		OutPacket p = OutPacket(CLOSE_ATTACK);
		p.writech(0);

		char numattdmg = 0;
		numattdmg = attack.numattacked;
		numattdmg = numattdmg << 4;
		numattdmg = numattdmg | attack.numdamage;
		p.writech(numattdmg);

		p.writeint(attack.skill);
		if (attack.charge > 0)
		{
			p.writeint(attack.charge);
		}
		p.writelg(0);
		p.writech(attack.display);
		p.writech(attack.direction);
		p.writech(attack.stance);
		p.writech(0);
		p.writech(attack.speed);
		p.writeint(0);

		for (map<int, vector<int>>::iterator attit = attack.mobsdamaged.begin(); attit != attack.mobsdamaged.end(); ++attit)
		{
			p.writeint(attit->first);
			p.writelg(0);
			p.writeint(0);
			p.writesh(0);

			for (vector<int>::iterator dmgit = attit->second.begin(); dmgit != attit->second.end(); ++dmgit)
			{
				p.writeint(*dmgit);
			}

			if (attack.skill != 5221004) {
				p.writeint(0);
			}
		}

		session->dispatch(&p);
	}*/

	void Packetcreator::talktonpc(int id)
	{
		OutPacket p = OutPacket(TALK_TO_NPC);
		p.writeint(id);
		session->dispatch(&p);
	}

	void Packetcreator::general_chat(string text, bool show)
	{
		OutPacket p = OutPacket(GENERAL_CHAT);
		p.writestr(text);
		p.writech(show ? 1 : 0);
		session->dispatch(&p);
	}

	void Packetcreator::moveitem(char type, short slot, char action, short qty)
	{
		OutPacket p = OutPacket(MOVE_ITEM);
		p.writeint(0);
		p.writech(type);
		p.writesh(slot);
		p.writesh(action);
		p.writesh(qty);
		session->dispatch(&p);
	}

	void Packetcreator::useitem(short slot, int itemid)
	{
		OutPacket p = OutPacket(USE_ITEM);
		p.writeint(0);
		p.writesh(slot);
		p.writeint(itemid);
		session->dispatch(&p);
	}

	void Packetcreator::scrollequip(short slot, short eqslot, bool usews)
	{
		OutPacket p = OutPacket(SCROLL_EQUIP);
		p.writeint(0);
		p.writesh(slot);
		p.writesh(eqslot);
		p.writesh(usews);
		session->dispatch(&p);
	}

	void Packetcreator::spendap(int stat)
	{
		OutPacket p = OutPacket(SPEND_AP);
		p.writeint(0);
		p.writeint(static_cast<int32_t>(stat));
		session->dispatch(&p);
	}

	/*void Packetcreator::movemonster(int oid, short type, byte skillb, byte skill0, byte skill1, byte skill2, byte skill3, byte skill4, vector2d startpos, vector<movefragment> movements)
	{
		OutPacket p = OutPacket(MOVE_MONSTER);
		p.writeint(oid);
		p.writesh(type);
		p.writech(skillb);
		p.writech(skill0);
		p.writech(skill1);
		p.writech(skill2);
		p.writech(skill3);
		p.writech(skill4);
		p.writelg(0);
		p.writech(0);
		p.writeint(0);
		p.writesh(startpos.x());
		p.writesh(startpos.y());
		writemoves(&p, movements);
		session->dispatch(&p);
	}*/

	void Packetcreator::pickupitem(int32_t oid, vector2d<int32_t> pos)
	{
		OutPacket p = OutPacket(PICKUP_ITEM);
		p.writeint(0);
		p.writech(0);
		p.writesh(static_cast<int16_t>(pos.x()));
		p.writesh(static_cast<int16_t>(pos.y()));
		p.writeint(oid);
		session->dispatch(&p);
	}
}