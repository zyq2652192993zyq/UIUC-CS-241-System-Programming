#include <iostream.h>
#include <stdio.h>
#include <fstream>
#include <string.h>
#include <map.h>

int main()
{
	FILE * fptr;
	string buffer;
	char * myline;

	multimap<string, string> mymap1;
	unordered_set<string, string>chemistry;
	unordered_set<string, string>physics;
	unordered_set<string, string>math;
	unordered_set<string, string>history;

	fptr = fopen("myfile.txt", "r");

	if(pFile==NULL) 
		{
			perrro("Error");
		}
	else
	{
		int temp;
		int size;
		string classname;
		string proname;
		int sid;
		while(getline(fptr, buffer));
		{
			sscanf(buffer, "%s, %s, %d", classname, proname, &sid);
			switch(classname)
			{
				case "Chemistry":
					if(chemistry.find(proname))
						break;
					else
					{
						mymap.insert(classname, proname);
						chemistry.insert(proname);
					}
					break; 

				case "History":
					if(history.find(proname))
						break;
					else
					{
						mymap.insert(classname, proname)
						history.insert(proname);
					}
					break; 

				case "Mathematics":
					if(math.find(proname))
						break;
					else
					{
						mymap.insert(classname, proname);
						math.insert(proname);
					}
					break; 

				case "Physics":
					if(physics.find(proname))
						break;
					else
					{
						mymap.insert(classname, proname);
						physics.insert(proname);
					}
					break; 
			}

		}
		for(iterator it = mymap1.begin();it!=mymap1.end();it++)
		{
			cout<<it->first<<", "<<it->second<<endl;
		}	

	}

	fclose(fptr);
}



/*

unordered_map<int, unordered_set<string>>
iterator it;
it->second.insert(classname);

*/





