/***
* Name: task2
* Author: Mert Alp Taytak
* Description: 
* Tags: Tag1, Tag2, TagN
***/

model task2

global {
	int numOfStages <- 4;
	int numOfGuests <- 20;
	
	init {
		create species:Stage number:numOfStages;
		create species:Guest number:numOfGuests;
	}
}

species Concert {
	Stage playsIn;
	bool isOver;
	int playsFor; // Cycles until concert is over
	int vfx; // Visual effects quality
	int sfx; // Sound effects quality
	int perf; // Performance quality
	int volume; // Loudness of the concert
	int price; // Cost of attending the concert
	
	// Randomize everything
	init {
		playsIn <- nil;
		isOver <- false;
		playsFor <- rnd(125, 250);
		vfx <- rnd(100);
		sfx <- rnd(100);
		perf <- rnd(100);
		volume <- rnd(100);
		price <- rnd(100);
	}
	
	reflex beOver when: isOver {
		do die;
	}
}

species Stage skills:[fipa] {
	Concert concert;
	int timeLeft;
	
	init {
		concert <- nil;
		timeLeft <- 0;
	}
	
	reflex passageOfTime {
		timeLeft <- timeLeft - 1;
	}
	
	reflex nextAct when: (timeLeft < 0) {
		// Old concert
		if (concert != nil) {
			concert.isOver <- true;	
		}
		
		// New concert
		create species:Concert returns:tmp;
		concert <- tmp at 0;
		concert.playsIn <- self;
		
		timeLeft <- concert.playsFor;
		do announceConcert;
	}
	
	// Uses FIPA to inform guests that a new concert has began
	action announceConcert {
		do start_conversation(to :: Guest.population, performative :: 'inform', contents :: [concert]);
	}
	
	aspect standard {
		draw geometry:square(3) color:#red;
	}
}

species Guest skills:[moving, fipa] {
	Concert targetConcert;
	point targetPoint;
	float prefDuration;
	float prefVFX;
	float prefSFX;
	float prefPerf;
	float prefVolume;
	float prefPrice;
	
	init {
		targetConcert <- nil;
		targetPoint <- nil;
		prefDuration <- rnd(0.0, 10.0, 0.01);
		prefVFX <- rnd(0.0, 10.0, 0.01);
		prefSFX <- rnd(0.0, 10.0, 0.01);
		prefPerf <- rnd(0.0, 10.0, 0.01);
		prefVolume <- rnd(0.0, 10.0, 0.01);
		prefPrice<- rnd(-2.5, 0.0, 0.01); // Higher prices are undesired
	}
	
	reflex selectConcert when: !empty(informs) {
		Concert bestConcert <- targetConcert;
		float maxUtility <- utility(targetConcert);
		
		loop message over: informs {
			Concert tmp <- message.contents at 0;
			float tmpUtility <- utility(tmp);
			
			if (tmpUtility > maxUtility) {
				bestConcert <- tmp;
				maxUtility <- tmpUtility;
			}
			
			// Got information, done with the conversation
			//do end_conversation(message :: message, contents :: []);
		}
		
		if (bestConcert != nil) {
			targetPoint <- bestConcert.playsIn.location;	
		}
	}
	
	reflex goToConcert when: (targetPoint != nil) {
		do goto target: targetPoint;
	}
	
	float utility(Concert concert) {
		if (concert = nil) {
			return 0;
		}
		
		float sum <- prefDuration * concert.playsIn.timeLeft + prefVFX * concert.vfx + prefSFX * concert.sfx + prefPerf * concert.perf + prefVolume * concert.volume + prefPrice * concert.price;
		return sum;
	}
	
	aspect standard {
		draw geometry:circle(1) color:#black;
	}
}

experiment GUIExperiment type:gui {
	parameter "Number of Stages:" var: numOfStages;
	parameter "Number of Guests:" var: numOfGuests;
	
	output {
		display graphical type:opengl {
			species Stage aspect:standard;
			species Guest aspect:standard;
		}
	}
}