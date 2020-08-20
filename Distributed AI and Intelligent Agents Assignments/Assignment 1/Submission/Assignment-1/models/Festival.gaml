/***
* Name: Festival
* Author: Mert Alp Taytak
* Description: 
* Tags: Tag1, Tag2, TagN
***/

model Festival

global {
	bool enableBoredom <- true;
	bool enableBrain <- true;
	float adventureness <- 0.5;
	int numOfInfoCenters <- 1;
	int numOfStores <- 4;
	int numOfGuests <- 10;
	
	init {
		create species:InformationCenter number:numOfInfoCenters;
		create species:Store number:numOfStores;
		create species:Guest number:numOfGuests;
	}
}

species InformationCenter {
	
	point tellNearestStore(Guest asker) {
		list<Store> stores;
		
		// Filter based on asker need, with hunger having precedence over thirst
		if (asker.isHungry()) {
			stores <- Store.population where (each.type = "FOOD");
		}
		else if (asker.isThirsty()) {
			stores <- Store.population where (each.type = "WATER");
		}
		else { /* Should not happen, give the nearest store for the sake of completeness */
			stores <- Store.population;
		}
		
		Store nearestStore <- stores closest_to(self);
		return nearestStore.location;
	}
	
	aspect standard {
		draw geometry:square(3) color:#red;
	}
}

species Store {
	
	string type <- "FOOD" among: ["FOOD", "WATER"];
	
	init {
		if (flip(0.5)) {
			type <- "WATER";
		}
	}
	
	aspect standard {
		if (type = "FOOD") {
			draw geometry:square(5) color:#green;	
		}
		else {
			draw geometry:square(5) color:#blue;
		}
	}
}

species Guest skills: [moving] {
	int hunger <- 10000;
	int thirst <- 10000;
	int fun <- 1000;
	point targetPoint <- nil;
	bool movingToInfoCenter <- false;
	bool movingToStore <- false;
	Store lastVisited <- nil;
	
	reflex beIdle when: targetPoint = nil {
		// If alone get bored
		if (enableBoredom and self distance_to(Guest.population closest_to(self)) < 3) {
			fun <- fun - 1;
		}
		
		if (enableBoredom and isBored()) {
			// Do something fun, e.g. dance, at the cost of more resources
			hunger <- hunger - 10;
			thirst <- thirst - 10;
			fun <- fun + 3;
			
			// Enforce fun upper limit
			if (fun > 1000) {
				fun <- 1000;
			}
		}
		else {
			do wander;
			hunger <- hunger - 2;
			thirst <- thirst - 4;
		}		
	}
	
	reflex moveToTarget when: targetPoint != nil {
		do goto target:targetPoint;
		hunger <- hunger - 3;
		thirst <- thirst - 5;
		
		// If alone when moving to meet a need, get even more bored
		if (enableBoredom and self distance_to(Guest.population closest_to(self)) < 3) {
			fun <- fun - 3;
		}
	}
	
	reflex enterStore when: movingToStore and location distance_to(targetPoint) < 2 {
		targetPoint <- nil;
		movingToStore <- false;
		
		// Interact with the store
		Store nearestStore <- Store.population closest_to(self);
		ask nearestStore {
			if (self.type = "FOOD") {
				myself.hunger <- 10000;
			}
			else {
				myself.thirst <- 10000;
			}
		}
		
		// Remember the store
		lastVisited <- nearestStore;
	}
	
	reflex seekInformation when: !movingToStore and isInNeed() {
		// On random skip seeking information center and go to the last visited store directly
		// if conditions are met and last visited meets Guest need
		if (enableBrain and (lastVisited != nil) and flip(1 - adventureness) and ((isHungry() and lastVisited.type = "FOOD") or (isThirsty() and lastVisited.type = "WATER"))) {
			targetPoint <- lastVisited.location;
			movingToStore <- true;
		}
		// Otherwise go to the nearest information center
		else {
			targetPoint <- (InformationCenter closest_to(self)).location;
			movingToInfoCenter <- true;
		}		
	}
	
	reflex askInformation when: movingToInfoCenter {
		// Try asking Guests nearby
		bool haveTarget <- false;
		if (enableBrain) {
			ask Guest.population where (each.lastVisited != nil) at_distance(5) {
				if (myself.isHungry() and self.lastVisited.type = "FOOD") {
					myself.targetPoint <- self.lastVisited.location;
					haveTarget <- true;
				}
				else if (myself.isThirsty() and self.lastVisited.type = "WATER") {
					myself.targetPoint <- self.lastVisited.location;
					haveTarget <- true;
				}
			}
		}		
		
		// Try asking to check if target is sufficiently close
		if (!haveTarget) {
			ask InformationCenter.population at_distance(2) {
				myself.targetPoint <- self.tellNearestStore(myself);
				haveTarget <- true;
			}
		}
		
		// Set and reset flags
		movingToStore <- true;
		movingToInfoCenter <- false;
	}
	
	bool isInNeed {
		return isHungry() or isThirsty();
	}
	
	bool isHungry {
		return hunger < 400;
	}
	
	bool isThirsty {
		return thirst < 600;
	}
	
	bool isBored {
		return fun < 100;
	}
	
	aspect standard {
		draw geometry:circle(1) color:#black;
	}
}

experiment GUIExperiment type:gui {
	parameter "Enable Boredom Mechanics?" var: enableBoredom;
	parameter "Enable Guest Memory?" var: enableBrain;
	parameter "Adventureness of Guest:" var: adventureness min: 0.0 max: 1.0;
	parameter "Number of Information Centers:" var: numOfInfoCenters;
	parameter "Number of Stores:" var: numOfStores;
	parameter "Number of Guests:" var: numOfGuests;
	
	output {
		display graphical type:opengl {
			species InformationCenter aspect:standard;
			species Store aspect:standard;
			species Guest aspect:standard;
		}
	}
}

