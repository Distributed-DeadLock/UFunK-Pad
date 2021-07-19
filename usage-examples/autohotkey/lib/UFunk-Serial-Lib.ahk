; UFunK-Serial-Lib.ahk
;  a library to manage the serial communication with the 
;  UFunK-Pad Sketch for Arduino Leonardo(or compatible) and Waveshare 2.8inch Touch LCD Shield for Arduino
;  https://github.com/Distributed-DeadLock/UFunK-Pad
;
; based on work by:
;  nkruzan   (https://github.com/nkruzan/arduino-servo-example) and 
;  aobrien 	 (https://autohotkey.com/board/topic/26231-serial-com-port-console-script/) and
;  engunneer (https://autohotkey.com/board/topic/44145-serial-port-stuff-seeking-an-authoritative-answer/#entry274633)
;
;
; Functions:
;
;	UFunK_Autodetect_UFunK_Pad()
;	Checks all availiable COM-Ports to find the UFunK-Pad
;		Returns a Handle to the first found UFunK-Pad
;
;	UFunK_Button_Text(Handle, Button, Text)
;	Set Text-Label of a Button
;	Handle = a File-Handle for the COM-Port (gotten from UFunK_COM_Init() or UFunK_Autodetect_UFunK_Pad())
;	Button = Number of the Button 13 - 24
;	Text   = Text-Label for the Button (max. 10 chars)
;	Returns a String with the response to the sent command 
;			(usually "OK". "ERROR : ..." or "0" when there was an error)
;
;	UFunK_Button_Color(Handle, Button, RGB888)
;	Set background Color of a Button
;	Handle = a File-Handle for the COM-Port (gotten from UFunK_COM_Init() or UFunK_Autodetect_UFunK_Pad())
;	Button = Number of the Button 13 - 24
;	Color = a number (in HEX or DEC) representing a RGB888-color
;	Returns a String with the response to the sent command 
;			(usually "OK". "ERROR : ..." or "0" when there was an error)
;
;	UFunK_Button_TextColor(Handle, Button, RGB888)
;	Set text Color of a Button
;	Handle = a File-Handle for the COM-Port (gotten from UFunK_COM_Init() or UFunK_Autodetect_UFunK_Pad())
;	Button = Number of the Button 13 - 24
;	Color = a number (in HEX or DEC) representing a RGB888-color
;	Returns a String with the response to the sent command 
;			(usually "OK". "ERROR : ..." or "0" when there was an error)
;
;	UFunK_Button_BorderColor(Handle, Button, RGB888)
;	Set border Color of a Button
;	Handle = a File-Handle for the COM-Port (gotten from UFunK_COM_Init() or UFunK_Autodetect_UFunK_Pad())
;	Button = Number of the Button 13 - 24
;	Color = a number (in HEX or DEC) representing a RGB888-color
;	Returns a String with the response to the sent command 
;			(usually "OK". "ERROR : ..." or "0" when there was an error)
;
;	UFunK_Button_Pushed_Color(Handle, Button, RGB888)
;	Set background Color of a Pushed-Button
;	Handle = a File-Handle for the COM-Port (gotten from UFunK_COM_Init() or UFunK_Autodetect_UFunK_Pad())
;	Button = Number of the Button 13 - 24
;	Color = a number (in HEX or DEC) representing a RGB888-color
;	Returns a String with the response to the sent command 
;			(usually "OK". "ERROR : ..." or "0" when there was an error)
;
;	UFunK_Button_Pushed_TextColor(Handle, Button, RGB888)
;	Set text Color of a Pushed-Button
;	Handle = a File-Handle for the COM-Port (gotten from UFunK_COM_Init() or UFunK_Autodetect_UFunK_Pad())
;	Button = Number of the Button 13 - 24
;	Color = a number (in HEX or DEC) representing a RGB888-color
;	Returns a String with the response to the sent command 
;			(usually "OK". "ERROR : ..." or "0" when there was an error)
;
;	UFunK_Button_Pushed_BorderColor(Handle, Button, RGB888)
;	Set border Color of a Pushed-Button
;	Handle = a File-Handle for the COM-Port (gotten from UFunK_COM_Init() or UFunK_Autodetect_UFunK_Pad())
;	Button = Number of the Button 13 - 24
;	Color = a number (in HEX or DEC) representing a RGB888-color
;	Returns a String with the response to the sent command 
;			(usually "OK". "ERROR : ..." or "0" when there was an error)
;
;	UFunK_Button_ReDraw(Handle, Button)
;	Redraw the Button with the new (text & color) properties
;	Handle = a File-Handle for the COM-Port (gotten from UFunK_COM_Init() or UFunK_Autodetect_UFunK_Pad())
;	Button = Number of the Button 13 - 24
;	Returns a String with the response to the sent command 
;			(usually "OK". "ERROR : ..." or "0" when there was an error)
;
;	UFunK_InfoLine_Text(Handle, Line, Text)
;	Set Text of a Info-Line
;	Handle = a File-Handle for the COM-Port (gotten from UFunK_COM_Init() or UFunK_Autodetect_UFunK_Pad())
;	Line = Number of the Info-Line 1 or 2
;	Text   = Text-Label for the Button (max. 17 chars)
;	Returns a String with the response to the sent command 
;			(usually "OK". "ERROR : ..." or "0" when there was an error)
;
;	UFunK_InfoLine_Color(Handle, Line, RGB888)
;	Set Color of a Info-Line
;	Handle = a File-Handle for the COM-Port (gotten from UFunK_COM_Init() or UFunK_Autodetect_UFunK_Pad())
;	Line = Number of the Info-Line 1 or 2
;	Color = a number (in HEX or DEC) representing a RGB888-color
;	Returns a String with the response to the sent command 
;			(usually "OK". "ERROR : ..." or "0 when there was an error)
;
;
;
;	UFunK_COM_Init(Port, HaltOnError := 0)
;	Set up a COM-Port to use with the other UFunK-COM..-functions
;		Port 	= a String containing the COM-Port Name (e.g. COM1 or COM12)
;		HaltOnError = Default "0". To show Error-Messages and Close Script on Error set to "1"
;		Returns	a File-Handle for the COM-Port
;
;	UFunK_COM_Close(Handle, HaltOnError := 0)
;	Close a COM-Port (use before exiting the program to clean-up)
;		Handle 	= a File-Handle for the COM-Port (gotten from UFunK_COM_Init())
;		HaltOnError = Default "0". To show Error-Messages on Error set to "1"
;		Returns	1 on success, 0 on failure
;
;	UFunK_COM_Send_String(Handle, String, HaltOnError := 0)
;	Send a String of text to the UFunK-Pad connected to the COM-Port, converting it to ANSI beforehand
;		Handle 	= a File-Handle for the COM-Port (gotten from UFunK_COM_Init())
;		String  = the String to send
;		HaltOnError = Default "0". To show Error-Messages on Error set to "1"
;		Returns then number of sent Bytes
;
;	UFunK_COM_Read_String(Handle, HaltOnError := 0 )
;	Receive a String of text from the UFunK-Pad connected to the COM-Port, converting it from ANSI beforehand
;		Handle 	= a File-Handle for the COM-Port (gotten from UFunK_COM_Init())
;		HaltOnError = Default "0". To show Error-Messages and Close Script on Error set to "1"
;		Returns a String with the received Text
;
;	UFunK_COM_Issue_Command(Handle, Command)
;	Send a command-string to the UFunK-Pad and receive the answer to that command
;		Handle	= a File-Handle for the COM-Port (gotten from UFunK_COM_Init())
;		Command = a string with a command for the UFunK-Pad
;		Returns a String with the response to the sent command 
;				(usually "OK". "ERROR : ..." or "0" when there was an error or "UFunK-Pad READY" when "HELLO" was sent)
;
; 	UFunK_List_COM_Ports()
; 	get list of all COM-Ports in an array
;		Returns an Array with the COM-Port Names (e.g. COM1 or COM12) registered on the Computer
;
;	UFunK_RGB_888to565(rgb888)
;	convert a RGB888-color (e.g. 0xFFFFFF or 16777215) to a RGB565-color in decimal (e.g. 65535)
;		rgb888	= a number (in HEX or DEC) representing a RGB888-color
;		Returns a number in DEC representing the nearest RGB565-color
;

; UFunK_Autodetect_UFunK_Pad	- Begin
UFunK_Autodetect_UFunK_Pad(){
	loop{
		portlist := UFunK_List_COM_Ports()
		; try all COM-Ports found
		for index, element in portlist{
			Handle := UFunK_COM_Init(element)
			if Handle {
				sleep, 50
				answer := UFunK_COM_Issue_Command(Handle, "HELLO")
				if InStr(answer, "UFunK-Pad READY")
					return Handle
				else
					UFunK_COM_Close(Handle)
			}
		}
		MsgBox, 37, UFunK-Pad Not Found, The UFunK-Pad could not be found. `n Try again or Cancel (and Exit the Program)?
		IfMsgBox, Cancel
			ExitApp
	}
}
; UFunK_Autodetect_UFunK_Pad	- End

; UFunK Button Commands	- Begin
UFunK_Button_Text(Handle, Button, Text){
	if ((Button < 13) || (Button > 24))
		return 0
	Text := SubStr(Text, 1 , 10)
	Command := "F" . Button . "TL" . Text
	return UFunK_COM_Issue_Command(Handle, Command)
}
UFunK_Button_Color(Handle, Button, RGB888){
	if ((Button < 13) || (Button > 24))
		return 0
	Color := UFunK_RGB_888to565(RGB888)
	Command := "F" . Button . "CF" . Color
	return UFunK_COM_Issue_Command(Handle, Command)
}
UFunK_Button_TextColor(Handle, Button, RGB888){
	if ((Button < 13) || (Button > 24))
		return 0
	Color := UFunK_RGB_888to565(RGB888)
	Command := "F" . Button . "CL" . Color
	return UFunK_COM_Issue_Command(Handle, Command)
}
UFunK_Button_BorderColor(Handle, Button, RGB888){
	if ((Button < 13) || (Button > 24))
		return 0
	Color := UFunK_RGB_888to565(RGB888)
	Command := "F" . Button . "CB" . Color
	return UFunK_COM_Issue_Command(Handle, Command)
}
UFunK_Button_Pushed_Color(Handle, Button, RGB888){
	if ((Button < 13) || (Button > 24))
		return 0
	Color := UFunK_RGB_888to565(RGB888)
	Command := "F" . Button . "PF" . Color
	return UFunK_COM_Issue_Command(Handle, Command)
}
UFunK_Button_Pushed_TextColor(Handle, Button, RGB888){
	if ((Button < 13) || (Button > 24))
		return 0
	Color := UFunK_RGB_888to565(RGB888)
	Command := "F" . Button . "PL" . Color
	return UFunK_COM_Issue_Command(Handle, Command)
}
UFunK_Button_Pushed_BorderColor(Handle, Button, RGB888){
	if ((Button < 13) || (Button > 24))
		return 0
	Color := UFunK_RGB_888to565(RGB888)
	Command := "F" . Button . "PB" . Color
	return UFunK_COM_Issue_Command(Handle, Command)
}
UFunK_Button_ReDraw(Handle, Button){
	if ((Button < 13) || (Button > 24))
		return 0
	Command := "F" . Button . "DB"
	return UFunK_COM_Issue_Command(Handle, Command)
}
; UFunK Button Commands	- End

; UFunK InfoLine Commands	- Begin
UFunK_InfoLine_Text(Handle, Line, Text){
	if ((Line != 1) && (Line != 2))
		return 0
	Text := SubStr(Text, 1 , 17)
	Command := "I" . Line . "TL" . Text
	return UFunK_COM_Issue_Command(Handle, Command)
}
UFunK_InfoLine_Color(Handle, Line, RGB888){
	if ((Line != 1) && (Line != 2))
		return 0
	Color := UFunK_RGB_888to565(RGB888)
	Command := "I" . Line . "TC" . Color
	return UFunK_COM_Issue_Command(Handle, Command)
}	
; UFunK InfoLine Commands	- End

; ----------------------------
; UFunK_COM_Init 	- Begin
UFunK_COM_Init(Port, HaltOnError := 0){
	; Format the COM-Port: For COM Ports > 9 \\.\ needs to prepended to the COM Port name.	
	If (StrLen(Port) > 4)
		Port = \\.\%Port%
	; Serial Settings String , has same syntax as in MODE-command
	SERIAL_Settings = %Port%:baud=9600 parity=N data=8 stop=1 dtr=off to=off  xon=off odsr=off octs=off rts=on idsr=off
	
	;Creates the structure that contains the COM Port number, baud rate,...
	VarSetCapacity(DCB, 28)
	BCD_Result := DllCall("BuildCommDCB"
		,"str" , SERIAL_Settings	;lpDef
		,"UInt", &DCB)        		;lpDCB
	If (BCD_Result <> 1){
		error := DllCall("GetLastError")
		if HaltOnError {
			MsgBox, There is a problem with Serial Port communication. `nFailed Dll BuildCommDCB, BCD_Result=%BCD_Result% `nLasterror=%error%`nThe Script Will Now Exit.
			ExitApp
		}
		else
			return 0
	}
	
	;Creates the COM Port File Handle
	Handle := DllCall("CreateFile"
		,"Str" , Port    ;File Name
		,"UInt", 0xC0000000   	;Desired Access
		,"UInt", 3            	;Safe Mode
		,"UInt", 0            	;Security Attributes
		,"UInt", 3            	;Creation Disposition
		,"UInt", 0            	;Flags And Attributes
		,"UInt", 0            	;Template File
		,"Cdecl Int")
	If (Handle < 1){
		error := DllCall("GetLastError")
		if HaltOnError {
			MsgBox, There is a problem with Serial Port communication. `nFailed Dll CreateFile, Handle=%Handle% `nLasterror=%error%`nThe Script Will Now Exit.
			ExitApp
		}
		else return 0
	}

	;Sets the COM Port number, baud rate,...
	SCS_Result := DllCall("SetCommState"
		,"UInt", Handle 	;File Handle
		,"UInt", &DCB)          	;Pointer to DCB structure
	If (SCS_Result <> 1){
		error := DllCall("GetLastError")
		UFunK_COM_Close(Handle)
		if HaltOnError {
			MsgBox, There is a problem with Serial Port communication. `nFailed Dll SetCommState, SCS_Result=%SCS_Result% `nLasterror=%error%`nThe Script Will Now Exit.			
			ExitApp
		}
		else
			return 0
	}
	
	;Create the SetCommTimeouts Structure 
	ReadIntervalTimeout        = 0xffffffff
	ReadTotalTimeoutMultiplier = 0x00000000
	ReadTotalTimeoutConstant   = 0x00000000
	WriteTotalTimeoutMultiplier= 0x00000000
	WriteTotalTimeoutConstant  = 0x00000000
	VarSetCapacity(Data, 20, 0) 	; 5 * sizeof(DWORD)
	NumPut(ReadIntervalTimeout,         Data,  0, "UInt")
	NumPut(ReadTotalTimeoutMultiplier,  Data,  4, "UInt")
	NumPut(ReadTotalTimeoutConstant,    Data,  8, "UInt")
	NumPut(WriteTotalTimeoutMultiplier, Data, 12, "UInt")
	NumPut(WriteTotalTimeoutConstant,   Data, 16, "UInt")
	
	;Set the COM Timeouts
	SCT_result := DllCall("SetCommTimeouts"
		,"UInt", Handle 	;File Handle
		,"UInt", &Data)         	;Pointer to the data structure
	If (SCT_result <> 1){
		error := DllCall("GetLastError")
		UFunK_COM_Close(Handle)
		if HaltOnError {
			MsgBox, There is a problem with Serial Port communication. `nFailed Dll SetCommState, SCT_result=%SCT_result% `nLasterror=%error%`nThe Script Will Now Exit.
			ExitApp
		}
		else
			return 0
	}
	Return Handle
}
; UFunK_COM_Init 	- End

; UKunK_COM_Close	- Begin
UFunK_COM_Close(Handle, HaltOnError := 0){
	; Close the COM File
	CH_result := DllCall("CloseHandle", "UInt", Handle)
	If (CH_result <> 1){
		if HaltOnError
			MsgBox, Failed Dll CloseHandle CH_result=%CH_result%
		Return 0
	}
	Return 1
}
; UFunK_COM_Close	- End

; UFunK_COM_Send_String		- Begin
UFunK_COM_Send_String(Handle, String, HaltOnError := 0){
	; Transform String to Code Page 1252 , aka "extended ASCII"
	;  get Length of String in bytes
	Data_Length := StrPut(String, "CP1252")
	;  Ensure capacity.
    VarSetCapacity(ANSIString, Data_Length)
    ;  Copy or convert the string.
    StrPut(String, &ANSIString, "CP1252")
	
	; Write the data to the COM Port
	WF_Result := DllCall("WriteFile"
		,"UInt" , Handle 		;File Handle
		,"UInt" , &ANSIString   ;Pointer to string to send
		,"UInt" , Data_Length   ;Data Length
		,"UInt*", Bytes_Sent    ;Returns pointer to num bytes sent
		,"Int"  , "NULL")
	If ((WF_Result <> 1 or Bytes_Sent <> Data_Length) && HaltOnError )
		MsgBox, Failed Dll WriteFile to COM Port, result=%WF_Result% `nData Length=%Data_Length% `nBytes_Sent=%Bytes_Sent%
   
    Return Bytes_Sent
}
; UFunK_COM_Send_String		- End

; UFunK_COM_Read_String		- Begin
UFunK_COM_Read_String(Handle, HaltOnError := 0 ){
	;Set the Data buffer size, 25 is enough for the longest string sent by the UFunK-Pad
	Data_Length  := VarSetCapacity(Data, 25, 0)
	
	; Read the data from the COM Port
	Read_Result := DllCall("ReadFile"
		,"UInt" , Handle			; hFile
		,"Str"  , Data             	; lpBuffer
		,"Int"  , Data_Length      	; nNumberOfBytesToRead
		,"UInt*", Bytes_Received   	; lpNumberOfBytesReceived
		,"Int"  , 0)               	; lpOverlapped
	If (Read_Result <> 1){
		UFunK_COM_Close(Handle)
		If HaltOnError {
			MsgBox, There is a problem with Serial Port communication. `nFailed Dll ReadFile on COM Port, result=%Read_Result% - The Script Will Now Exit.
			ExitApp
		}
		else
			Return 0
	}
	
	; Convert String from Code Page 1252 , aka "extended ASCII"
	String := StrGet(&Data, Bytes_Received, "CP1252")
	
	Return String
}
; UFunK_COM_Read_String		- End

; UFunK_COM_Issue_Command	- Begin
UFunK_COM_Issue_Command(Handle, Command){
	; add line-feed at the end of the command
	CommandLine := Command . "`n"
	; send the command
	UFunK_COM_Send_String(Handle, CommandLine)
	; loop reading from COM-port (max 32 times) until line-break is found
	loop, 32
	{
		sleep, 1
		answer .= UFunK_COM_Read_String(Handle)
		if InStr(answer, "`n" ,0, 0) 
			break
	}
	answer := StrReplace(answer, "`r", "")
	answer := StrReplace(answer, "`n", "")
	Return answer
}
; UFunK_COM_Issue_Command	- End

; UFunK_List_COM_Ports		- Begin
UFunK_List_COM_Ports(){
	comports := []
	Loop, Reg, HKLM\HARDWARE\DEVICEMAP\SERIALCOMM
	{
		RegRead, ComPort
		comports[A_Index] := ComPort
	}
	return comports
}
; UFunK_List_COM_Ports		- End

; UFunK_RGB_888to565		- Begin
UFunK_RGB_888to565(rgb888){
	SetFormat, Integer, hex
	; seperate R, G & B channels
	R8 := rgb888 >> 16
	G8 := (rgb888 >> 8) & 0x00FF
	B8 := rgb888 & 0x0000FF	
	; convert channels to rgb565 - format
	; formula from: https://stackoverflow.com/questions/2442576/how-does-one-convert-16-bit-rgb565-to-24-bit-rgb888
	R5 := ( R8 * 249 + 1014 ) >> 11
	G6 := ( G8 * 253 +  505 ) >> 10
	B5 := ( B8 * 249 + 1014 ) >> 11	
	; merge channels
	TR := R5 << 11
	TG := G6 << 5
	TB := B5	
	rgb565 := TR + TG +TB
	; convert to decimal
	SetFormat, Integer, D
	rgb565 += 0
	Return rgb565
}
; UFunK_RGB_888to565		- End