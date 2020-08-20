/***
* Name: task1
* Author: Mert Alp Taytak
* Description: 
* Tags: Tag1, Tag2, TagN
***/

model task1

global {
	int N min: 1 max: 99 <- 4;
	
	init {
		create species:Board with:(size: N) returns:board;
		create species:Queen number:N with:(board:board at 0) returns:listOfQueens;
		
		int counter <- 1;
		loop q over: listOfQueens {
			q.number <- counter;
			counter <- counter + 1;
		}
	}
}

// Represents the NxN board
species Board {
	int size;
	list<list<Queen>> squares;
	
	init {
		squares <- [];
		loop i from: 0 to: size {
			list<Queen> tmp <- [];
			loop j from: 0 to: size {
				tmp <- tmp + [nil];
			}
			
			squares <- squares + [tmp];
		}
	}
	
	// Actions used to check whether a particular row/column/diagonal is free
	
	
	// Actions used to inform movement, moves queen on the board
	action informMove(Queen queen, int row, int col) {
		bool flag <- false;
		
		loop i from: 0 to: size {
			loop j from: 0 to: size {
				if (squares[i][j] = queen) {
					squares[i][j] <- nil;
					flag <- true;
					break;
				}
			}
			
			if (flag) {
				break;
			}
		}
		
		squares[row][col] <- queen;
	}
	
	
	// Reflexes
	
	// Writes the queen positions on the board as a grid
	reflex writeState when: (time != 0) {
		string tmp <- 'Time: ' + time + "\n[\n";
		loop outer over: squares {
			tmp <- tmp + "  ";
			loop inner over: outer {
				if (inner = nil) {
					tmp <- tmp + '  0';
				}
				else {
					if (inner.number < 10) {
						tmp <- tmp + '  ' + inner.number;
					}
					else {
						tmp <- tmp + ' ' + inner.number;
					}
				}
			}
			tmp <- tmp + "\n";
		}
		
		write tmp + "]";
	}
}

// Represents Queens
species Queen skills:[fipa] {
	Board board;
	int number;
	Queen predecessor;	
}

// Experiments
experiment test type:gui {
	output {
		
	}
}