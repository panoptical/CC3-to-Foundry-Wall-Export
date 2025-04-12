#include "Main.h"


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
