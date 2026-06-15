# La-Brisca
To play against any of the opponent simply uncomment the corresponding cofiguration on the file properties.h

To select the rule-based oponents uncomment the tag RULES_BS for basic opponent or comment it for advanced opponent.

If an DQL agent is your desired opponent remember to select the version from agents folder. Note: only versions 200 or higher are compatible with the last state. Use 201 for Agent without memory (comment DQL_mem) ans 252 for one with memory (uncomment DQL_mem).

Before runing the script you need to activate the virtual enviroment inside the folfer DQL_Agent.
source DQL_Agent/.env/bin/activate
