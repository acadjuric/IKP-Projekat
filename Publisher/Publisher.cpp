#include "../Common/Publisher.h"


int __cdecl main(int argc, char **argv)
{
	// variable used to store function return value
	int iResult;


	if (InitializeWindowsSockets() == false)
	{
		// we won't log anything since it will be logged
		// by InitializeWindowsSockets() function
		return 1;
	}

	if (Connect())
		return 1;

	char topic[21];
	char message[491];

	// OVO JE KORISCENO ZA STRES TESTOVE, nemoj brisati zbog odbrane
	char unos[2];
	
	printf("Unesi: 1 (za kratke poruke) ; 2 (za dugacke poruke) ; bilo sta za dalji rad\n");
	gets_s(unos, 2);
	int izbor = unos[0] - '0';

	char topic2[21];
	char message2[491];


	char topic3[21];
	char message3[491];

	
	if (izbor == 1) {
		
		//negativan broj salji kratke poruke
		printf("SALJEM KRATKE PORUKE\n");
		strcpy_s(topic3, "jetset");
		strcpy_s(message3, "Vesti o poznatima.");
		for (int i = 0; i < 500; i++) {

			iResult = Publish((void*)topic3, (void*)message3);
			printf("Redni broj: %d\n", i);
			//Sleep(800);
			//desila se greska prilikom slanja strukture;
			if (iResult == -1) break;

		}
		
	}
	else if(izbor == 2) {
		
		// salji dugacke poruke
		printf("SALJEM DUGACKE PORUKE\n");
		int random = rand();
		strcpy_s(topic, "sport");
		strcpy_s(message, "Asked if Tuesday's win at Turf Moor felt like a big moment for United, Pallister replied: 'It did.To hit the top of the table, albeit just after Christmas was a great feeling.It's been a long road back there and it's justification for Ole and the way he's been talking about his team and how they weren't very far away from clicking. To move to the top of the table last night justifies the work he's been doing..");
		strcpy_s(topic2, "vreme");
		strcpy_s(message2, "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA");
		// NASUMICNO SALNJE DUGIH PORUKA NA 2 TOPICA -> PROVERENO RADI
		for (int i = 0; i < 100; i++) {

			if (random % 2 == 0) {
				random += rand();
				iResult = Publish((void*)topic, (void*)message);
				printf("Redni broj: %d\n", i);
				//Sleep(800);
				//desila se greska prilikom slanja strukture;
				if (iResult == -1) break;
			}
			else {
				random += rand();
				iResult = Publish((void*)topic2, (void*)message2);
				printf("Redni broj: %d\n", i);
				//Sleep(800);
				//desila se greska prilikom slanja strukture;
				if (iResult == -1) break;
			}


			//iResult = Publish((void*)topic, (void*)message);
			//printf("Redni broj: %d\n", i);
			////Sleep(800);
			////desila se greska prilikom slanja strukture;
			//if (iResult == -1) break;

		}
	}

	//END STRES TESTS
	

	do {


		printf("Unesite TOPIC za vest:");
		gets_s(topic, 20);
		strcpy_s(topic, TopicToLower(topic));
		printf("Unesite sada poruku:");
		gets_s(message, 490);

		if (strcmp("kraj", topic) == 0 || strcmp("kraj", message) == 0 || strcmp("exit", topic) == 0 || strcmp("exit", message) == 0) break; //gracefully
		
		iResult = Publish((void*)topic, (void*)message);

		//desila se greska prilikom slanja strukture;
		if (iResult == -1) break;

		printf("Bytes Sent: %ld\n", iResult);

	} while (true);





	// cleanup
	closesocket(connectSocket);
	WSACleanup();

	return 0;
}
