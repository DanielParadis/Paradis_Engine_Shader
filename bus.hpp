#pragma once
#include <string>
#include <sstream>
#include <vector>
/*
We are going to make a message/data bus to be able to send and receive events through the entire application.

We need to define what a message is:
	- Has an author
	- Has a recipient(s)
	- Contains a message or data

We need a way to send and receive these messages, sort of like an email client. We will call this a node. A node should be able to:
	- Receive messages
	- Send Messages

We need an authority to route these messages to their proper location, similar to an email server. This authority should:
	- Contain a database of all existing nodes
	- Route a message to the proper node(s)
*/

// Let's define a message

class message
{
private:
	// We privately define the data of the message so that no one can access them without using access functions
	std::string Author;
	std::string Recipient;
	std::string Data;
public:
	// Here we are defining our setter and our getter functions
	std::string getAuthor()
	{
		return Author;
	}
	std::string getRecipient() 
	{
		return Recipient;
	}
	std::string getData()
	{
		return Data;
	}
	void setAuthor(std::string author)
	{
		Author = author;
	}
	void setRecipient(std::string recipient)
	{
		Recipient = recipient;
	}
	void setData(std::string data) 
	{
		Data = data;
	}
	message()
	{

	}
	~message()
	{

	}
	void outputMessage()
	{
		std::cout << Author << " -> " << Recipient << ": " << Data << std::endl;
	}
};

class bus
{
public:
	std::vector<message> Received;
};

// Now we need to define our node

class node
{
private:
	// Our node will have a name. This will be used to label messages, and messages will be sent to that name
	std::string Name;
	// We have a buffer to handle our current message
	message Buffer;
	bus* Bus;
	int totalMessages;
public:
	std::vector<message> ReceivedMessages;
	void sendMessage(std::string recipient, std::string data)
	{
		Buffer.setData(data);
		Buffer.setRecipient(recipient);
		Buffer.setAuthor(Name);
		Bus->Received.push_back(Buffer);
	}
	std::string getMessage()
	{
		std::string Buffer;
		std::vector<message>::iterator it = ReceivedMessages.begin();
		if (it != ReceivedMessages.end())
		{
			--totalMessages;
			Buffer = it->getData();
			it = ReceivedMessages.erase(it);
		}
		return Buffer;
	}
	node(bus* ptrBus, std::string name)
	{
		Bus = ptrBus;
		Name = name;
		totalMessages = 0;
	}
	~node()
	{

	}
	void update()
	{
		for (std::vector<message>::iterator it = Bus->Received.begin(); it != Bus->Received.end(); )
		{
			if (it->getRecipient() == Name)
			{
				++totalMessages;
				ReceivedMessages.push_back(*it);
				it = Bus->Received.erase(it);
			}
			else
			{
				++it;
			}
		}
	}
	bus* getBus()
	{
		return Bus;
	}
	int getMessageCount()
	{
		return totalMessages;
	}
};

// Let's make our lives easier by creating a class that will take a message and turn it into a command with flags
class command
{
private:
	int numFlags;
	std::vector<std::string> flags;
public:
	int getNumFlags()
	{
		return numFlags;
	}
	void readString(std::string String)
	{
		numFlags = 0;
		flags.clear();
		std::string buffer = String;
		std::string flag;
		bool EOL = false;
		for (int i = 0; EOL == false; ++i)
		{
			if (buffer[i] != ' ' && buffer[i] != '\0')
			{
				flag += buffer[i];
			}
			else
			{
				flags.push_back(flag);
				++numFlags;
				flag = std::string{};
				if (buffer[i] == '\0')
					EOL = true;
			}
		}
	}
	std::string getFlags(int numFlag)
	{
		if (numFlag < 0 || numFlag >= numFlags)
		{
			return std::string{};
		}
		else
		{
			return flags[numFlag];
		}
	}
	float getFlagf(int numFlag)
	{
		if (numFlag < 0 || numFlag >= numFlags)
		{
			std::cout << "Invalid flag request!\n";
			return 0;
		}
		else
		{
			std::stringstream sstr(flags[numFlag]);
			float buf;
			if (!(sstr >> buf))
			{
				std::cout << "Could not get float from " << flags[numFlag] << std::endl;
			}
			return buf;
		}
	}
	double getFlagd(int numFlag)
	{
		if (numFlag < 0 || numFlag >= numFlags)
		{
			std::cout << "Invalid flag request!\n";
			return 0;
		}
		else
		{
			std::stringstream sstr(flags[numFlag]);
			double buf;
			if (!(sstr >> buf))
			{
				std::cout << "Could not get double from " << flags[numFlag] << std::endl;
			}
			return buf;
		}
	}
	int getFlagi(int numFlag)
	{
		if (numFlag < 0 || numFlag >= numFlags)
		{
			std::cout << "Invalid flag request!\n";
			return 0;
		}
		else
		{
			std::stringstream sstr(flags[numFlag]);
			int buf;
			if (!(sstr >> buf))
			{
				std::cout << "Could not get int from " << flags[numFlag] << std::endl;
			}
			return buf;
		}
	}
};