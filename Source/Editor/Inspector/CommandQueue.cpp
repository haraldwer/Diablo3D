#include "CommandQueue.h"

void CommandQueue::Add(Command* aCommand)
{
	while(off > 0)
	{
		if (myCommands[myCommands.size() - 1]->erase)
			myCommands[myCommands.size() - 1]->erase(*aCommand);
		delete myCommands[myCommands.size() - 1];
		myCommands.erase(myCommands.begin() + myCommands.size() - 1);
		off--;
	}
	myCommands.push_back(aCommand);
}

void CommandQueue::Revert()
{
	if (myCommands.empty())
		return;
	if (off < 0 || off >= myCommands.size())
		return;
	Command* command = myCommands[myCommands.size() - 1 - off];
	if(command->revert)
		command->revert(*command);
	off++;
}

void CommandQueue::Redo()
{
	if (off < 1 || off > myCommands.size())
		return;
	if (myCommands.empty())
		return;
	off--;
	auto c = myCommands[myCommands.size() - 1 - off];
	if (c->redo)
		c->redo(*c);
}
