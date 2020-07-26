#include "Organism_Database.h"

int main()
{
	Organism_Database database;

	database.addOrganism<Mouse, Fearless_Trait>();
	database.addOrganism<Elephant, Fearless_Of_Mice_Trait>();

	database.confrotTwo(*database.organism_vec[0], *database.organism_vec[1]);

	return 0;
}