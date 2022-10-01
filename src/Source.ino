// Ali Sedaghi
// 97521378

#include <LiquidCrystal.h>
#include <Keypad.h>
#include <Wire.h> 
#include <math.h>

#define BAUD_RATE 9600

// Keypad config
const byte ROWS = 4;
const byte COLS = 4;
char keys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};
byte rowPins[ROWS] = {11, 10, 9, 8};
byte colPins[COLS] = {7, 6, 5, 4};
Keypad keypad = Keypad(
	makeKeymap(keys),
	rowPins,
	colPins,
	ROWS,
	COLS
);

// LCD pins
LiquidCrystal lcd(13, 12, A2, A3, A4, A5);

char operations[5] = {'+','-','*','/'};
bool status = false;
bool getting_operation = false;
String lcd_text = "";
int prev_val = 0;

// Init serial connection
// Print welcome message
// Print menu
void setup() {
	pinMode(2, OUTPUT);
	pinMode(3, OUTPUT);
	pinMode(A0, OUTPUT);
	pinMode(A1, OUTPUT);
	
	Serial.begin(BAUD_RATE);
	lcd.begin(16, 2);
	lcd.print("Ali Sedaghi");
	delay(1000);
	lcd.clear();
}

void update_leds(int result_int) {
	int numBits = 4;
	int bin[numBits];
	for (int i = 0; i < numBits; i++){
        bin[i] = 1 & (result_int >> i);
    }
	
	if (bin[0] == 1) {
		digitalWrite(2, HIGH);
	}
	else {
		digitalWrite(2, LOW);
	}
	
	if (bin[1] == 1) {
		digitalWrite(3, HIGH);
	}
	else {
		digitalWrite(3, LOW);
	}
	
	if (bin[2] == 1) {
		digitalWrite(A0, HIGH);
	}
	else {
		digitalWrite(A0, LOW);
	}
	
	if (bin[3] == 1) {
		digitalWrite(A1, HIGH);
	}
	else {
		digitalWrite(A1, LOW);
	}
}

// Update lcd_text variable
void update_text(String str) {
	lcd_text = lcd_text + str;
}

// Menu printer
void print_menu() {
	Serial.println("Enter option number on keypad then enter D.");
	Serial.println("5. Sin");
	Serial.println("6. Cos");
	Serial.println("7. Tan");
	Serial.println("8. Start");
	
	Serial.println("Operations:");
	Serial.println("1. +");
	Serial.println("2. -");
	Serial.println("3. *");
	Serial.println("4. /");

	Serial.println("");
}

// Display Operation
String operation(int num) {
	String result = "";
	while (1) {
		if (num == 1) {
			result = "+";
		}
		else if (num == 2) {
			result = "-";
		}
		else if (num == 3) {
			result = "*";
		}
		else if (num == 4) {
			result = "/";
		}
		else if (num == 5) {
			result = "sin: ";
		}
		else if (num == 6) {
			result = "cos: ";
		}
		else if (num == 7) {
			result = "tan: ";
		}
		break;
	}
	update_text(result);
	return result;
}

// Taking input
int get_number() {
	int number = 0;
	while (1) {
		char Key = keypad.getKey();
		if (Key >= '0' && Key <= '9' && getting_operation == false) {
			number = number * 10 + (Key - '0');
			lcd.clear();
			String myString = String(Key);
			update_text(myString);
			lcd.print(lcd_text);
		}
		if (Key == 'D') {
			break;
		}
		else if (getting_operation == true) {
			number = Key - '0';
			if (number == 1) {
				update_text("+");
				lcd.clear();
				lcd.print(lcd_text);
				return number;
			}
			else if (number == 2) {
				update_text("-");
				lcd.clear();
				lcd.print(lcd_text);
				return number;
			}
			else if (number == 3) {
				update_text("*");
				lcd.clear();
				lcd.print(lcd_text);
				return number;
			}
			else if (number == 4) {
				update_text("/");
				lcd.clear();
				lcd.print(lcd_text);
				return number;
			}
			else {
			}
		}
		
	}
	return number;
}

// Start Computation - Option 8
void Start() {
	String input_takers[16];
	lcd_text = "";
	Serial.println("For entering operand press A then enter operand then press D.");
	Serial.println("For entering operation press C then enter operation.");
	Serial.println("");
	int i = 0;
	while (1) {
		char Key = keypad.getKey();
		if (Key != NO_KEY) {
			if (Key == 'B') {
				Serial.println("B: Stop taking input.");
				break;
			}
			if (Key == 'C') {
				getting_operation = true;
				Serial.println("C: Enter the operation number. (1:+  2:-  3:*  4:/)");
				String prev_show1 = lcd_text;
				int value = get_number();
				lcd_text = prev_show1;
				Serial.print("Got operation: ");
				
				
				if (value == 1) {
					// Add
					Serial.println("+");
					Serial.println("");
					String myString1 = String(operations[value-1]);
					update_text(myString1);
					lcd.clear();
					lcd.print(lcd_text);
					input_takers[i] = myString1;
					i++;
				}
				else if (value == 2) {
					// Subtract
					Serial.println("-");
					Serial.println("");
					String prev_show2 = lcd_text;
					String myString2 = String(operations[value-1]);
					lcd_text = prev_show2;
					update_text(myString2);
					lcd.clear();
					lcd.print(lcd_text);
					input_takers[i] = myString2;
					i++;
				}
				else if (value == 3) {
					// Multiply
					Serial.println("*");
					Serial.println("");
					String prev_show3 = lcd_text;
					String myString3 = String(operations[value-1]);
					lcd_text = prev_show3;
					update_text(myString3);
					lcd.clear();
					lcd.print(lcd_text);
					input_takers[i] = myString3;
					i++;
				}
				else if (value == 4) {
					// Divide
					Serial.println("/");
					Serial.println("");
					String prev_show4 = lcd_text;
					String myString4 = String(operations[value-1]);
					lcd_text = prev_show4;
					update_text(myString4);
					lcd.clear();
					lcd.print(lcd_text);
					input_takers[i] = myString4;
					i++;
				}
				else {
					Serial.println("Invalid");
					Serial.println("");
				}
				getting_operation = false;
			}
			else if (Key == 'A') {
				Serial.println("A: Enter the operand then press D.");
				int value1 = get_number();
				Serial.print("Got numeric value: ");
				Serial.println(value1);
				Serial.println("");
				lcd.clear();
				lcd.print(lcd_text);
				String myString = String(value1);
				input_takers[i] = myString;
				i++;
			}
		}
	}
	int p = 0;
	while (p <= 16) {
		// Display Operands and Operations
		Serial.print(input_takers[p]);
		Serial.print(" ");
		p++;
	}
	
	Serial.println("");
	
  // Computation
	while (i != 1) {
		int j = 0;
		bool found1 = true;
		bool found2 = true;
		bool found3 = true;
		while(j < i && i > 1){
			if (input_takers[j] == "/") {
				// Division
				int val1 = input_takers[j-1].toInt();
				int val2 = input_takers[j+1].toInt();
				float res = val1 / val2;
				input_takers[j] = ".";
				input_takers[j+1] = ".";
				String myString = String(res);
				input_takers[j-1] = myString;
				found1 = false;
				break;
			}
			j++;
		}
		j = 0;
		while (j < i && i > 1 && found1 == true) {
			if (input_takers[j] == "*") {
				// Multiplication
				float val1 = input_takers[j-1].toFloat();
				float val2 = input_takers[j+1].toFloat();
				float res = val1 * val2;
				input_takers[j] = ".";
				input_takers[j+1] = ".";
				String myString = String(res);
				input_takers[j-1] = myString;
				found2 = false;
				break;
			}
			j++;
		}
		j = 0;
			while (j < i && i > 1 && found1 == true && found2 == true) {
				if (input_takers[j] == "-") {
					// Subtraction
					float val1 = input_takers[j-1].toFloat();
					float val2 = input_takers[j+1].toFloat();
					float res = val1 - val2;
					input_takers[j] = ".";
					input_takers[j+1] = ".";
					String myString = String(res);
					input_takers[j-1] = myString;
					found3 = false;
					break;
				}
				j++;
			}
			j = 0;
			while (j < i && i > 1 && found1 == true && found2 == true && found3 == true) {
				if (input_takers[j] == "+") {
					// Addition
					float val1 = input_takers[j-1].toFloat();
					float val2 = input_takers[j+1].toFloat();
					float res = val1 + val2;
					input_takers[j] = ".";
					input_takers[j+1] = ".";
					String myString = String(res);
					input_takers[j-1] = myString;
					break;
				}
				j++;
			}
			int d = 0;
			while (d < i-2) {
				if (input_takers[d] == ".") {
					String temp = input_takers[d];
					input_takers[d] = input_takers[d+2];
					input_takers[d+2] = temp;
				}
				d++;
			}
			i = i-2;
	}
	// Final result
	float result = input_takers[0].toFloat();
	int result_int = (int)result;
	update_leds(result_int);
	lcd.setCursor(0,2);
	lcd.print(result);
	Serial.print("Result: ");
	Serial.print(result);
	Serial.println("");
	Serial.println("");
	delay(3000);
}


void loop() {
	update_leds(0);
	print_menu();
	status = false;
	int entered_value = get_number();
	Serial.print("Option chosen: ");
	Serial.print(entered_value);
	Serial.println();
	lcd.clear();
	lcd.print("Option chosen: ");
	lcd.setCursor(0, 2);
	lcd.print(entered_value);
	delay(1000);
	
	// Start
	if (entered_value == 8) {
		Start();
	}
	
	else if (entered_value >= 5 && entered_value <= 7) {
		lcd_text = "";
		prev_val = process(entered_value, status);

		while (1) {
			status = true;
			Serial.println("");
			Serial.println("Press D to start new computation.");
			lcd_text = "";
			int entered_value1 = get_number();
			if (entered_value1 == 0) {
				Serial.println("Starting new computation.");
				Serial.println("");
				break;
			}
			else {
				Serial.print("Option chosen :");
				Serial.println(entered_value);
				lcd.clear();
				lcd_text = "";
				String myString = String(prev_val);
				update_text(myString);
				lcd.print(lcd_text);
				prev_val = process(entered_value1,status);
			}
		}
	}
	
	else {
		Serial.println("Entered option is not valid.");
		Serial.println("Choose between 5, 6, 7, 8.");
		Serial.println("");
	}
	delay(3000);
	lcd.clear();
	lcd_text = "";
}

//Trigonometric functions (Option 5 to 7)
float process(int num, bool status) {
	float val1 = prev_val;
	if (num == 5) {
		lcd.clear();
		lcd_text = "";
		String oper = operation(num);
		lcd.clear();
		Serial.println(oper);
		if (status == true) {
			String myString = String(val1);
			update_text(myString);
			lcd.print(lcd_text);
		}
		else {
			Serial.println("Enter the sin argument.");
			val1 = (float)get_number();
			Serial.println(val1);
		}
		lcd.setCursor(0,2);
		// Sine function
		float result = sin(val1);
		int result_int = (int)result;
		update_leds(result_int);
		lcd.print(result);
		Serial.print("Result: ");
		Serial.print(result);
		Serial.println("");
		return result;
	}
	else if (num == 6) {
		lcd.clear();
		lcd_text = "";
		String oper = operation(num);
		lcd.clear();
		Serial.println(oper);
		if (status == true) {
			String myString = String(val1);
			update_text(myString);
			lcd.print(lcd_text);
		}
		else {
			Serial.println("Enter the cos argument.");
			val1 = (float)get_number();
			Serial.println(val1);
		}
		lcd.setCursor(0,2);
		// Cosine function
		float result = cos(val1);
		int result_int = (int)result;
		update_leds(result_int);
		lcd.print(result);
		Serial.print("Result: ");
		Serial.print(result);
		Serial.println("");
		return result;
	}
	else if (num == 7) {
		lcd.clear();
		lcd_text = ""; 
		String oper = operation(num);
		lcd.clear();
		Serial.println(oper);
		if (status == true) {
			String myString = String(val1);
			update_text(myString);
			lcd.print(lcd_text);
		}
		else {
			Serial.println("Enter the tan argument.");
			val1 = (float)get_number();
			Serial.println(val1);
		}
		lcd.setCursor(0,2);
		// Tan function
		float result = tan(val1);
		int result_int = (int)result;
		update_leds(result_int);
		lcd.print(result);
		Serial.print("Result: ");
		Serial.print(result);
		Serial.println("");
		return result;
	}
}

