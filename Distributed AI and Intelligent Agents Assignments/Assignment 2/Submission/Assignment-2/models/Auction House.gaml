/***
* Name: AuctionHouse
* Author: Mert Alp Taytak
* Description: 
* Tags: Tag1, Tag2, TagN
***/

model AuctionHouse

global {
	int numOfParticipants <- 5;
	bool isOver <- false;
	
	init {
		create species:Initiator;
		create species:Participant number:numOfParticipants;
	}
	
	// Stop simulation when the auction is over
	reflex stop_simulation when: isOver {
		write 'Stop signal received. Pausing simulation.';
		do pause;
	}
}

species Initiator skills:[fipa] {
	
	// Instance variables
	int currPrice;
	int minPrice;
	int priceDecrement;
	
	// Constructor
	init {
		write 'Auctioneer ' + name + ' created.';
		currPrice <- 300;
		minPrice <- 100;
		priceDecrement <- 5;
	}
	
	// Start an auction
	reflex initiate_auction when: (time = 1) {
		write 'Auctioneer ' + name + ' is starting an auction with starting price ' + currPrice + ' bucks.';
		do start_conversation (to :: list(Participant.population), protocol :: 'fipa-contract-net', performative :: 'cfp', contents :: [string(currPrice)] );
	}
	
	// Repeat the auction until resolved or failed
	reflex continue_auction when: (time > 1) and !isOver {
		currPrice <- currPrice - priceDecrement;
		
		// Failure
		if (currPrice < minPrice) {
			write 'Auctioneer ' + name + ' reports that auction failed due to falling below the minimum price of ' + minPrice + ' bucks.';
			isOver <- true;
		}
		// New offer
		else {
			write 'Auctioneer ' + name + ' is continuing the auction with current price ' + currPrice + ' bucks.';
			do start_conversation (to :: list(Participant.population), protocol :: 'fipa-contract-net', performative :: 'cfp', contents :: [string(currPrice)] );
		}		
	}
	
	// Read buy decisions from bidders
	reflex read_agree_message when: !(empty(agrees)) and !isOver {
		loop a over: agrees {
			write 'Buy message with contents: ' + (string(a.contents));
		}
		isOver <- true;
	}
	
	// Read no-buy decisions from bidders
	reflex read_failure_message when: !(empty(failures)) and !isOver {
		loop f over: failures {
			write 'No-buy message with contents: ' + (string(f.contents));
		}
	}
}

species Participant skills:[fipa] {
	
	// Instance variables
	int prefPrice;
	
	// Constructor
	init {
		write 'Bidder ' + name + ' created.';
		prefPrice <- rnd(20, 180, 1);
	}
	
	// Participate in the auction
	reflex reply_messages when: !(empty(cfps)) and !isOver {
		bool doBuy <- false;
		message offer <- (cfps at 0);
		int currPrice <- int(offer.contents at 0);
		
		// Price matches preference, do buy
		if (currPrice <= prefPrice) {
			doBuy <- true;
		}
		else {
			// Randomized decision that takes preference into consideration
			// So that auctions may be more interesting
			int rndBuyFactor <- rnd(1, 100, 1);
			
			if ((currPrice - prefPrice) <= rndBuyFactor) {
				doBuy <- true;
			}
			// Else doBuy is false by default
		}
		
		if (doBuy) {
			write 'Participant ' + name + ' decides to buy at ' + currPrice + ' bucks.';
			do agree with: (message: offer, contents: [name + ', ' + currPrice]);
		}
		else {
			write 'Participant ' + name + ' decides to NOT buy at ' + currPrice + ' bucks.';
			do failure with: (message: offer, contents: [name + ', ' + currPrice]);	
		}
	}
}

experiment test type:gui {
	output {
		
	}
}