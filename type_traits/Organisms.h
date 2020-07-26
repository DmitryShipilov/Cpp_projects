#pragma once
#include <memory>
#include <string>

enum class Organism_Size
{
	Small,
	Medium,
	Large
};

class Behavior_Interface_Base;

class Organism_Base
{

public:

	Organism_Size	size;
	std::string		subtype_name = "";
	std::unique_ptr<Behavior_Interface_Base> behavior_ptr;

public:

	Organism_Base(Organism_Size t_size, std::unique_ptr<Behavior_Interface_Base>&& temp_ptr) : size(t_size), behavior_ptr(std::move(temp_ptr)) {}
};

class Mouse : public Organism_Base
{

public:

	Mouse(std::unique_ptr<Behavior_Interface_Base>&& temp_ptr) : Organism_Base(Organism_Size::Small, std::move(temp_ptr)) { subtype_name = "mouse"; }
};

class Tiger : public Organism_Base
{

public:

	Tiger(std::unique_ptr<Behavior_Interface_Base>&& temp_ptr) : Organism_Base(Organism_Size::Medium, std::move(temp_ptr)) { subtype_name = "tiger"; }
};

class Elephant : public Organism_Base
{

public:

	Elephant(std::unique_ptr<Behavior_Interface_Base>&& temp_ptr) : Organism_Base(Organism_Size::Large, std::move(temp_ptr)) { subtype_name = "elephant"; }
};