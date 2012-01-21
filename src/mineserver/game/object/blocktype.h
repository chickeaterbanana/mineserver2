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

#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>

#include<mineserver/world.h>
#include<mineserver/game/player.h>

namespace Mineserver
{
	
	typedef uint8_t blocktype_t;
	
	class Game_Object_BlockType_Base;
  
  template <blocktype_t blockType>
  class Game_Object_BlockType : public Game_Object_BlockType_Base
  {
  	private:		 		
  		Game_Object_BlockType();
  		
  	public:
  		virtual ~Game_Object_BlockType() {};
  		
  		static boost::shared_ptr<Game_Object_BlockType_Base> GetBlockType()
  		{
	  			if (m_curType)
	  				return m_curType;
	  			else
	  				return boost::shared_ptr< Game_Object_BlockType<blockType> >(new Game_Object_BlockType<blockType>());
  		}
  };
	
  class Game_Object_BlockType_Base : public boost::enable_shared_from_this<Mineserver::Game_Object_BlockType_Base>
  {
  	protected:
  		blocktype_t m_curTypeIdentifier;
  		static boost::shared_ptr<Mineserver::Game_Object_BlockType_Base> m_curType;  		 		
  		Game_Object_BlockType_Base() {};
  		
  	public:
  		virtual ~Game_Object_BlockType_Base() {};
  		
  		//some useful thinks
  		virtual bool isBreakable();
  		
  		static boost::shared_ptr<Game_Object_BlockType_Base> GetBlockType(const blocktype_t Type)
  		{
  			switch (Type) {
  				case 0x07:
  					return Game_Object_BlockType<0x07>::GetBlockType();
  				default:
		  			if (m_curType)
		  				return m_curType;
		  			else
		  				return boost::shared_ptr< Game_Object_BlockType_Base >(new Game_Object_BlockType_Base());
	  		}
  		}
  };
}

#endif
