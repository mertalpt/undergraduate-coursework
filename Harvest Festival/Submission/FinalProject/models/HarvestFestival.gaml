/***
* Name: Hotel
* Author: Mert Alp Taytak
* Description: Simplistic model of a harvest festival
***/

model HarvestFestival

global {
	/* Statistical data */
	int thievingAttempts <- 0;
	int thievingSuccesses <- 0;
	int thievingFailures <- 0;	
	
	/* Model parameters */
	int numOfVendors <- 5;
	int numOfPerformers <- 5;
	int numOfAttendees <- 20;
	int numOfThieves <- 10;
	int numOfGuards <- 10;
	int bazaarX <- 30;
	int bazaarY <- 30;
	int bazaarW <- 20;
	int bazaarH <- 20;
	int fieldX <- 70;
	int fieldY <- 70;
	int fieldW <- 20;
	int fieldH <- 20;
	
	/* Rule parameters */
	int advertisementRadius <- 50;
	int performanceRadius <- 25;
	int thiefInformRadius <- 20;
	int thiefCatchRange <- 5;
	int nearbyDefinition <- 15;
	float thiefStickinessLimit <- 0.5;
	float guardStickinessLimit <- 0.5;
	bool vendorSeekGuard <- true;
	bool vendorAvoidVendor <- true;
	bool performerAvoidPerformer <- true;
	bool thiefAvoidGuard <- true;
	
	/* Initializer */
	init {
		create species: Vendor number: numOfVendors;
		create species: Performer number: numOfPerformers;
		create species: Attendee number: numOfAttendees;
		create species: Thief number: numOfThieves;
		create species: Guard number: numOfGuards;
		create species: Bazaar number: 1 returns: bazaars;
		create species: Field number: 1 returns: fields;
		
		/* Setting parameters */
		Bazaar bazaar <- bazaars at 0;
		Field field <- fields at 0;
		
		bazaar.location <- {bazaarX, bazaarY};
		bazaar.width <- bazaarW;
		bazaar.heigth <- bazaarH;
		field.location <- {fieldX, fieldY};
		field.width <- fieldW;
		field.heigth <- fieldH;
	}
}

/* AGENTS */
/* Sells products in the bazaar */
species Vendor skills: [moving, fipa] {
	/* Traits */
	float advertisementFrequency min: 0.0 max: 1.0; // How much attention does a vendor gather?
	float competitionAversion min: 0.0 max: 1.0; // How likely is a vendor to stay away from another vendor?
	float safeness min: 0.0 max: 1.0; // How likely is a vendor to seek a guard?
	
	/* Variables */
	point targetLocation;
	bool isTargeting;
	
	/* Initializer */
	init {
		advertisementFrequency <- rnd(0.0, 1.0);
		competitionAversion <- rnd(0.0, 1.0);
		safeness <- rnd(0.0, 1.0);
		
		targetLocation <- nil;
		isTargeting <- false;
	}
	
	/* Reflexes */
	reflex moving {
		if (isTargeting) {
			do goto target: targetLocation;
		
			if (self distance_to(targetLocation) < 1) {
				isTargeting <- false;
			}
		}
		else {
			do wander;
		}
	}
	
	// Staying inside bazaar borders is the highest priority
	reflex seekBazaar when: !inBazaar(self.location) {
		if (!isTargeting) {
			targetLocation <- (Bazaar.population at 0).location;
			isTargeting <- true;
			
			// Spread the target over nine points instead of just the center
			point selfLoc <- self.location;
			
			// Approaching from left
			if (selfLoc.x - targetLocation.x < -(bazaarW/2)) {
				targetLocation <- {targetLocation.x - bazaarW/4, targetLocation.y};
			}
			// Approaching from right
			else if (selfLoc.x - targetLocation.x > (bazaarW/2)) {
				targetLocation <- {targetLocation.x + bazaarW/4, targetLocation.y};
			}
			// Approaching from up
			if (selfLoc.y - targetLocation.y < -(bazaarH/2)) {
				targetLocation <- {targetLocation.x, targetLocation.y - bazaarH/4};
			}
			// Approaching from down
			else // Approaching from up
			if (selfLoc.y - targetLocation.y > (bazaarH/2)) {
				targetLocation <- {targetLocation.x, targetLocation.y + bazaarH/4};
			}
		}
	}
	
	// Gather attention to your products
	reflex advertiseProducts when: flip(advertisementFrequency) {
		list<Attendee> attendees <- Attendee.population where (each distance_to(self) < advertisementRadius);
		list<Thief> thieves <- Thief.population where (each distance_to(self) < advertisementRadius);
		list<Guard> guards <- Guard.population where (each distance_to(self) < advertisementRadius);
		
		// In reality nobody can tell the difference between a thief and an attendee, we can in the model
		if (!empty(attendees)) {
			do start_conversation(to :: attendees, performative :: 'inform', contents :: [self, "VENDOR"]);
		}
		if (!empty(thieves)) {
			do start_conversation(to :: thieves, performative :: 'inform', contents :: [self, "VENDOR"]);
		}
		if (!empty(guards)) {
			do start_conversation(to :: guards, performative :: 'inform', contents :: [self, "VENDOR"]);
		}
	}
	
	// Move closer to guards
	reflex seekGuards when: vendorSeekGuard and !isTargeting and flip(safeness) {
		// Find the nearest guard
		Guard nearest <- Guard.population closest_to(self);
		float distance <- nearest distance_to(self);
		
		if (distance != 0) {
			point target <- {self.location.x - (self.location.x - nearest.location.x) / distance, self.location.y - (self.location.y - nearest.location.y) / distance};
			targetLocation <- target;
			isTargeting <- true;
		}	
	}
	
	// Move away from competitors
	reflex avoidCompetitors when: vendorAvoidVendor and !isTargeting and flip(competitionAversion) {
		// Find the nearest competitor
		Vendor nearest <- Vendor.population closest_to(self);
		float distance <- nearest distance_to(self);
		
		if (distance != 0) {
			point target <- {self.location.x + (self.location.x - nearest.location.x) / distance, self.location.y + (self.location.y - nearest.location.y) / distance};
			targetLocation <- target;
			isTargeting <- true;
		}		
	}
	
	/* Functions */
	bool inBazaar(point pt) {
		point selfLoc <- pt;
		point bazLoc <- (Bazaar.population at 0).location;
		
		if ((abs(selfLoc.x - bazLoc.x) > bazaarW/2) or (abs(selfLoc.y - bazLoc.y) > bazaarH/2)) {
			return false;
		}
		else {
			return true;
		}
	}
	
	/* Visual */
	aspect standard {
		draw geometry:circle(1) color:#red;
	}
}

/* Performs entertaining acts in the festival field */
species Performer skills: [moving, fipa] {
	/* Traits */
	float loudness min: 0.0 max: 1.0; // How much attention does a performer gather?
	float competitionAversion min: 0.0 max: 1.0; // How likely is a performer to stay away from another performer?
	float audiencePreference min: 0.0 max: 1.0; // How big of an audience does a performer prefer?
	
	/* Variables */
	point targetLocation;
	bool isTargeting;
	bool isPerforming;
	
	/* Initializer */
	init {
		loudness <- rnd(0.0, 1.0);
		competitionAversion <- rnd(0.0, 1.0);
		audiencePreference <- rnd(0.0, 1.0);
		
		targetLocation <- nil;
		isTargeting <- false;
		isPerforming <- false;
	}
	
	/* Reflexes */
	reflex moving when: !isPerforming {
		if (isTargeting) {
			do goto target: targetLocation;
		
			if (self distance_to(targetLocation) < 1) {
				isTargeting <- false;
			}
		}
		else {
			do wander;
		}
	}
	
	// Staying inside field borders is the highest priority
	reflex seekField when: !inField(self.location) {
		if (!isTargeting) {
			targetLocation <- (Field.population at 0).location;
			isTargeting <- true;
			
			// Spread the target over nine points instead of just the center
			point selfLoc <- self.location;
			
			// Approaching from left
			if (selfLoc.x - targetLocation.x < -(fieldW/2)) {
				targetLocation <- {targetLocation.x - fieldW/4, targetLocation.y};
			}
			// Approaching from right
			else if (selfLoc.x - targetLocation.x > (fieldW/2)) {
				targetLocation <- {targetLocation.x + fieldW/4, targetLocation.y};
			}
			// Approaching from up
			if (selfLoc.y - targetLocation.y < -(fieldH/2)) {
				targetLocation <- {targetLocation.x, targetLocation.y - fieldH/4};
			}
			// Approaching from down
			else // Approaching from up
			if (selfLoc.y - targetLocation.y > (fieldH/2)) {
				targetLocation <- {targetLocation.x, targetLocation.y + fieldH/4};
			}
		}
	}
	
	// Gather attention to your products
	reflex advertisePerformance when: flip(loudness) {
		list<Attendee> attendees <- Attendee.population where (each distance_to(self) < advertisementRadius);
		list<Thief> thieves <- Thief.population where (each distance_to(self) < advertisementRadius);
		list<Guard> guards <- Guard.population where (each distance_to(self) < advertisementRadius);
		
		// In reality nobody can tell the difference between a thief and an attendee, we can in the model
		if (!empty(attendees)) {
			do start_conversation(to :: attendees, performative :: 'inform', contents :: [self, "PERFORMER"]);
		}
		if (!empty(thieves)) {
			do start_conversation(to :: thieves, performative :: 'inform', contents :: [self, "PERFORMER"]);
		}
		if (!empty(guards)) {
			do start_conversation(to :: guards, performative :: 'inform', contents :: [self, "PERFORMER"]);
		}
	}
	
	// Move away from competitors
	reflex avoidCompetitors when: performerAvoidPerformer and !isPerforming and !isTargeting and flip(competitionAversion) {
		// Find the nearest competitor
		Performer nearest <- Performer.population closest_to(self);
		float distance <- nearest distance_to(self);
		
		if (distance != 0) {
			point target <- {self.location.x + (self.location.x - nearest.location.x) / distance, self.location.y + (self.location.y - nearest.location.y) / distance};
			targetLocation <- target;
			isTargeting <- true;
		}		
	}
	
	// Decide to perform or not when not moving towards a target
	reflex decideToPerform when: !isTargeting {
		// Check if there is sufficient audience
		int limit <- floor((numOfAttendees + numOfThieves) * audiencePreference);
		
		// Cannot tell apart attendees and thieves, guards do not pay, vendors do not come to the field
		list<Attendee> nearbyAttendees <- Attendee.population where (each distance_to(self) < performanceRadius);
		list<Thief> nearbyThieves <- Thief.population where (each distance_to(self) < performanceRadius);
		
		int count <- length(nearbyAttendees) + length(nearbyThieves);
		
		if (limit <= count) {
			isPerforming <- true;
		}
		else {
			isPerforming <- false;
		}
	}
	
	/* Functions */
	bool inField(point pt) {
		point selfLoc <- pt;
		point fieldLoc <- (Field.population at 0).location;
		
		if ((abs(selfLoc.x - fieldLoc.x) > fieldW/2) or (abs(selfLoc.y - fieldLoc.y) > fieldH/2)) {
			return false;
		}
		else {
			return true;
		}
	}
	
	/* Visual */
	aspect standard {
		draw geometry:circle(1) color:#green;
	}
}

/* Buys products and watches performances */
species Attendee skills: [moving, fipa] {
	/* Traits */
	float activityPreference min: 0.0 max: 1.0; // What is the activity preference of an attendee between shopping (0.0) and spectating (1.0)?
	float activityStickiness min: 0.75 max: 1.0; // How likely is an attendee to stick to the current activity? Higher floor for more interesting model.
	float safeness min: 0.0 max: 1.0; // How likely is an attendee to seek the presence of guards?
	
	/* Variables */
	point targetLocation;
	bool isTargeting;
	string targetType;
	Vendor vendorTarget;
	Performer performerTarget;
	
	/* Initializer */
	init {
		activityPreference <- rnd(0.0, 1.0);
		activityStickiness <- rnd(0.75, 1.0);
		safeness <- rnd(0.0, 1.0);
		
		targetLocation <- nil;
		isTargeting <- false;
		targetType <- nil;
		vendorTarget <- nil;
		performerTarget <- nil;
	}
	
	/* Reflexes */
	reflex moving {
		if (isTargeting) {
			// Track moving targets better
			if (targetType != nil) {
				if (targetType = "VENDOR") {
					targetLocation <- vendorTarget.location;
				}
				else if (targetType = "PERFORMER") {
					targetLocation <- performerTarget.location;
				}
			}
			do goto target: targetLocation;
		
			if (self distance_to(targetLocation) < 5) {
				targetType <- nil;
				isTargeting <- false;
			}
		}
		else {
			do wander;
		}
	}
	
	// Inform guards after being robbed
	reflex informGuards when: !empty(informs) {
		loop message over: informs {
			string desc <- message.contents at 1;
			if (desc = 'THIEF') {
				list<Guard> nearbyGuards <- Guard.population where (each distance_to(self) < thiefInformRadius);
				
				// There might be no guards close enough
				if (!empty(nearbyGuards)) {
					do start_conversation(to :: nearbyGuards, performative :: 'inform', contents :: message.contents);
				}				
			}
		}
	}
	
	// Choose next activity
	reflex chooseActivity when: !isTargeting and !flip(activityStickiness) and !empty(informs) {
		// Prefers spectating performances
		if (flip(activityPreference)) {
			list<message> messages <- informs where ((each.contents at 1) = 'PERFORMER');
			list<Performer> performers <- [];
			loop message over: messages {
				performers <- performers + [message.contents at 0];
			}
			
			// Choose closest
			Performer performer <- performers closest_to(self);
			
			// There may be no performances to attend at all
			if (performer != nil) {
				targetType <- "PERFORMER";
				performerTarget <- performer;
				targetLocation <- performer.location;
				isTargeting <- true;
			}			
		}
		// Prefers shopping
		else {
			list<message> messages <- informs where ((each.contents at 1) = 'VENDOR');
			list<Vendor> vendors <- [];
			loop message over: messages {
				vendors <- vendors + [message.contents at 0];
			}
			
			// Choose closest
			Vendor vendor <- vendors closest_to(self);
			
			// There may be no vendors to go to
			if (vendor != nil) {
				targetType <- "VENDOR";
				vendorTarget <- vendor;
				targetLocation <- vendor.location;
				isTargeting <- true;
			}			
		}
		
		// Close conversations
		loop message over: informs {
			do end_conversation(message, []);
		}
	}
	
	// Seek nearby guards
	reflex seekGuard when: !isTargeting and flip(safeness) {
		// Find the nearest guard
		Guard nearest <- Guard.population closest_to(self);
		float distance <- nearest distance_to(self);
		
		if (distance != 0) {
			point target <- {self.location.x - (self.location.x - nearest.location.x) / distance, self.location.y - (self.location.y - nearest.location.y) / distance};
			targetLocation <- target;
			isTargeting <- true;
		}	
	}
	
	/* Visual */
	aspect standard {
		draw geometry:circle(1) color:#gray;
	}
}

/* Steals money from attendees */
species Thief skills: [moving, fipa] {
	/* Traits */
	float stickiness min: thiefStickinessLimit max: 1.0; // How long a thief sticks to an activity before moving on?
	float boldness min: 0.0 max: 1.0; // How risky does a thief act?
	float guardAvoidance min: 0.0 max: 1.0; // How likely is a thief to avoid guards?	
	
	/* Variables */
	Attendee mark;
	point targetLocation;
	bool isTargeting;
	bool isThieving;
	bool isRunning;
	
	/* Initializer */
	init {
		boldness <- rnd(0.0, 1.0);
		guardAvoidance <- rnd(0.0, 1.0);
		stickiness <- rnd(thiefStickinessLimit, 1.0);
		
		mark <- nil;
		targetLocation <- nil;
		isTargeting <- false;
		isThieving <- false;
		isRunning <- false;
	}
	
	/* Reflexes */
	reflex moving {
		if (isTargeting) {
			do goto target: targetLocation;
		
			if (self distance_to(targetLocation) < 1) {
				isTargeting <- false;
				
				// Successful escape
				if (isRunning) {
					isRunning <- false;
					thievingSuccesses <- thievingSuccesses + 1;
					do start_conversation(to :: Guard.population, performative :: 'inform', contents :: [self, 'ESCAPED']);
				}				
			}
		}
		else {
			do wander;
		}
	}
	
	// Check if caught by guards before successful escape
	reflex checkIfCaught when: isRunning and !empty(informs) {
		loop message over: informs {
			if (message.contents at 1 = "CAUGHT") {
				mark <- nil;
				targetLocation <- nil;
				isTargeting <- false;
				isThieving <- false;
				isRunning <- false;
				
				thievingFailures <- thievingFailures + 1;
				do end_conversation(message, []);
			}
		}
	}
	
	// Choose activity to find the next mark
	reflex chooseActivity when: !isTargeting and !flip(stickiness) and !empty(informs) {
		list<message> messages <- informs;
		list<agent> _agents <- [];
		loop message over: messages {
			_agents <- _agents + [message.contents at 0];
		}
		
		// Choose closest
		agent target <- _agents closest_to(self);
		
		// There may be no target to go to
		if (target != nil) {
			targetLocation <- target.location;
			isTargeting <- true;
		}
		
		// Close conversations
		loop message over: informs {
			do end_conversation(message, []);
		}
	}
	
	// Decide whether to choose a mark and if decided to do so, choose one
	reflex chooseMark when: !isRunning {
		list<Attendee> nearbyAttendees <- Attendee.population where (each distance_to(self) < nearbyDefinition);
		list<Guard> nearbyGuards <- Guard.population where (each distance_to(self) < nearbyDefinition);
		
		float riskTaking <- boldness^(length(nearbyGuards));
		
		if (flip(riskTaking)) {
			Attendee nearest <- nearbyAttendees closest_to(self);
			mark <- nearest;
			
			if (mark != nil) {
				isThieving <- true;
				targetLocation <- mark.location;
			}
			else {
				isThieving <- false;
			}
		}
	}
	
	// Stalk the mark
	reflex stalkMark when: isThieving {
		targetLocation <- mark.location;
		
		// Check whether close enough to steal
		if (mark distance_to(self) < 2) {
			thievingAttempts <- thievingAttempts + 1;
			do start_conversation(to :: [mark], performative :: 'inform', contents :: [self, 'THIEF']);
			isThieving <- false;
			isRunning <- true;
			targetLocation <- closestBorder();
		}
	}
	
	// Avoid nearby guards
	reflex avoidGuard when: thiefAvoidGuard and !isTargeting and flip(guardAvoidance) {
		// Find the nearest guard
		Guard nearest <- Guard.population closest_to(self);
		float distance <- nearest distance_to(self);
		
		if (distance != 0) {
			point target <- {self.location.x + (self.location.x - nearest.location.x) / distance, self.location.y + (self.location.y - nearest.location.y) / distance};
			targetLocation <- target;
			isTargeting <- true;
		}	
	}
	
	/* Functions */
	point closestBorder {
		point pos <- self.location;
		float northDist <- pos.y;
		float eastDist <- 100 - pos.x;
		float southDist <- 100 - pos.y;
		float westDist <- pos.x;
		float closest <- min([northDist, eastDist, southDist, westDist]);
		
		if (northDist = closest) {
			return {pos.x, 0.0};
		}
		else if (eastDist = closest) {
			return {100.0, pos.y};
		}
		else if (southDist = closest) {
			return {pos.x, 100.0};
		}
		else {
			return {0.0, pos.y};
		}
	}
	
	/* Visual */
	aspect standard {
		draw geometry:circle(1) color:#black;
	}
}

/* Looks out for and catches thieves */
species Guard skills: [moving, fipa] {
	/* Traits */
	float stickiness min: guardStickinessLimit max: 1.0; // How long a guard sticks to a place before moving on?
	float collaboration min: 0.0 max: 1.0; // How likely is a guard to call for help?
	float spread min: 0.0 max: 1.0; // How likely it is for a guard to spread out over a larger area?
	
	/* Variables */
	Thief mark;
	point targetLocation;
	bool isTargeting;
	bool isChasing;
	
	/* Initializer */
	init {
		stickiness <- rnd(guardStickinessLimit, 1.0);
		collaboration <- rnd(0.0, 1.0);
		
		mark <- nil;
		targetLocation <- nil;
		isTargeting <- false;
		isChasing <- false;
	}
	
	/* Reflexes */
	reflex moving {
		if (isTargeting) {
			do goto target: targetLocation;
		
			if (self distance_to(targetLocation) < 1) {
				isTargeting <- false;
			}
		}
		else {
			do wander;
		}
	}
	
	// Choose activity to find the next mark
	reflex chooseActivity when: !isTargeting and !flip(stickiness) and !empty(informs) {
		list<message> messages <- informs;
		list<agent> _agents <- [];
		list<Thief> thieves <- [];
		loop message over: messages {
			string desc <- message.contents at 1;
			if (desc = "VENDOR" or desc = "PERFORMER") {
				_agents <- _agents + [message.contents at 0];
			}
			else if (desc = "THIEF") {
				thieves <- thieves + [message.contents at 0];
			}
		}
		
		if (!empty(thieves)) {
			Thief closest <- thieves closest_to(self);
			mark <- closest;
			isChasing <- true;
			targetLocation <- mark.location;
			
			// Chance to collaborate
			if (flip(collaboration)) {
				list<Guard> nearbyGuards <- Guard.population where (each distance_to(self) > 0 and each distance_to(self) < thiefInformRadius);
				
				if (!empty(nearbyGuards)) {
					do start_conversation(to :: nearbyGuards, performative :: 'inform', contents :: [mark, 'THIEF']);
				}
			}
		}
		else {
			// Choose closest
			agent target <- _agents closest_to(self);
			
			// There may be no target to go to
			if (target != nil) {
				targetLocation <- target.location;
				isTargeting <- true;
			}
		}
		
		// Close conversations
		loop message over: informs {
				do end_conversation(message, []);
		}
	}
	
	// Chase down the marked thief
	reflex chaseThief when: isChasing {			
		// Check if something happened with the thief
		loop message over: informs {
			string desc <- message.contents at 1;
			
			if (desc = "ESCAPED" or desc = "CAUGHT") {
				if (mark = message.contents at 0) {
					mark <- nil;
					targetLocation <- nil;
					isTargeting <- false;
					isChasing <- false;
					
					do end_conversation(message, []);
				}
			}
		}
		
		// If thief hasn't been caught or escaped yet
		if (mark != nil) {
			targetLocation <- mark.location;
		
			if (self distance_to(targetLocation) < thiefCatchRange) {
				do start_conversation(to :: [mark], performative :: 'inform', contents :: [mark, 'CAUGHT']);
				do start_conversation(to :: Guard.population, performative :: 'inform', contents :: [mark, 'CAUGHT']);
				mark <- nil;
				targetLocation <- nil;
				isTargeting <- false;
				isChasing <- false;
			}	
		}
	}
	
	// Move away from nearby guards
	reflex spreadOut when: !isTargeting and flip(spread) {
		// Find the nearest guard
		Guard nearest <- Guard.population closest_to(self);
		float distance <- nearest distance_to(self);
		
		if (distance != 0) {
			point target <- {self.location.x + (self.location.x - nearest.location.x) / distance, self.location.y + (self.location.y - nearest.location.y) / distance};
			targetLocation <- target;
			isTargeting <- true;
		}	
	}
	
	/* Visual */
	aspect standard {
		draw geometry:circle(1) color:#blue;
	}
}

/* PLACES */
/* A place for the vendors to set up shop */
species Bazaar {
	/* Attributes */
	int width;
	int heigth;
	
	/* Visual */
	aspect standard {
		draw geometry: rectangle(width, heigth) color: #brown;
	}
}

/* A place for performers to perform and attendees to enjoy themselves */
species Field {
	/* Attributes */
	int width;
	int heigth;
	
	/* Visual */
	aspect standard {
		draw geometry: rectangle(width, heigth) color: #lightgreen;
	}
}

experiment HarvestFestival type: gui {
	/* Inputs */
	parameter "Number of Vendors: " var: numOfVendors;
	parameter "Number of Performers: " var: numOfPerformers;
	parameter "Number of Attendees: " var: numOfAttendees;
	parameter "Number of Thieves: " var: numOfThieves;
	parameter "Number of Guards: " var: numOfGuards;
	parameter "Bazaar X-Position: " var: bazaarX;
	parameter "Bazaar Y-Position: " var: bazaarY;
	parameter "Bazaar Width: " var: bazaarW;
	parameter "Bazaar Heigth: " var: bazaarH;
	parameter "Field X-Position: " var: fieldX;
	parameter "Field Y-Position: " var: fieldY;
	parameter "Field Width: " var: fieldW;
	parameter "Field Heigth: "var: fieldH;
	parameter "Vendor Advertisement Radius: " var: advertisementRadius;
	parameter "Performer Performance Radius: " var: performanceRadius;
	parameter "Thief Inform Radius: " var: thiefInformRadius;
	parameter "Thief Catch Range: " var: thiefCatchRange;
	parameter "Limit of Being Nearby in Units: " var: nearbyDefinition;
	parameter "Thief Stickiness Chance Lower Limit: " var: thiefStickinessLimit;
	parameter "Guard Stickiness Chance Lower Limit: " var: guardStickinessLimit;
	parameter "Vendors Move Closer to Guards: " var: vendorSeekGuard;
	parameter "Vendors Avoid Other Vendors: " var: vendorAvoidVendor;
	parameter "Performers Avoid Other Performers: " var: performerAvoidPerformer;
	parameter "Thieves Avoid Guards: "  var: thiefAvoidGuard;
	
	/* Outputs */
	output {
		display Graphical type: opengl {
			species Bazaar aspect: standard;
			species Field aspect: standard;
			species Vendor aspect: standard;
			species Performer aspect: standard;
			species Attendee aspect: standard;
			species Thief aspect: standard;
			species Guard aspect: standard;			
		}
		
		display TimeData type: opengl {
			chart "Thievery Statistics" type: series {
				data "Attempts" value: thievingAttempts color: #black;
				data "Successes" value: thievingSuccesses color: #red;
				data "Failures" value: thievingFailures color: #blue;
			}
		}
		
		display FinalData type: opengl {
			chart "Ratios" type: pie {
				data "Successes" value: thievingSuccesses color: #red;
				data "Failures" value: thievingFailures color: #blue;
			}
		}
	}
}
