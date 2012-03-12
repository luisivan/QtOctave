#include "menubar.h"

void ActionEvent::triggered()
{
	emit triggered_event("*triggered:"+name+"\n");
}
