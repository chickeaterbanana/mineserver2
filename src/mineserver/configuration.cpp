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
#include <inttypes.h>
#include <iostream>

#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>
#include <mineserver/world/generator.h>
#include <mineserver/world/generator/flatlands.h>

#include <mineserver/configuration.h>

Mineserver::Configuration::Configuration(int argc, char **argv) : m_worldConfig("World options"), m_configCommandline("Commandline options"), m_configFile("Configfile options")
{
	m_configCommandline.add_options()
		("help", "a general help")
		("config,c", boost::program_options::value< std::string >()->default_value("./server.cfg"), "the configuration file of the server")
	;
	
	boost::program_options::store(boost::program_options::parse_command_line(argc, argv, m_configCommandline), m_options);
	boost::program_options::notify(m_options);

	m_configFile.add_options()
		("server.port", boost::program_options::value<uint16_t>()->default_value(25565), "the Port of the Server")
		("server.world.configpath", boost::program_options::value< std::string >()->default_value("./worlds/"), "the configuration path of the worlds")
	;
	
	srand(time(NULL));
	m_worldConfig.add_options()
		("world.id", boost::program_options::value<int>()->default_value(-1), "the world id\n \t -1: create a unique id")
		("world.seed", boost::program_options::value<long>()->default_value(rand()), "the world seed")
		("world.height", boost::program_options::value<unsigned int>()->default_value(128), "the height of the world")
		("world.gamemode", boost::program_options::value<std::string>()->default_value("survival"), "the GameMode\n Options: \n \t survival \n \t creative")
		("world.dimension", boost::program_options::value<std::string>()->default_value("overworld"), "the dimension\n Options: \n \t overworld \n \t nether \n \t theend")
		("world.difficulty", boost::program_options::value<std::string>()->default_value("easy"), "the difficulty\n Options: \n \t peaceful \n \t easy \n \t normal \n \t hard")
		("world.spawn.x", boost::program_options::value<double>()->default_value(0), "the spawn point x")
		("world.spawn.y",boost::program_options::value<double>()->default_value(62), "the spawn point y")
		("world.spawn.z", boost::program_options::value<double>()->default_value(0), "the spawn point z")
		("world.levelType", boost::program_options::value<std::string>()->default_value("default"), "the leveltype\n Options: \n \t default \n \t superflat")
	;
}

void Mineserver::Configuration::parseConfig()
{	
	if (m_options.count("config"))
	{
		boost::filesystem::path configFilePath(m_options["config"].as<std::string>());
		if (!boost::filesystem::exists(configFilePath))
			std::cout << "couldn't find the server config file: " << m_options["config"].as<std::string>() << std::endl;
		else
		{
			boost::filesystem::ifstream configFile(configFilePath);
			boost::program_options::store(boost::program_options::parse_config_file(configFile, m_configFile), m_options);
			configFile.close();
		}
	}
	
	if (m_options.count("server.world.configpath"))
	{
		boost::filesystem::path worldConfigFilePath(m_options["server.world.configpath"].as<std::string>());
		if (!boost::filesystem::exists(worldConfigFilePath))
			std::cout << "couldn't find the world config path: " << m_options["server.world.configpath"].as<std::string>() << std::endl;
		else if (boost::filesystem::is_directory(worldConfigFilePath))
		{
			boost::program_options::variables_map temp_worldconfig;
			boost::filesystem::directory_iterator end_iter;
			for( boost::filesystem::directory_iterator dir_iter(worldConfigFilePath) ; dir_iter != end_iter ; ++dir_iter)
			{
				if (boost::filesystem::is_regular_file(dir_iter->status()) )
				{
					boost::filesystem::ifstream worldConfigFile(*dir_iter);
					boost::program_options::store(boost::program_options::parse_config_file(worldConfigFile, m_worldConfig), temp_worldconfig);
					worldConfigFile.close();
					if (temp_worldconfig.count("world.id"))
					{
						if (temp_worldconfig["world.id"].as<int>() < 0)
						{	
							int new_id;
							do
							{
								new_id = rand();
							}	while((temp_worldconfig["world.id"].as<int>() < 0) && (m_worldoptions.count(new_id) > 0));
							
							m_worldoptions[new_id] = temp_worldconfig;
						} else 
							m_worldoptions[temp_worldconfig["world.id"].as<int>()] = temp_worldconfig;
					}
				}
			}
		}
	}
}

bool Mineserver::Configuration::hasHelp()
{
	return m_options.count("help");
}

void Mineserver::Configuration::displayHelp()
{
	boost::program_options::options_description allowed_options("Allowed Options");
	allowed_options.add(m_configCommandline).add(m_configFile).add(m_worldConfig);
	std::cout << allowed_options << std::endl;
}
//this function maybe in the Game class
void Mineserver::Configuration::configureGame(Mineserver::Game::pointer_t game)
{
	if (m_worldoptions.empty())
	{
		game->setWorld(0, boost::make_shared<Mineserver::World>());
  	
  	game->getWorld(0)->addGenerator(boost::make_shared<Mineserver::World_Generator_Flatlands>());
	} else
	{
		Mineserver::Game::worldConfigList_t::iterator it;
		for (it = m_worldoptions.begin(); it != m_worldoptions.end(); it++ )
		{
			game->setWorld((*it).second["world.id"].as<int>(), boost::make_shared<Mineserver::World>());
			game->getWorld((*it).second["world.id"].as<int>())->setWorldSeed((*it).second["world.seed"].as<long>());
			game->getWorld((*it).second["world.id"].as<int>())->setLevelType((*it).second["world.levelType"].as<std::string>());
			game->getWorld((*it).second["world.id"].as<int>())->setGameMode(Mineserver::World::getGameModeByString((*it).second["world.gamemode"].as<std::string>()));
			game->getWorld((*it).second["world.id"].as<int>())->setDimension(Mineserver::World::getDimensionByString((*it).second["world.dimension"].as<std::string>()));
			game->getWorld((*it).second["world.id"].as<int>())->setDifficulty(Mineserver::World::getDifficultyByString((*it).second["world.difficulty"].as<std::string>()));
			game->getWorld((*it).second["world.id"].as<int>())->setWorldHeight((*it).second["world.height"].as<unsigned int>());
			game->getWorld((*it).second["world.id"].as<int>())->setSpawnPosition(WorldPosition((*it).second["world.spawn.x"].as<double>(), (*it).second["world.spawn.y"].as<double>(), (*it).second["world.spawn.z"].as<double>()));
			
  		game->getWorld((*it).second["world.id"].as<int>())->addGenerator(boost::make_shared<Mineserver::World_Generator_Flatlands>());
		}
	}
}

