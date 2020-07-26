#pragma once
#include <vector>
#include "Behaviors.h"

class Organism_Database
{
public:
	std::vector<std::unique_ptr<Organism_Base>> organism_vec;

	template <typename Organism_Type, typename Behavior_Tag>
	void addOrganism()
	{
		std::unique_ptr<Behavior_Interface_Base> behaviour_interface(std::make_unique<Behavior_Derived<Behavior_Tag>>());

		std::unique_ptr<Organism_Base> temp_organism(std::make_unique<Organism_Type>(std::move(behaviour_interface)));

		organism_vec.emplace_back(std::move(temp_organism));
	}

	void confrotTwo(Organism_Base& first, Organism_Base& second)
	{
		first.behavior_ptr->HandleBehavior(first, second);

		second.behavior_ptr->HandleBehavior(second, first);
	}

};