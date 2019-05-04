#ifndef CONVERSATION_H
#define CONVERSATION_H

#include <QString>

struct Conversation{
   int people;
   int robot;
   QString says;

   Conversation():people(0),robot(0), says("")
   {}
   Conversation(   int people,int robot, QString says ):people(people),robot(robot), says(says)
   {}
};

#endif // CONVERSATION_H
