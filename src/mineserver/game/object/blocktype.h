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

#ifndef MINESERVER_GAME_OBJECT_BLOCKTYPE_H
#define MINESERVER_GAME_OBJECT_BLOCKTYPE_H

#include <inttypes.h>
#include <map>

#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>

#include <mineserver/game/player.h>
#include <mineserver/game.h>
#include <mineserver/game/object/block.h>

namespace Mineserver
{

	namespace BlockType
	{		
	
		typedef uint8_t blocktype_t;
		
		//the base of all BlockTypes is pure virtual a Blocktype have to have this functions
		class Game_Object_BlockType_Base : public boost::enable_shared_from_this<Game_Object_BlockType_Base>
		{  	
			protected:
				blocktype_t m_curTypeIdentifier;	
				
			public:
				//slots:
				virtual bool blockBreakPostWatcher() = 0;
				virtual bool blockBreakPreWatcher()
				{
					return this->isBreakable();
				}
			
				virtual bool isPlayerOnBlock(const Game_Player &Player, const Game_Object_Block &Position) = 0;
				virtual bool isPlayerOnBlock(const Game_Player &Player, const Game_Object_Block Position) = 0;
				virtual bool isBreakable() = 0;
		};
		
		//if no special Blocktype exists this type is used
		class Game_Object_BlockType_Default : public Game_Object_BlockType_Base
		{			
			public:			
				//slots:
				virtual bool blockBreakPostWatcher()
				{
					return true;
				}
			
				virtual bool isPlayerOnBlock(const Game_Player &Player, const Game_Object_Block &Position)
				{
					return false;
				}
				
				virtual bool isPlayerOnBlock(const Game_Player &Player, const Game_Object_Block Position)
				{
					return false;
				}
				virtual bool isBreakable();
		};
  	
  	class Game_Object_BlockType_SignalHandler : public boost::enable_shared_from_this<Game_Object_BlockType_SignalHandler>
		{
			public:
				typedef boost::shared_ptr<Mineserver::BlockType::Game_Object_BlockType_SignalHandler> pointer_t;
				
				bool blockBreakPostWatcher(Mineserver::Game::pointer_t game, Mineserver::Game_Player::pointer_t player, Mineserver::World::pointer_t world, Mineserver::WorldBlockPosition wPosition, Mineserver::World_Chunk::pointer_t chunk, Mineserver::World_ChunkPosition cPosition);
				bool blockBreakPreWatcher(Mineserver::Game::pointer_t game, Mineserver::Game_Player::pointer_t player, Mineserver::World::pointer_t world, Mineserver::WorldBlockPosition wPosition, Mineserver::World_Chunk::pointer_t chunk, Mineserver::World_ChunkPosition cPosition);
		};
  	
		boost::shared_ptr<Game_Object_BlockType_Base> getBlockType(const blocktype_t Type);
	}
}

#include <mineserver/game/object/Blocks/unspecial.h>
#include <mineserver/game/object/Blocks/bedrock.h>

#endif
