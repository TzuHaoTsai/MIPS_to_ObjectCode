#include<iostream>
#include<cstring>
#include<fstream>
#include<sstream>

using namespace std;

struct optable{
	string name;
	string value;
};

struct regtable{
	string name;
	string value;
};

struct location{
	string label;
	int loc;
};

struct sftable{
	string name;
	string shamt;
	string func;
};

struct obtable{
	string op;
	string opname;
	string command;
	string label;
	string obtemp;
};

void create_optable(struct optable op_table[]);
void create_regtable(struct regtable reg_table[]);
void create_sftable(struct sftable sf_table[]);
void input_MIPS(struct optable op_table[], struct obtable ob_table[], struct regtable reg_table[], 
				struct sftable sf_table[],struct location loc[]);
void cheak_label(struct optable op_table[], struct obtable ob_table[], struct regtable reg_table[], 
				struct sftable sf_table[], struct location loc[], string line[], int correct, int &shift, int &number, string &obtemp);
string ten_to_binary(string tennum, int choose);
int op_zero(int newnum);
int reg_zero(int newnum);
int imm_zero(int newnum);
int addr_zero(int newnum);
string int_to_str(int tennum);

int main(){
	optable op_table[12];
	regtable reg_table[32];
	obtable ob_table[100];
	sftable sf_table[3];
	location loc[100];
	
	create_optable(op_table);
	create_regtable(reg_table);
	create_sftable(sf_table);
	
	input_MIPS(op_table, ob_table, reg_table, sf_table, loc);	
}

void create_optable(struct optable op_table[]){
	
	ifstream file;
	file.open("optable.txt",std::ifstream::in);
	
	string get_string;
	
	for(int i=0 ; i<12 ; i++){
		file >> get_string;
		op_table[i].name = get_string;
		file >> get_string;
		op_table[i].value = get_string;
	}
	/*
	for(int i=0 ; i<12 ; i++){
		cout << i+1 << ". " << op_table[i].name;
		cout << " : " << op_table[i].value << endl;
	}	
	*/
	file.close();
}

void create_regtable(struct regtable reg_table[]){
	
	ifstream file;
	file.open("regtable.txt",std::ifstream::in);
	
	string get_string;
	
	for(int i=0 ; i<32 ; i++){
		file >> get_string;
		reg_table[i].name = get_string;
		file >> get_string;
		reg_table[i].value = get_string;
	}
	/*
	for(int i=0 ; i<31 ; i++){
		cout << i+1 << ". " << reg_table[i].name;
		cout << " : " << reg_table[i].value << endl;
	}
	*/
	file.close();
} 

void create_sftable(struct sftable sf_table[]){
	ifstream file;
	
	file.open("sftable.txt",std::ifstream::in);
	stringstream SS;
	
	string get_string;
	
	for(int i=0 ; i<3 ; i++){
		file >> get_string;
		sf_table[i].name = get_string;
		file >> get_string;
		sf_table[i].shamt = get_string;
		file >> get_string;
		sf_table[i].func = get_string;
	}
	/*
	for(int i=0 ; i<3 ; i++){
		cout << i+1 << ". " << sf_table[i].name;
		cout << " : " << sf_table[i].shamt;
		cout << "  " << sf_table[i].func << endl;
	}
	*/
	file.close();
}

void input_MIPS(struct optable op_table[], struct obtable ob_table[], struct regtable reg_table[], 
				struct sftable sf_table[], struct location loc[]){
	string my_string;
	bool correct = true;
	int shift = 0;
	int number = 0;
	string obtemp = "";
	
 	cout << "If you wanna end the program. Input e ..." << endl << "MIPS : " << endl << "_________________________________________" << endl << endl;
 	
  	while(getline(cin, my_string, '\n')) {
  		
  		bool cheak_read = true;
  		
  		if(my_string == ""){
 			cheak_read = false;
 			cout << "Enter MIPS instruction !!!" << endl;
		}
  		
  		if(cheak_read == true){						
	  		string line[6] = "";
	    	char *pch;
	    	char str[128];
	    	int i=0;
	   		int leave = 0;									
	   		correct = true;								
	   		
	    	strcpy(str, my_string.c_str()); 
	 	
	    	pch = strtok(str, " ,:()");  
	    	
	    	while(pch != NULL) {
	      		line[i] = pch;
	      		i++;
	      		pch = strtok(NULL, " ,:()"); 
	      		
	      		leave++;
				
				if(leave == 6){								
					cout << endl <<"wrong format !!" << endl;
					exit(0);
				}
	    	}
	    			
			if(my_string == "e"){
				exit(0);
			}
			
			cheak_label(op_table, ob_table, reg_table, sf_table, loc, line, correct, shift, number, obtemp);
		}
	}
}

void cheak_label(struct optable op_table[], struct obtable ob_table[], struct regtable reg_table[], 
				struct sftable sf_table[], struct location loc[], string line[], int correct, int &shift, int &number, string &obtemp){
	int flag = 0;	
	int com_index;
	string com_name;
	
	string obcode; 
	string test;
	
	if(line[1] == ""){								//label
		loc[number].label = line[0];
		loc[number].loc = 0 + shift*4;
		cout << endl << "location : " << hex << loc[number].loc << endl;


			for(int i=0 ; i<50 ; i++){
				if((line[0] == ob_table[i].label)){
					if((ob_table[i].opname == "beq") || (ob_table[i].opname == "bne")){
						cout << "find label : " << ob_table[i].command << endl << "object code : " << ob_table[i].obtemp << ten_to_binary((int_to_str(loc[number].loc/4)), 2) << endl;
					}
					if((ob_table[i].opname == "j") || (ob_table[i].opname == "jal")){
						cout << "find label : " << ob_table[i].command << endl << "object code : " << ob_table[i].obtemp << ten_to_binary((int_to_str(loc[number].loc/4)), 3) << endl;
					}
				}
			}

		cout << "______________________________________________" << endl << endl;
	}
	else{
		for(int i=0 ; i<12 ; i++){					//find label index
			if(line[0] == op_table[i].name){
				com_index = i;						//get command index 
				com_name = op_table[i].name;
			}
		}
	
		loc[number].label = "";
		loc[number].loc = 0 + shift*4;
		cout << endl << "location : " << hex << loc[number].loc << endl;
				
		if(line[0] == com_name){

			ob_table[number].op = op_table[com_index].value;		
			obcode = ten_to_binary(ob_table[number].op, 0);
			
			string str = "";
			
			if(com_name == "add" || com_name == "sub" || com_name == "slt"){		
				for(int i=0 ; i<32 ; i++){				
					if(line[2] == reg_table[i].name){					
						obcode += ten_to_binary(reg_table[i].value, 1);
					}
				}
				for(int i=0 ; i<32 ; i++){				
					if(line[3] == reg_table[i].name){					
						obcode += ten_to_binary(reg_table[i].value, 1);
					}
				}	
				for(int i=0 ; i<32 ; i++){	
					if(line[1] == reg_table[i].name){					
						obcode += ten_to_binary(reg_table[i].value, 1);
					}		
				}
				obcode += ten_to_binary(sf_table[com_index].shamt,1);
				obcode += ten_to_binary(sf_table[com_index].func,0);
			}
			else if(com_name == "beq" || com_name == "bne" || com_name == "addi" || com_name == "slti"){
				for(int i=0 ; i<32 ; i++){
					if(line[2] == reg_table[i].name){					//rs
						obcode += ten_to_binary(reg_table[i].value, 1);
					}
				}
				for(int i=0 ; i<32 ; i++){
					if(line[1] == reg_table[i].name){					//rt
						obcode += ten_to_binary(reg_table[i].value, 1);
					}
				}
				if(com_name == "beq" || com_name == "bne"){
										
					bool flag = true;
					
					for(int i=0 ; i<50 ; i++){
						if(line[3] == loc[i].label){
							flag = false;
							obcode += ten_to_binary((int_to_str(loc[i].loc/4)), 2);
						}
					}
					
					if(flag == true){							
						ob_table[number].label = line[3];
						ob_table[number].obtemp = obcode;
						
						ob_table[number].command += line[0];	
						ob_table[number].command += " ";
						for(int i=1 ; i<3 ; i++){
							ob_table[number].command += line[i];
							ob_table[number].command += ", ";
						}
						ob_table[number].command += line[3];
						
						ob_table[number].opname = line[0];
						
						obcode += "xxxxxxxxxxxxxxxx";
					}
				}
				if(com_name == "addi" || com_name == "slti"){		
					obcode += ten_to_binary(line[3], 2);
					if(line[3] == "1"){								
						obcode += "1";
					}
				}
			}
			else if(com_name == "lw" || com_name == "sw"){				
				for(int i=0 ; i<32 ; i++){								
					if(line[3] == reg_table[i].name){					
						obcode += ten_to_binary(reg_table[i].value, 1);
					}
				}
				for(int i=0 ; i<32 ; i++){								
					if(line[1] == reg_table[i].name){					
						obcode += ten_to_binary(reg_table[i].value, 1);
					}
				}
				obcode += ten_to_binary(line[2], 2);
			}
			else if(com_name == "sll"){
				
				obcode += "00000";										
				for(int i=0 ; i<32 ; i++){								
					if(line[2] == reg_table[i].name){					
						obcode += ten_to_binary(reg_table[i].value, 1);
					}
				}
				for(int i=0 ; i<32 ; i++){								
					if(line[1] == reg_table[i].name){					
						obcode += ten_to_binary(reg_table[i].value, 1);
					}
				}
				obcode += ten_to_binary(line[3], 1);
				
				obcode += "000000";
			}
			else if(com_name == "j" || com_name == "jal"){
				
				bool flag = true;
				string temp = "";
				
				for(int i=0 ; i<50 ; i++){
					if(line[1] == loc[i].label){
						flag = false;
						temp = ten_to_binary((int_to_str(loc[i].loc/4)), 3);
						obcode += ten_to_binary((int_to_str(loc[i].loc/4)), 3);
					}
				}
				
				if(flag == true){
					ob_table[number].label = line[1];
					ob_table[number].obtemp = obcode;
					
					ob_table[number].command += line[0];
					ob_table[number].command += " ";
					ob_table[number].command += line[1];
					
					ob_table[number].opname = line[0];
					
					obcode += "xxxxxxxxxxxxxxxxxxxxxxxxxx";
				}
			}
		}
		cout << "object code : " << obcode << endl << "______________________________________________" << endl << endl;
	}
	
	shift++;
	number++;
}

string ten_to_binary(string tennum, int choose){
    string binnum = "";
	stringstream ss;
	int remainder;
	int quotient;
	int newnum;
	int zero_bit = 0;
	string temp;
	int flag = 0;
	
	ss.str("");
	ss.clear();
	
	ss << tennum;
	ss >> newnum;
	
	if(choose == 0){
		zero_bit = op_zero(newnum);
	}
	if(choose == 1){
		zero_bit = reg_zero(newnum);
	}
	if(choose == 2){
		zero_bit = imm_zero(newnum); 
	}
	if(choose == 3){
		zero_bit = addr_zero(newnum);
	}
	
	quotient = (newnum / 2);
	
	while(quotient != 0){
		
		remainder = (newnum % 2);
		quotient = (newnum / 2);
		newnum /= 2;
		
		ss.str("");
		ss.clear();
		temp = "";
		
		ss << remainder;
		ss >> temp;
		
		binnum += temp;
	}
	
	temp = "";

	for(int i=0 ; i<zero_bit ; i++){
		temp += "0";
	}
	
	for(int i=binnum.length()-1 ; i>=0 ; i--){
		temp += binnum[i];
	}
	
	binnum = temp;
	
    return binnum;
}

int op_zero(int newnum){
	
	int zero_bit = 0;
	
	if(newnum == 0){
		zero_bit = 6;
	}
	else if(newnum == 1){
		zero_bit = 5;
	}
	else if(newnum>=2 && newnum<=3){
		zero_bit = 4;
	}
	else if(newnum>=4 && newnum<=7){
		zero_bit = 3;
	}
	else if(newnum>=8 && newnum<=15){
		zero_bit = 2;
	}
	else if(newnum>=16 && newnum<=31){
		zero_bit = 1;
	}
	return zero_bit;
} 

int reg_zero(int newnum){
	
	int zero_bit = 0;
	
	if(newnum == 0){
		zero_bit = 5;
	}
	else if(newnum == 1){
		zero_bit = 4;
	}
	else if(newnum>=2 && newnum<=3){
		zero_bit = 3;
	}
	else if(newnum>=4 && newnum<=7){
		zero_bit = 2;
	}
	else if(newnum>=8 && newnum<=15){
		zero_bit = 1;
	}
	return zero_bit;
} 

int imm_zero(int newnum){
	int zero_bit = 0;
	
	if(newnum == 0){
		zero_bit = 16;
	}
	else if(newnum == 1){
		zero_bit = 15;
	}
	else if(newnum>=2 && newnum<=3){
		zero_bit = 14;
	}
	else if(newnum>=4 && newnum<=7){
		zero_bit = 13;
	}
	else if(newnum>=8 && newnum<=15){
		zero_bit = 12;
	}
	else if(newnum>=16 && newnum<=31){
		zero_bit = 11;
	}
	else if(newnum>=32 && newnum<=63){
		zero_bit = 10;
	}
	else if(newnum>=64 && newnum<=127){
		zero_bit = 9;
	}
	else if(newnum>=128 && newnum<=255){
		zero_bit = 8;
	}
	else if(newnum>=256 && newnum<=511){
		zero_bit = 7;
	}
	else if(newnum>=512 && newnum<=1023){
		zero_bit = 6;
	}
	else if(newnum>=1024 && newnum<=2047){
		zero_bit = 5;
	}
	else if(newnum>=2048 && newnum<=4095){
		zero_bit = 4;
	}
	else if(newnum>=4096 && newnum<=8191){
		zero_bit = 3;
	}
	else if(newnum>=8192 && newnum<=16383){
		zero_bit = 2;
	}
	else if(newnum>=16384 && newnum<=32767){
		zero_bit = 1;
	}
	
	return zero_bit;	
}

int addr_zero(int newnum){
	int zero_bit = 0;
	
	if(newnum == 0){
		zero_bit = 26;
	}
	else if(newnum == 1){
		zero_bit = 25;
	}
	else if(newnum>=2 && newnum<=3){
		zero_bit = 24;
	}
	else if(newnum>=4 && newnum<=7){
		zero_bit = 23;
	}
	else if(newnum>=8 && newnum<=15){
		zero_bit = 22;
	}
	else if(newnum>=16 && newnum<=31){
		zero_bit = 21;
	}
	else if(newnum>=32 && newnum<=63){
		zero_bit = 20;
	}
	else if(newnum>=64 && newnum<=127){
		zero_bit = 19;
	}
	else if(newnum>=128 && newnum<=255){
		zero_bit = 18;
	}
	else if(newnum>=256 && newnum<=511){
		zero_bit = 17;
	}
	else if(newnum>=512 && newnum<=1023){
		zero_bit = 16;
	}
	else if(newnum>=1024 && newnum<=2047){
		zero_bit = 15;
	}
	else if(newnum>=2048 && newnum<=4095){
		zero_bit = 14;
	}
	else if(newnum>=4096 && newnum<=8191){
		zero_bit = 13;
	}
	else if(newnum>=8192 && newnum<=16383){
		zero_bit = 12;
	}
	else if(newnum>=16384 && newnum<=32767){
		zero_bit = 11;
	}
	else if(newnum>=32768 && newnum<=65535){
		zero_bit = 10;
	}
	else if(newnum>=65536 && newnum<=131071){
		zero_bit = 9;
	}
	else if(newnum>=131072 && newnum<=262143){
		zero_bit = 8;
	}
	else if(newnum>=262144 && newnum<=524287){
		zero_bit = 7;
	}
	else if(newnum>=524288 && newnum<=1048575){
		zero_bit = 6;
	}
	else if(newnum>=1048576 && newnum<=2097151){
		zero_bit = 5;
	}
	else if(newnum>=2097152 && newnum<=4194303){
		zero_bit = 4;
	}
	else if(newnum>=4194304 && newnum<=8388607){
		zero_bit = 3;
	}
	else if(newnum>=8388608 && newnum<=16777215){
		zero_bit = 2;
	}
	else if(newnum>=16777216 && newnum<=33554431){
		zero_bit = 1;
	}
	return zero_bit;	
}

string int_to_str(int tennum){
	stringstream ss;
	string newnum;
	
	ss << tennum;
	ss >> newnum;
	
	return newnum;
}
