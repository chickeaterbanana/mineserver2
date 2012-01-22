/*
  Copyright (c) 2011, The Mineserver Project
  All rights reserved.

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions are met:
  * Redistributions of source code must retain the above copyright
    notice, this list of conditions and the following disclaimer.
  * Redistributions in binary form must reproduce the above copyright
    notice, this list of conditions and the following disclaimer in the
    documentation and/or other materials provided with the distribution.
  * Neither the name of the The Mineserver Project nor the
    names of its contributors may be used to endorse or promote products
    derived from this software without specific prior written permission.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
  ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER BE LIABLE FOR ANY
  DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
  ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include <iostream>

#include <mineserver/game/object/blocktype.h>
#include <mineserver/game.h>
#include <mineserver/world.h>

bool Mineserver::BlockType::Game_Object_BlockType_Default::isBreakable()
{
	return true;
}

boost::shared_ptr<Mineserver::BlockType::Game_Object_BlockType_Base> Mineserver::BlockType::getBlockType(const Mineserver::BlockType::blocktype_t Type)
{
	static std::map<Mineserver::BlockType::blocktype_t, boost::shared_ptr<Mineserver::BlockType::Game_Object_BlockType_Base> > m_curTypes;
	if (m_curTypes.count(Type) > 0)
	{
		if (m_curTypes[Type])
			return m_curTypes[Type];
	}

	switch (Type) {
		case 0x07:
			m_curTypes[Type] = boost::shared_ptr< Mineserver::BlockType::Game_Object_BlockType_Base >(new Mineserver::BlockType::Game_Object_BlockType<0x07>()); //Bedrock
			break;
		default:
			m_curTypes[Type] = boost::shared_ptr< Mineserver::BlockType::Game_Object_BlockType_Base >(new Mineserver::BlockType::Game_Object_BlockType_Default());
			break;
	}

	return m_curTypes[Type];
}

bool Mineserver::BlockType::Game_Object_BlockType_SignalHandler::blockBreakPostWatcher(Mineserver::Game::pointer_t game, Mineserver::Game_Player::pointer_t player, Mineserver::World::pointer_t world, Mineserver::WorldBlockPosition wPosition, Mineserver::World_Chunk::pointer_t chunk, Mineserver::World_ChunkPosition cPosition)
{    
	return Mineserver::BlockType::getBlockType(chunk->getBlockType(cPosition.x, cPosition.y, cPosition.z))->blockBreakPostWatcher();
}
bool Mineserver::BlockType::Game_Object_BlockType_SignalHandler::blockBreakPreWatcher(Mineserver::Game::pointer_t game, Mineserver::Game_Player::pointer_t player, Mineserver::World::pointer_t world, Mineserver::WorldBlockPosition wPosition, Mineserver::World_Chunk::pointer_t chunk, Mineserver::World_ChunkPosition cPosition)
{
	return Mineserver::BlockType::getBlockType(chunk->getBlockType(cPosition.x, cPosition.y, cPosition.z))->blockBreakPreWatcher();
}
