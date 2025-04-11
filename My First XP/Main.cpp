#include "Main.h"


char CList[] = "DEMO\0TESTNEWFUNCTIONS\0EXPORTWALLS\0EXPORTWALLMAP\0BB\0\0";
PCMDPROC PList[] = { About, Demo, TestNewFunctions, ExportWalls, ExportRect, GetBB };

XP MyXP = { 0, CList, PList, 0, 0, 0, XPID, 0, 620, 0, 0, 620 };


/////////////  DllMain - XP initialization & Unload code //////////////
BOOL WINAPI DllMain(const HINSTANCE hDLL, const DWORD dwReason, LPVOID lpReserved) {
	UNREFERENCED_PARAMETER(lpReserved);

	switch (dwReason)
	{
	case DLL_PROCESS_ATTACH:
		MyXP.ModHdl = hDLL;
		_XPRegCmd(&MyXP);
		break;
	case DLL_PROCESS_DETACH:
		_XPUnregCmd(&MyXP);
		break;
	default:
		break;
	}
	return TRUE;
}


void XPCALL About() {

	FORMST(MyAPkt, "CC3+ XP Template 2022\n\n"
		"About box for CC3+ XP Template 2022"
	)

	FormSt(&MyAPkt, RSC(FD_MsgBox));
	CmdEnd();
}

char name[50];
char SceneName[50];

void XPCALL SayHello(const int Result, int Result2, int Result3)
{
	if (Result != X_OK) { CmdEnd(); return; }
	UNREFERENCED_PARAMETER(Result2);
	UNREFERENCED_PARAMETER(Result3);

	FORMSTPKT(MyFPkt, "Hi, !01\nHave a Nice Day\0", 1)
		ITEMFMT(name, FT_Stg, FJ_Var, 0, 0)
		FORMSTEND

		FormSt(&MyFPkt, RSC(FD_MsgBox));
	CmdEnd();
}

void XPCALL Demo() {
	FORMST(lpszPrompt, "Your Name:\0")

		RDATA NameReq =
	{ sizeof(RDATA), RD_TxWord, NULL, RDF_C, (DWORD*)&name,
		(DWORD*)&lpszPrompt,RDC_ARROW, SayHello, NULL, NULL, 0, NULL, 0 };

	ReqData(&NameReq);
}

void XPCALL TestNewFunctions() {
	std::string filePath = GetSaveFilePath();
	if (!filePath.empty()) {
		SaveTextToFile(filePath, "Hello, this is a test text saved to a file!");

		FORMST(MyAPkt, "Success\n\n"
			"File saved successfully."
		)
			FormSt(&MyAPkt, RSC(FD_MsgBox));
	}
	else {
		FORMST(MyAPkt, "Error\n\n"
			"File save operation was cancelled."
		)
			FormSt(&MyAPkt, RSC(FD_MsgBox));
			// Handle the case where the user cancels the file save dialog
			// You can show a message or take any other action as needed
	}
	CmdEnd();    
}

//function to troubleshoot bounding box request
//

GLINE2 boundingbox;

void XPCALL GetBB(void) {
	FORMST(BBPrompt, "Select Lower Left, then Upper Right.\0")

		RDATA BBReq =
	{ sizeof(RDATA), RD_Win, NULL, RDF_C, (DWORD*)&boundingbox,
		(DWORD*)&BBPrompt,RDC_XH, MSGBB, NULL, NULL, 0, NULL, 0 };

	ReqData(&BBReq);
}

void XPCALL MSGBB(const int Result, int Result2, int Result3)
{
	if (Result != X_OK) { CmdEnd(); return; }
	UNREFERENCED_PARAMETER(Result2);
	UNREFERENCED_PARAMETER(Result3);

	FORMSTPKT(BBFPkt, "(!01)\n(!02)\0", 2)
		ITEMFMT(boundingbox.p1, FT_2dC4, FJ_Var, 1, 0)
		ITEMFMT(boundingbox.p2, FT_2dC4, FJ_Var, 1, 0)
		FORMSTEND

		FormSt(&BBFPkt, RSC(FD_MsgBox));
	CmdEnd();
}


std::string wallsJSON = "";
int feetToPixels = 20; //to do: check for this programmatically
int p1x, p1y, p2x, p2y;
int widthFeet, heightFeet, widthPixels, heightPixels;
int wlnr = 0;


//stores the extent of the map grid. p1 = lower left, p2 = upper right
GLINE3 GridExt;


//function based on extent function in https://rpgmaps.profantasy.com/developing-add-ons-for-cc3-part-7-dynamic-dungeon-tools-3/
//get grid entity
//calculate extent
//this is very janky - just let the user define the box
//
void XPCALL FindGridExt() {
	//old code - didn't work right
	BgnPExtents();
	DLScan(NULL, FindGridEntities, DLS_UNLK | DLS_HSHTOK | DLS_NOWDC | DLS_RO, 0, 0);
	EndPExtents(&GridExt);

	




}

void XPCALL HandleExc(const int Result, int Result2, int Result3)
{
	if (Result != X_OK) { CmdEnd(); return; }
	UNREFERENCED_PARAMETER(Result2);
	UNREFERENCED_PARAMETER(Result3);

	CmdEnd();
}



// Finds all entities that are on the GRID layer for the extents check
pENTREC XPCALL FindGridEntities(hDLIST list, pENTREC entity, const DWORD parm1, DWORD parm2) {

	int layer = GetLayerNr("HEX/SQUARE GRID");
	//int layer = GetLayerNr("BACKGROUND (MAP)");
	//int layer = GetLayerNr("MAP BORDER");

	if (entity->CStuff.ELayer == layer) {
		EXCheck(entity);
	}

	return 0;
}

//Sets the parameters for exporting walls and map images
// basically all we need now is a bounding box for the part of the map that's on the grid
// this will be what we export as an image and also what we use to calculate the wall positions in Foundry
void SetParameters() {

	FindGridExt();
	widthFeet = GridExt.p2.x - GridExt.p1.x;
	heightFeet = GridExt.p2.y - GridExt.p1.y;
	widthPixels = widthFeet * feetToPixels;
	heightPixels = heightFeet * feetToPixels;
}

void XPCALL ExportWalls() {
	
	std::string filePath = GetSaveFilePath();	//prompt user to choose an export file name and path
													
	if (!filePath.empty()) {					//if they selected one, proceed with the export	

		wallsJSON = "";							//ensure holder for JSON is empty at the start
		
		SetParameters();						//calculates and sets parameters which determine height, width, scale, etc. of wall canvas
		AppendSceneName();						//adds beginning matter to wallsJSON (scene name, height, width, scale)
		
		wallsJSON.append("  \"walls\": [");		//start the walls entry
		
		GetWallsJSON();							//add a comma-separated list of all walls, with terminal comma
		
		wallsJSON.pop_back();					//remove the terminal comma
		wallsJSON.append("]\n}");				//add the end of the walls array and the JSON object
		
		SaveTextToFile(filePath, wallsJSON);	//write the file
												//inform the user:
		FORMST(MyAPkt, "Success\n\n"
			"File saved successfully."
		)
			FormSt(&MyAPkt, RSC(FD_MsgBox));
	}
	else {
		FORMST(MyAPkt, "Error\n\n"
			"File save operation was cancelled."
		)
			FormSt(&MyAPkt, RSC(FD_MsgBox));
		// Handle the case where the user cancels the file save dialog
	}

	CmdEnd();
}



//Exports the current map
// uses the export jpeg rectangle function to export only a selected rectangle
// We only want to export what's on the grid so our map will match up with the grid on the VTT
// So we use SetParameters to assign values to GridExt:
//  GridExt.p1 is the lower left point of the grid
//  GridExt.p2 is the upper right point of the grid
// Then we format the command as a string, cast it as a cstring, and call it
//
void XPCALL ExportRect() {
	SetParameters();
	//need to compose the command as a string
	std::string scriptText = "";
	scriptText.append("WBS ");
	scriptText.append(std::to_string(GridExt.p1.x));
	scriptText.append(",");
	scriptText.append(std::to_string(GridExt.p1.y));
	scriptText.append(" ");
	scriptText.append(std::to_string(GridExt.p2.x));
	scriptText.append(",");
	scriptText.append(std::to_string(GridExt.p2.y));
	scriptText.append(";");
	char* cstr = const_cast<char*>(scriptText.c_str());
	//FormSt(&cstr, RSC(FD_MsgBox));
	ExecScriptCopy(cstr); 
	CmdEnd();
}

//add the scene name to the json file
//to do: allow the user to set this?
//or set it based on the file name?
//to do: append all scene details, like size (maybe using border extent), padding, etc.
void XPCALL AppendSceneName()
{
	//append name
	wallsJSON.append("{\n  \"name\": \"");
	wallsJSON.append("Walls Export Demo");
	wallsJSON.append("\",\n");
	//append width
	wallsJSON.append("  \"width\": ");
	wallsJSON.append(std::to_string(widthPixels));
	wallsJSON.append(",\n");
	//append height
 	wallsJSON.append("  \"height\": ");
	wallsJSON.append(std::to_string(heightPixels));
	wallsJSON.append(",\n");
	//append padding
	wallsJSON.append("  \"padding\": 0,\n");

	CmdEnd();
}


//the main function where we're going to have to find the walls
//must aappend a correctly formatted JSON list of all the walls to wallsJSON
// This is a comma-separated list
// to do: scan the drawing list for walls
//  - walls are stored with sets of nodes
//  - call GetWallText on each pair of nodes
//  - append to result
//
// is it better to first grab the walls sheet and then only process entities on it?
//

void GetWallsJSON() {

	// fake data for testing 
	//int x1, y1, x2, y2;
	//x1 = y1 = 100;
	//x2 = y2 = 200;
	// fake call for testing
	//wallsJSON.append(GetWallText(x1, y1, x2, y2));

	//1. Write the callback function that actually processes each entity
	//2. Write the DLScan command here:
	//3. Profit!!


	//DLS Scan step 1: find out if there are any walls at all; if not we can just call it a day
	//also, get wlnr, the code we can pass to ProcessWall to help it check if an entity is a wall
	char* wallslayer = "WALLS";
	wlnr = GetLayerNr(wallslayer);
	if (wlnr) {
		//call processwall on everything
		//when I use DLS_SL it gave me 22 extra entities, for some reason
		 //investigate? Maybe later...
		//with just DLS_RO it seems to work
		DLScan(NULL, ProcessWall, DLS_RO, &wlnr, NULL);

		
	}




}

//callback function to process a single wall entity
pENTREC XPCALL ProcessWall(hDLIST list, const pENTREC entity, const DWORD parm1, DWORD parm2) {

	UNREFERENCED_PARAMETER(parm1);
	UNREFERENCED_PARAMETER(parm2);
	UNREFERENCED_PARAMETER(list);

	if (entity->CStuff.ELayer == wlnr) {
		//it's a wall!

		//loop through the nodes and call GetWallText on each relevant pair of nodes
		//do this in a helper function
		ProcessWallEntity(entity);
	}
		
	return 0;
}


void ProcessWallEntity(const pENTREC entity) {
	//debug: just record the wall and its tag
	//wallsJSON.append("\nSorry to cut in but I've found a wall enttiy with tag ");
	//wallsJSON.append(std::to_string(entity->CStuff.Tag));
	if (entity->Path.Path.Count > 0) //make sure it's not empty
	{
		for (int i = 0; i < entity->Path.Path.Count -1; i++) {
			//construct the ith wall
			MakeWall(entity, i, i + 1);
		}
		if (entity->Path.Path.Count == entity->Path.Path.EParm) //if it's closed
		{
			MakeWall(entity, 0, entity->Path.Path.Count);
		}
	}
	//testing:
	// this currently works when all the walls are PATH or POLYGON
	// There are also:
	// LINE
	// ARC
	// MULTIPOLY
	//
}

void MakeWall(const pENTREC entity, int node1, int node2) {
	int x1 = entity->Path.Path.Nodes[node1].x * feetToPixels;
	int y1 = heightPixels - (entity->Path.Path.Nodes[node1].y * feetToPixels);
	int x2 = entity->Path.Path.Nodes[node2].x * feetToPixels;
	int y2 = heightPixels - (entity->Path.Path.Nodes[node2].y * feetToPixels);
	wallsJSON.append(GetWallText(x1, y1, x2, y2));
}


//GetWallText
//Given x and y coordinates of a wall, returns a string containining a correctly formatted entry for a walls array in Foundry json format
//entry has a trailing comma - will need to delete the comma from the final entry, or remove it from here and add it between 
// entries in the wrapper function
std::string GetWallText(const int x1, const int y1, const int x2, const int y2) {
	std::string result = "";
	result.append("{\n      \"light\": 20,\n      \"sight\": 20,\n      \"sound\": 20,\n      \"move\": 20,\n      \"c\": [\n        ");
	result.append(std::to_string(x1));
	result.append(",\n        ");
	result.append(std::to_string(y1));
	result.append(",\n        ");
	result.append(std::to_string(x2));
	result.append(",\n        ");
	result.append(std::to_string(y2));
	result.append("\n      ],\n      \"_id\": \"");
	result.append(random_string(16));
	result.append("\",\n      \"dir\": 0,\n      \"door\": 0,\n      \"ds\": 0,\n      \"threshold\": {\n");
	result.append("        \"light\": null,\n        \"sight\": null,\n        \"sound\": null,\n        \"attenuation\": false\n      },\n");
	result.append("      \"flags\": {}\n    },");
	return result;
}


//basic file output function
//made with copilot
void SaveTextToFile(const std::string& filePath, const std::string& text) {
	std::ofstream outFile(filePath); // Open the file for writing
	if (outFile.is_open()) {
		outFile << text; // Write the text to the file
		outFile.close(); // Close the file
	}
	else {
		// Handle error if the file cannot be opened
		FORMST(MyAPkt, "Error: Unable to open file for writing.\n")
			FormSt(&MyAPkt, RSC(FD_MsgBox));
	}
}

//basic dialog to let the user select an export file
//made with copilot
std::string GetSaveFilePath() {
	char fileName[MAX_PATH] = ""; // Buffer to store the selected file path
	OPENFILENAME ofn;             // Structure to configure the file dialog
	ZeroMemory(&ofn, sizeof(ofn)); // Initialize the structure to zero

	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = NULL; // No owner window
	ofn.lpstrFilter = "Text Files\0*.txt\0All Files\0*.*\0"; // File type filters
	ofn.lpstrFile = fileName;
	ofn.nMaxFile = MAX_PATH;
	ofn.Flags = OFN_OVERWRITEPROMPT; // Prompt if the file already exists
	ofn.lpstrDefExt = "txt";         // Default file extension

	if (GetSaveFileName(&ofn)) {
		return std::string(fileName); // Return the selected file path
	}
	else {
		return ""; // Return an empty string if the user cancels
	}
}

// generates a random string
// call this with length 16 to generate wall IDs
// from https://github.com/InversePalindrome/Blog/tree/master/RandomString
std::string random_string(std::size_t length)
{
	const std::string characters = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

	std::random_device random_device;
	std::mt19937 generator(random_device());
	std::uniform_int_distribution<> distribution(0, characters.size() - 1);

	std::string random_string;

	for (std::size_t i = 0; i < length; ++i)
	{
		random_string += characters[distribution(generator)];
	}

	return random_string;
}