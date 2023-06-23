#include "StdAfx.h"
#include "Board.h"
#include "Minimax.h"
#include "GraphicsManager.h"


//Things to do:


Movetree::Movetree(Board* fakeboard, int nextdepth, bool whichplayer, int newwidth, stopWatch *checkifdone, int timelimit) //always use a duplicate
{

	storedboard = fakeboard;
	depthtoadd = nextdepth;
	getmax = whichplayer;
	timesup = false;
	width = newwidth;
	timer = checkifdone;
	timeleft = timelimit;
}


Movetree::~Movetree()
{
	delete(storedboard);
}

//explain later

void Movetree::UpdateMovetree(Board* backup)
{
	depthtoadd = 0;
	storedboard = backup;
//	returnafterfree = new Movetree(backup, 0, getmax, width, timer, timeleft); //make a new Movetree set to the current gamestate
//	delete(this);
	
}


int Movetree::getWidth()
{
	return width;
}


int Movetree::getDepthToAdd()
{
	return depthtoadd;
}



Move* Movetree::Minimax()
{
	bool perfectplay = false; //whether perfect play is discovered
	Move* returnvalue = new Move; //starts empty, to be filled then returned as best Move
	returnvalue->column = -7; //placeholder bad values
	returnvalue->isPop = false; //placeholder bad values
	Move* tempreturnvalue = new Move; //used to store the best value when we're not completely through with an iteration, don't want the best value to be recorded as the better of the first two Moves 6 Moves later because we didn't get to the 6th portion of the 3rd Move which is better
	tempreturnvalue->column = -7; 
	tempreturnvalue->isPop = false; 
	timesup = false; //whether we are running out of time
	
	int tempvalue = 0; //value of a currently checked branch
	int bestMovevalue; //value of the optimal board state found so far


	while(perfectplay == false && timesup == false)
	{
		bestMovevalue = -1002; // resets it at the start of the loop, any result from next branch down will be at least -1000
		depthtoadd++; //increase the depth to check
		//cout << depthtoadd << "\n";
		for (int i = 0; i<width*2; i++)
		{
			//cout << i/width << "\n";
			if (storedboard->isValidMove(i%width, getmax, i>=width))
			{

				branch = new Movetree(storedboard->duplicate(), depthtoadd-1, !getmax, width, timer, timeleft);
				branch->storedboard->makeMove(i%width, getmax, i>=width);

				tempvalue = branch->Evalhelper(bestMovevalue); //no longer checks Move itself, check that it's being done elsewhere
					


				if(tempvalue == -1001) //figure out exact number later
				{
					timesup=true; //stop going through the loop
					delete(branch); //delete the branch that was being checked
					break; //leave the for loop
				}
				else if (tempvalue > bestMovevalue)  //this branch has a higher result than the others
				{
					/*if(i >= width)
					{
						cout << "popping is an optimal move" << endl;
					}*/
					bestMovevalue = tempvalue; //store the better result
					
					tempreturnvalue->column = i%width; //store the move until we finish all branches at this depth
					tempreturnvalue->isPop = i>=width;	
				}
				delete(branch); //delete the branch once we're done with it
			}
		}
		
		
		if(!timesup) //if we still have time
		{
			returnvalue = tempreturnvalue; //store the best move from the completed depth

			if(bestMovevalue == 1000)
			{
				perfectplay = true;
			}
		}
	}
	return returnvalue;
	
}

//Recursively calls itself until it gets to the end of the Move tree as it has currently been made, then uses alpha-beta pruning to skip past certain




int Movetree::Evalhelper(int currentminmax) 
{
	int tempvalue = -1002;
	int returnvalue = -1002;


	LARGE_INTEGER holder = timer->checkCount();
	if(holder.QuadPart > (timeleft-1)*1000) //check if there's more than a second left
	{	return -1001;} //if not return that time is up

	if(depthtoadd <= 0) // check if furthest down node 
	{
		statevalue = storedboard->evaluate();
		return statevalue;
	}
	else
	{
		for(int i = 0; i<width; i++)
		{
			for(int j = 0; j<2; j++)
			{
				if (storedboard->isValidMove(i, getmax, j)) //check if the move is valid
				{

					branch = new Movetree(storedboard->duplicate(), depthtoadd-1, !getmax, width, timer, timeleft);
					branch->storedboard->makeMove(i, getmax, j); //update the stored board

					//cout << "run through\n";

					tempvalue = branch->Evalhelper(returnvalue);


					if(tempvalue == -1001) //if time running out 
						returnvalue = tempvalue; //return that time is running out instead of doing more recursive searches
					else if (getmax == true) //if this depth is the player's turn
					{
						if(tempvalue >returnvalue || returnvalue == -1002) //if this move is better than the best one from this branch at this layer so far, or if there hasn't been a different one at this branch and this layer so far
						{
							returnvalue = tempvalue; //set this as the new highest value
							delete(branch); //delete the branch now that we are done with it
							if(tempvalue > currentminmax && currentminmax != -1002) //if this is better than another move, and it's not the highest branch possible
							{
								i = 8; 
								j = 3;
							}

						}
						else
						{
							delete(branch); //delete the branch since the value wasn't high enough to be relevant
						}
					}
					else //if opponent's turn
					{
						if(tempvalue < returnvalue || returnvalue == -1002) //if this is the worst recorded move so far for the player past this depth
						{
							returnvalue = tempvalue; //store it
							delete(branch);
							if(tempvalue < currentminmax && currentminmax != -1002) //if this is worse for the player than another move, and it's not the highest branch possible
							{
								i = 8; 
								j = 3;
							}
							
						}
						else
						{
							delete(branch); //delete the branch since the value wasn't high enough to be relevant
						}
					}

					
				}
			}
		}
				


	}
	return returnvalue; // return the best/worst move possible
}