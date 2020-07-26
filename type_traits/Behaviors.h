#pragma once
#include <iostream>
#include "Organisms.h"

class Traits_Base {};

class Basic_Trait final : public Traits_Base {};
class Fearless_Trait final : public Traits_Base {};
class Fearless_Of_Mice_Trait final : public Traits_Base {};

class Behavior_Interface_Base
{
public:

	virtual void HandleBehavior(Organism_Base& caller, Organism_Base& other) = 0;
};

// Base behavior
template <typename Behavior_Tag, typename std::enable_if<std::is_base_of<Traits_Base, Behavior_Tag>::value, Behavior_Tag>::type* = nullptr>
class Behavior_Derived final : public Behavior_Interface_Base
{

public:
	void HandleBehavior(Organism_Base& caller, Organism_Base& other) override
	{
		if (caller.size < other.size)
			std::cout << caller.subtype_name + " flees \n";
		else
			std::cout << caller.subtype_name + " stays and fights \n";

	}
};


template <>
class Behavior_Derived <Fearless_Trait> final : public Behavior_Interface_Base
{

public:
	void HandleBehavior(Organism_Base& caller, Organism_Base& other) override
	{
		std::cout << caller.subtype_name + " stays and fights \n";
	}

};

template <>
class Behavior_Derived <Fearless_Of_Mice_Trait> final : public Behavior_Interface_Base
{

public:
	void HandleBehavior(Organism_Base& caller, Organism_Base& other) override
	{
		if (other.subtype_name == "mouse")
			std::cout << caller.subtype_name + " flees \n";
		else
		{
			Behavior_Derived<Basic_Trait> temp_behavior_obj;
			temp_behavior_obj.HandleBehavior(caller, other);
		}
	}

};