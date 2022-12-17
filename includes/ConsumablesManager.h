#pragma once
#include "Consumables.h"

// Class that will perform various action on the consumable class, managing his resource and logic
class ConsumablesManager {

	public:
		// Attributes
		std::vector<Healthbox> healthboxes;
		std::vector<Ammobox> ammoboxes;
		std::vector<std::unique_ptr<Consumable>> Templates;
		int curHealthID;
		int curAmmoID;

		// Constructor
		ConsumablesManager() {
			this->curHealthID = -1;
			this->curAmmoID = -1;
			this->templateLoaded = false;
		}

		// Destructor 
		~ConsumablesManager() {}

		// Functions 
		void spawnHealthbox(Vector3f, float, float, int);
		void spawnAmmobox(Vector3f, float, float, int);
		void deleteHealthbox(int);
		void deleteAmmobox(int);
		void assignTemplates(void);
		void reset(void);

	private:

		// Attributes
		bool templateLoaded;
};